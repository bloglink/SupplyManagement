#include "mainscreen.h"

MainScreen::MainScreen(QWidget *parent)
    : QMainWindow(parent)
{
    initUI();
}

MainScreen::~MainScreen()
{

}

int MainScreen::login()
{
    LoginScreen w;
    connect(&w,SIGNAL(sendSocket(QUrl)),this,SIGNAL(sendSocket(QUrl)));
    connect(this,SIGNAL(sendMsg(QUrl)),&w,SLOT(recvSocket(QUrl)));
    return w.exec();
}

void MainScreen::initUI()
{
    this->setWindowFlags(Qt::FramelessWindowHint);

    //设置界面风格
    QFile file;
    QString qss;
    file.setFileName(":/skins/gn_bu.css");
    file.open(QFile::ReadOnly);
    qss = QLatin1String(file.readAll());
    qApp->setStyleSheet(qss);

    QApplication::setOrganizationName("AIP");
    QApplication::setApplicationName("QFramer");
    QApplication::setWindowIcon(QIcon(":/icons/link.ico"));
    QSystemTrayIcon *trayicon = new QSystemTrayIcon(QIcon(QString(":/icons/link.ico")), this);
    trayicon->setToolTip(QString(qApp->applicationName()));
    trayicon->show();

}
