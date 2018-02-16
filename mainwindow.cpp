#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint);
    QGuiApplication::setQuitOnLastWindowClosed(false);
    errorLoadingFileLabel = new QLabel(this);
    firstHide = true;
    alignAndResize();
    createTrayActions();
    createTrayIcon();
    setOnStartup();
    if (!loadSettings(QCoreApplication::applicationDirPath() + "/settings.ini")) {
        browserPath = "";
    }
    updateActions();
    if (FileWriter::exists(userIniFilename)) {
        initialiseInterface();
    }
}

void MainWindow::updateActions() {
    DatabaseManager db = DatabaseManager();
    if (db.instantiateConnection("asumenu","Student","12345")) {
        int userID = db.userIdByName(qgetenv("USERNAME"));
        if (userID != -1) {
            FileWriter::write(userIniFilename,IniParser::createIniString(db.initialiseData(db.userActions(userID))));
        }
    } else {
        QMessageBox warning;
        warning.setText("Не удалось подключиться к базе данных! Будут загружены оффлайн-данные, если они"
                        " доступны. Обратитесь к администратору.");
        warning.setDetailedText(db.getError());
        warning.exec();
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
    this->move(desktopSize.width() - windowSize.width() - 20, 25);
}

void MainWindow::initialiseInterface() {
    // initialise a widget with buttons for sections
    windowSections = new QWidget;
    windowSections->setFont(QFont("Helvetica",14));
    QList<IniSection> iniSections = IniParser::parse(userIniFilename);
    QVBoxLayout *sections = new QVBoxLayout;
    windowSections->setLayout(sections);
    // мы отдельно обрабатываем секцию с индексом 0
    for (int i = 0; i < iniSections[0].itemList.count(); i++) {
        QPushButton *newButton = new QPushButton(iniSections[0].itemList[i]->buttonName);
        newButton->setMinimumHeight(windowSize.height()/12);
        newButton->setStyle(new QtPushButtonStyleProxy());
        newButton->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Minimum);
        addButtonAction(newButton, iniSections[0].itemList[i]->buttonAction, iniSections[0].itemList[i]->args);
        newButton->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Fixed);
        sections->addWidget(newButton);
    }
    for (int i = 1; i < iniSections.count(); i++) {
        QPushButton *newSectionButton = new QPushButton("→ " + iniSections[i].sectionName);
        newSectionButton->setMinimumHeight(windowSize.height()/12);
        newSectionButton->setStyle(new QtPushButtonStyleProxy());
        newSectionButton->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Minimum);
        sections->addWidget(newSectionButton);
        addSectionButtonAction(newSectionButton,(i - 1)); // отнимает 1 поскольку проходим с единицы
        QWidget *windowActionButtons = new QWidget;
        QWidget *actionButtons = new QWidget;
        windowActionButtons->setFont(QFont("Helvetica",14));
        QVBoxLayout *mainLayout = new QVBoxLayout;
        QVBoxLayout *layoutActions = new QVBoxLayout;
        QHBoxLayout* topLayout = new QHBoxLayout;
        windowActionButtons->setLayout(mainLayout);
        actionButtons->setLayout(layoutActions);
        QPushButton *backButton = new QPushButton();
        backButton->setIcon(QIcon(":/icons/back-button.png"));
        backButton->setIconSize(QSize(windowSize.height()/12,windowSize.height()/12));
        backButton->setStyleSheet("QPushButton{background: transparent;}");
        backButton->setMinimumHeight(windowSize.height()/12);
        backButton->setFixedWidth(backButton->minimumHeight());
        connect(backButton, SIGNAL(released()), this, SLOT(backButtonClick()));
        topLayout->addWidget(backButton);
        topLayout->addStretch();
        QLabel *groupLabel = new QLabel(iniSections[i].sectionName);
        groupLabel->setMaximumWidth(windowSize.width() - backButton->width() - 55);
        topLayout->addWidget(groupLabel);
        topLayout->addStretch(3);
        // we initialise buttons for actions and add them to the list of widgets containing buttons for sections
        for (int j = 0; j < iniSections[i].itemList.count(); j++) {
            QPushButton *newButton = new QPushButton(iniSections[i].itemList[j]->buttonName);
            newButton->setMinimumHeight(windowSize.height()/12);
            newButton->setStyle(new QtPushButtonStyleProxy());
            newButton->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Minimum);
            layoutActions->addWidget(newButton);
            addButtonAction(newButton, iniSections[i].itemList[j]->buttonAction, iniSections[i].itemList[j]->args);
            newButton->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Fixed);
        }
        layoutActions->addStretch();
        mainLayout->addLayout(topLayout);
        QScrollArea *scrollAreaActions = new QScrollArea();
        scrollAreaActions->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
        scrollAreaActions->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
        scrollAreaActions->setWidgetResizable(true);
        scrollAreaActions->setWidget(actionButtons);
        mainLayout->addWidget(scrollAreaActions);
        widgets.append(windowActionButtons);
    }
    sections->addStretch();
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(windowSections);
    MainWindow::setCentralWidget(scrollArea);
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
    } else {
        QMessageBox::warning(this,"Ошибка","Действие для кнопки задано неверно! Обратитесь к администратору.",QMessageBox::Ok);
    }
}

void MainWindow::backButtonClick() {
    MainWindow::centralWidget()->setParent(0);
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(windowSections);
    MainWindow::setCentralWidget(scrollArea);
}

void MainWindow::sectionButtonClicked(int index) {
    MainWindow::centralWidget()->setParent(0);
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    scrollArea->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded );
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(widgets[index]);
    MainWindow::setCentralWidget(scrollArea);
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
    } else {
        QString defaultSet = "browserPath = \n";
        FileWriter::write(QCoreApplication::applicationDirPath() + "/settings.ini",defaultSet);
    }
    return false;
}

void MainWindow::setOnStartup()
{
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
    if (!settings.childGroups().contains("ASU-menu", Qt::CaseInsensitive)) {
        QString value = QCoreApplication::applicationFilePath(); //get absolute path of running exe
        QString apostroph = "\"";
        value.replace("/","\\");
        value = apostroph + value + apostroph + " --argument";
        settings.setValue("ASU-menu",value);
    }
}
