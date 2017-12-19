#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    errorLoadingFileLabel = new QLabel(this);
    QSize desktopSize = qApp->desktop()->availableGeometry().size();
    windowSize = QSize(desktopSize.width() * 0.8, desktopSize.height() * 0.9);
    centerAndResize();
    userIniFilename = QDir::currentPath() + "/debug/" + qgetenv("USERNAME") + ".ini";  // TODO: Consider using WinApi (https://stackoverflow.com/questions/26552517/get-system-username-in-qt)
    if (!FileWriter::exists(userIniFilename)) {
        errorLoadingFileMsg();
    } else {
        initialiseInterface();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

inline void MainWindow::centerAndResize() {
    setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            windowSize,
            qApp->desktop()->availableGeometry()
        )
    );
}

inline void MainWindow::errorLoadingFileMsg() {  // TODO: add retry button
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
    MainWindow::setCentralWidget(tabWidget);
    for (int i = 0; i < iniSections.count(); i++) {
        QWidget *newTab = new QWidget(tabWidget);
        QVBoxLayout *layout = new QVBoxLayout;
        for (int j = 0; j < iniSections[i].itemList.count(); j++) {
            QPushButton *newButton = new QPushButton(iniSections[i].itemList[j]->key);
            layout->addWidget(newButton);
            addButtonAction(newButton, iniSections[i].itemList[j]->value);
        }
        newTab->setLayout(layout);
        tabWidget->addTab(newTab, iniSections[i].sectionName);
    }
}

inline void MainWindow::addButtonAction(QPushButton *button, QString action) {
    QSignalMapper* signalMapper = new QSignalMapper(this);
    signalMapper->setMapping(button, action);
    connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(buttonClicked(QString)));
    connect(button, SIGNAL(clicked()), signalMapper, SLOT(map()));
}

void MainWindow::buttonClicked(QString data) {
    QRegExp regExp("^(((http|ftp)(s?)://)|(www.))(([a-zA-Z0-9-.]+(.[a-zA-Z0-9-.]+)+)|localhost)(/?)([a-zA-Z0-9-.?,'/\\+&%$#_])?([\\d\\w./%+-=&amp;?:\\&quot;',|~;])$");
    regExp.indexIn(data);
    QFileInfo info = QFileInfo(data);
    if (regExp.cap(0).length() != 0) {
        QDesktopServices::openUrl(QUrl(data));
    }
    if (info.isExecutable()) {
        QProcess::startDetached('"' + data + '"');
    }
}
