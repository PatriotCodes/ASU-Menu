#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "filewriter.h"
#include <QDesktopWidget>
#include <QLabel>
#include <QMainWindow>
#include <QSize>
#include <QWindow>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QSize windowSize;
    QString userIniFilename;
    inline void centerAndResize();
    inline void errorLoadingFileMsg();
};

#endif // MAINWINDOW_H
