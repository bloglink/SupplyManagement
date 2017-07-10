#ifndef ROLEMANAGERMENT_H
#define ROLEMANAGERMENT_H

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

#define ROLE_ID 0
#define ROLE_NAME 1
#define ROLE_BACKUP 2

class RoleManagerment : public QWidget
{
    Q_OBJECT
public:
    explicit RoleManagerment(QWidget *parent = 0);

signals:
    void sendSocket(QUrl url);
private slots:
    void initUI();
    void initData();
    void addRole();
    void delRole();
    void sndRole();
    void refresh();
    void recvSocket(QUrl url);
    virtual void showEvent(QShowEvent *e);
private:
    StandardItemModel *m_roles;
    QTableView *tab_roles;
    QJsonArray json_roles;
    int max_role_id;
};

#endif // ROLEMANAGERMENT_H
