#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "filewriter.h"
#include "iniparser.h"
#include "inisection.h"
#include <QPushButton>
#include <QDesktopWidget>
#include <QDir>
#include <QLabel>
#include <QList>
#include <QMainWindow>
#include <QSize>
#include <QString>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QWindow>

#include <QDebug>
#include <QSettings>

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
    QLabel *errorLoadingFileLabel;
    QSize windowSize;
    QString userIniFilename;
    inline void centerAndResize();
    inline void errorLoadingFileMsg();
    void resizeEvent(QResizeEvent*) override;
    void initialiseInterface();
};

#endif // MAINWINDOW_H
