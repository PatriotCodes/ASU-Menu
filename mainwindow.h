#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "databasemanager.h"
#include "filewriter.h"
#include "iniparser.h"
#include "inisection.h"
#include "qtpushbuttonstyleproxy.h"
#include <QAction>
#include <QCloseEvent>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QDir>
#include <QEvent>
#include <QRegExp>
#include <QFileInfo>
#include <QGuiApplication>
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
#include <Qt>
#include <QTabWidget>
#include <QTextEdit>
#include <QUrl>
#include <QVBoxLayout>
#include <QWindow>
#include <QWidget>

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
    void sectionButtonClicked(int);
    void backButtonClick();

private:
    Ui::MainWindow *ui;
    QLabel *errorLoadingFileLabel;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QAction *restoreAction;
    QAction *quitAction;
    QSize windowSize;
    QString userIniFilename;
    QString browserPath;
    QWidget *windowSections;
    QList<QWidget*> widgets;
    bool firstHide;
    void alignAndResize();
    void errorLoadingFileMsg();
    void initialiseInterface();
    void addButtonAction(QPushButton*,QString,QString);
    void addSectionButtonAction(QPushButton*,int);
    void closeEvent(QCloseEvent*);
    void changeEvent(QEvent* e);
    void createTrayIcon();
    void createTrayActions();
    void showMessageTray();
    void updateActions();
    bool loadSettings(QString);
};

#endif // MAINWINDOW_H
