#ifndef USERMANAGERMENT_H
#define USERMANAGERMENT_H

#include <QDebug>
#include <QLayout>
#include <QWidget>
#include <QGroupBox>
#include <QTableView>
#include <QShowEvent>
#include <QHeaderView>
#include <QPushButton>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "delegate.h"

#define USER_ID 0
#define USER_ROLE 1
#define USER_NAME 2
#define USER_PASSWORD 3

class UserManagerment : public QWidget
{
    Q_OBJECT
public:
    explicit UserManagerment(QWidget *parent = 0);

signals:
    void sendSocket(QUrl url);
private:
    StandardItemModel *m_users;
    ComboBoxDelegate *delegate;
    QTableView *tab_users ;
    QJsonArray json_users;
    int max_user_id;
private slots:
    void initUI();
    void initData();
    void addUser();
    void delUser();
    void sndUser();
    void recvSocket(QUrl url);
    virtual void showEvent(QShowEvent *e);
};

#endif // USERMANAGERMENT_H
