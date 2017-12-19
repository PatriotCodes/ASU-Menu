#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "filewriter.h"
#include "iniparser.h"
#include "inisection.h"
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QDir>
#include <QRegExp>
#include <QFileInfo>
#include <QLabel>
#include <QList>
#include <QMainWindow>
#include <QProcess>
#include <QPushButton>
#include <QSignalMapper>
#include <QSize>
#include <QString>
#include <QTabWidget>
#include <QUrl>
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

public slots:
    void buttonClicked(QString);

private:
    Ui::MainWindow *ui;
    QLabel *errorLoadingFileLabel;
    QSize windowSize;
    QString userIniFilename;
    inline void centerAndResize();
    inline void errorLoadingFileMsg();
    void initialiseInterface();
    inline void addButtonAction(QPushButton*,QString);
};

#endif // MAINWINDOW_H
