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

inline void MainWindow::errorLoadingFileMsg() {
    errorLoadingFMsg = new QLabel(this);
    QFont f("Helvetica", 10, QFont::Bold);
    errorLoadingFMsg->setFont(f);
    errorLoadingFMsg->setStyleSheet("QLabel { color : red; }");
    errorLoadingFMsg->setText("Не удается найти файл с настройками для текущего пользователя системы!");
    errorLoadingFMsg->setAlignment(Qt::AlignCenter);
    errorLoadingFMsg->setGeometry(QRect(0,0,windowSize.width(),windowSize.height()));
}

void MainWindow::resizeEvent(QResizeEvent *event) {
    windowSize = QSize(this->width(),this->height());
    errorLoadingFMsg->setGeometry(QRect(0,0,windowSize.width(),windowSize.height()));
    QMainWindow::resizeEvent(event);
}
