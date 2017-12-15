#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSize desktopSize = qApp->desktop()->availableGeometry().size();
    windowSize = QSize(desktopSize.width() * 0.8, desktopSize.height() * 0.9);
    centerAndResize();
    userIniFilename = QDir::currentPath() + "/debug/" + qgetenv("USERNAME") + ".ini";  // TODO: Consider using WinApi
    //(https://stackoverflow.com/questions/26552517/get-system-username-in-qt)
    if (!FileWriter::exists(userIniFilename)) {
        errorLoadingFileMsg();
    }
    QList<IniSection> iniSections = IniParser::parse(userIniFilename);
    qDebug();
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

inline void MainWindow::errorLoadingFileMsg() {
    errorLoadingFileLabel = new QLabel(this);
    QFont f("Helvetica", 10, QFont::Bold);
    errorLoadingFileLabel->setFont(f);
    errorLoadingFileLabel->setStyleSheet("QLabel { color : red; }");
    errorLoadingFileLabel->setText("Не удается найти файл с настройками для текущего пользователя системы!");
    errorLoadingFileLabel->setAlignment(Qt::AlignCenter);
    errorLoadingFileLabel->setGeometry(QRect(0,0,windowSize.width(),windowSize.height()));
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    windowSize = QSize(this->width(),this->height());
    if (errorLoadingFileLabel->isEnabled()) {
        errorLoadingFileLabel->setGeometry(QRect(0,0,windowSize.width(),windowSize.height()));
    }
    QMainWindow::resizeEvent(event);
}
