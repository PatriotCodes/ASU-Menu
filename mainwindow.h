#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "filewriter.h"
#include "iniparser.h"
#include "inisection.h"
#include <QAction>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QDir>
#include <QRegExp>
#include <QFileInfo>
#include <QLabel>
#include <QList>
#include <QMainWindow>
#include <QMenu>
#include <QProcess>
#include <QPushButton>
#include <QSignalMapper>
#include <QSize>
#include <QSystemTrayIcon>
#include <QString>
#include <QTabWidget>
#include <QTextEdit>
#include <QUrl>
#include <QVBoxLayout>
#include <QWindow>

#include <QDebug>
#include <QSettings>
#include <QMessageBox>

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
    void trayIconClicked(QSystemTrayIcon::ActivationReason);

private:
    Ui::MainWindow *ui;
    QLabel *errorLoadingFileLabel;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QAction *restoreAction;
    QAction *quitAction;
    QSize windowSize;
    QString userIniFilename;
    bool firstHide;
    inline void centerAndResize();
    inline void errorLoadingFileMsg();
    void initialiseInterface();
    inline void addButtonAction(QPushButton*,QString);
    void closeEvent(QCloseEvent*);
    inline void createTrayIcon();
    inline void createTrayActions();
    void showMessageTray();
    QString getDefaultBrowser();
};

#endif // MAINWINDOW_H
