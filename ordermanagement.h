#ifndef ORDERMANAGEMENT_H
#define ORDERMANAGEMENT_H

#include <QDate>
#include <QDebug>
#include <QLabel>
#include <QLayout>
#include <QWidget>
#include <QGroupBox>
#include <QDateEdit>
#include <QTableView>
#include <QShowEvent>
#include <QHeaderView>
#include <QPushButton>
#include <QToolButton>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include "delegate.h"

#define ORDER_ODDS 0
#define ORDER_DATE 1
#define ORDER_SALE 2
#define ORDER_CSTM 3
#define ORDER_QUAN 4
#define ORDER_VIEW 5
#define ORDER_EDAT 6
#define ORDER_STTS 7

class OrderManagement : public QWidget
{
    Q_OBJECT
public:
    explicit OrderManagement(QWidget *parent = 0);

signals:
    void sendSocket(QUrl url);
private slots:
    void initUI();
    void initData();
    void addOrder();
    void delOrder();
    void uptOrder();
    void sndOrder();
    void tabSync(QModelIndex index);
    void recvSocket(QUrl url);
    void showAdd();
    void showSnd();
    void refresh();
    virtual void showEvent(QShowEvent *e);
private:
    StandardItemModel *m_show;
    QTableView *tab_show;
    QJsonArray json_show;

    StandardItemModel *m_add;
    QTableView *tab_add;
    QToolButton *btn_add;
    QWidget *w_add;
    QJsonArray json_sale;
    QJsonArray json_customer;
    QDateEdit *s_date;
    QDateEdit *e_date;
    ComboBoxDelegate *sales;
    ComboBoxDelegate *customer;

    StandardItemModel *m_snd;
    QTableView *tab_snd;
    QToolButton *btn_snd;
    QWidget *w_snd;

    int max_order_id;
    QComboBox *status;
};

#endif // ORDERMANAGEMENT_H
