#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mainToolBar->hide();
    ui->statusBar->hide();
    errorLoadingFileLabel = new QLabel(this);
    firstHide = true;
    alignAndResize();
    createTrayActions();
    createTrayIcon();
    if (!loadSettings(QDir::currentPath() + "/debug/" + "settings.ini")) {
        browserPath = "";
    }
    userIniFilename = QDir::currentPath() + "/debug/" + qgetenv("USERNAME") + ".ini";  // TODO: Consider using WinApi (https://stackoverflow.com/questions/26552517/get-system-username-in-qt)
    updateActions();
    if (!FileWriter::exists(userIniFilename)) {
        errorLoadingFileMsg();
    } else {
        //initialiseInterface();
        interfaceTest();
    }
}

void MainWindow::updateActions() {
    DatabaseManager db = DatabaseManager();
    if (db.instantiateConnection("asumenu","127.30.4.69",443,"Student","12345")) {
        FileWriter::write(userIniFilename,IniParser::createIniString(db.initialiseData(db.userActions(db.userIdByName(qgetenv("USERNAME"))))));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::alignAndResize() {
    QSize desktopSize = qApp->desktop()->availableGeometry().size();
    windowSize = QSize(desktopSize.width() * 0.4, desktopSize.height() * 0.9);
    this->resize(windowSize);
    this->move(desktopSize.width() - windowSize.width() - 20, 20);
}

void MainWindow::errorLoadingFileMsg() {  // TODO: add retry button
    QFont f("Helvetica", 10, QFont::Bold);
    errorLoadingFileLabel->setFont(f);
    errorLoadingFileLabel->setStyleSheet("QLabel { color : red; }");
    errorLoadingFileLabel->setText("Не удается найти файл с настройками для текущего пользователя системы!");
    errorLoadingFileLabel->setAlignment(Qt::AlignCenter);
    MainWindow::setCentralWidget(errorLoadingFileLabel);
}

void MainWindow::initialiseInterface() {
    QList<IniSection> iniSections = IniParser::parse(userIniFilename);
    QTabWidget *tabWidget = new QTabWidget;
    tabWidget->setFont(QFont("Helvetica",12));
    MainWindow::setCentralWidget(tabWidget);
    for (int i = 0; i < iniSections.count(); i++) {
        QWidget *newTab = new QWidget(tabWidget);
        QVBoxLayout *layout = new QVBoxLayout;
        for (int j = 0; j < iniSections[i].itemList.count(); j++) {
            QPushButton *newButton = new QPushButton(iniSections[i].itemList[j]->buttonName);
            newButton->setMinimumHeight(windowSize.height()/12);
            layout->addWidget(newButton);
            addButtonAction(newButton, iniSections[i].itemList[j]->buttonAction, iniSections[i].itemList[j]->args);
        }
        layout->addStretch();
        newTab->setLayout(layout);
        tabWidget->addTab(newTab, iniSections[i].sectionName);
    }
}

void MainWindow::interfaceTest() {
    // initialise a widget with buttons for sections
    windowSections = new QWidget;
    windowSections->setFont(QFont("Helvetica",12));
    QList<IniSection> iniSections = IniParser::parse(userIniFilename);
    QVBoxLayout *sections = new QVBoxLayout;
    windowSections->setLayout(sections);
    MainWindow::setCentralWidget(windowSections);
    for (int i = 0; i < iniSections.count(); i++) {
        QPushButton *newSectionButton = new QPushButton(iniSections[i].sectionName);
        newSectionButton->setMinimumHeight(windowSize.height()/12);
        sections->addWidget(newSectionButton);
        addSectionButtonAction(newSectionButton,i);
        QWidget *windowActionButtons = new QWidget;
        windowActionButtons->setFont(QFont("Helvetica",12));
        QVBoxLayout *layoutActions = new QVBoxLayout;
        windowActionButtons->setLayout(layoutActions);
        layoutActions->addWidget(new QLabel(iniSections[i].sectionName));
        // we initialise buttons for actions and add them to the list of widgets containing buttons for sections
        for (int j = 0; j < iniSections[i].itemList.count(); j++) {
            QPushButton *newButton = new QPushButton(iniSections[i].itemList[j]->buttonName);
            newButton->setMinimumHeight(windowSize.height()/12);
            layoutActions->addWidget(newButton);
            addButtonAction(newButton, iniSections[i].itemList[j]->buttonAction, iniSections[i].itemList[j]->args);
        }
        layoutActions->addStretch();
        sectionsButtons.append(windowActionButtons);
    }
    sections->addStretch();
}

void MainWindow::addSectionButtonAction(QPushButton *button, int index) {
    QSignalMapper* signalMapper = new QSignalMapper(this);
    signalMapper->setMapping(button, index);
    connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(sectionButtonClicked(int)));
    connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));
}

void MainWindow::addButtonAction(QPushButton *button, QString action, QString args) {
    QSignalMapper* signalMapper = new QSignalMapper(this);
    QString actionWithArgs = action + '\n' + args;
    signalMapper->setMapping(button, actionWithArgs);
    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(buttonClicked(QString)));
    connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));
}

void MainWindow::buttonClicked(QString data) {
    QStringList keyValue = data.split('\n');
    KeyValue<QString,QString> action = KeyValue<QString,QString>(keyValue[0],keyValue[1]);
    QRegExp regExp("^(((http|ftp)(s?)://)|(www.))(([a-zA-Z0-9-.]+(.[a-zA-Z0-9-.]+)+)|localhost)(/?)([a-zA-Z0-9-.?,'/\\+&%$#_])?([\\d\\w./%+-=&amp;?:\\&quot;',|~;])$");
    regExp.indexIn(action.key);
    QFileInfo info = QFileInfo(action.key);
    if (regExp.cap(0).length() != 0) {
        if (browserPath == "") {
            QDesktopServices::openUrl(QUrl(action.key));
        } else {
            QProcess::startDetached(browserPath, QStringList(action.key));
        }
    } else if (info.isExecutable()) {
        QProcess::startDetached(('"' + action.key + '"'),QStringList(action.value));
    }
}

void MainWindow::sectionButtonClicked(int index) {
    MainWindow::setCentralWidget(sectionsButtons[index]);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    hide();
    trayIcon->show();
    event->ignore();
    if (firstHide) {
        showMessageTray();
        firstHide = false;
    }
}

void MainWindow::createTrayActions() {
    restoreAction = new QAction(tr("&Открыть меню"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    quitAction = new QAction(tr("&Выйти"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::createTrayIcon() {
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    trayIcon->setIcon(QIcon(":icons/icon.ico"));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayIconClicked(QSystemTrayIcon::ActivationReason)));
}

void MainWindow::trayIconClicked(QSystemTrayIcon::ActivationReason reason) {
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        showNormal();
        this->activateWindow();
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    default:
        break;
    }
}

void MainWindow::changeEvent(QEvent* e)
{
   if (e->type() == QEvent::ActivationChange && !(this->isActiveWindow()))
       emit close();
}

void MainWindow::showMessageTray() {
    QString message("Чтобы закрыть программу выберете в контекстном меню пункт 'Выйти'.");
    trayIcon->showMessage("АСУ-Меню", message, QSystemTrayIcon::MessageIcon(1), 1000);
}

bool MainWindow::loadSettings(QString settingFileName) {
    if (FileWriter::exists(settingFileName)) {
        QList<IniSection> parseResult = IniParser::parse(settingFileName);
        browserPath = IniParser::valueByKey(parseResult, "browser");
        return true;
    }
    return false;
}
