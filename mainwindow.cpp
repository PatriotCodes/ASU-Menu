#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSize currentDesktopSize = qApp->desktop()->availableGeometry().size();
    windowSize = QSize(currentDesktopSize.width() * 0.8, currentDesktopSize.height() * 0.9);
    centerAndResize();
    userIniFilename = qgetenv("USERNAME");  // TODO: Consider using WinApi
    //(https://stackoverflow.com/questions/26552517/get-system-username-in-qt)
    if (!FileWriter::exists(userIniFilename)) {
        errorLoadingFileMsg();
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

void MainWindow::errorLoadingFileMsg() {
    QLabel *label = new QLabel(this);
    QFont f("Helvetica", 10, QFont::Bold);
    label->setFont(f);
    label->setStyleSheet("QLabel { color : red; }");
    label->setText("Не удается найти файл с настройками для текущего пользователя системы!");
    label->setAlignment(Qt::AlignCenter);
    label->setGeometry(QRect(0,0,windowSize.width(),windowSize.height()));
}
