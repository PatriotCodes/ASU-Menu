#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    centerAndResize();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::centerAndResize() {
    QSize availableSize = qApp->desktop()->availableGeometry().size();
    QSize newSize( availableSize.width() * 0.8, availableSize.height() * 0.9 );
    setGeometry(
        QStyle::alignedRect(
            Qt::LeftToRight,
            Qt::AlignCenter,
            newSize,
            qApp->desktop()->availableGeometry()
        )
    );
}
