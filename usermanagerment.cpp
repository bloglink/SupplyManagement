#include "usermanagerment.h"

UserManagerment::UserManagerment(QWidget *parent) : QWidget(parent)
{
    initUI();
    max_user_id = 0;
}

void UserManagerment::initUI()
{
    this->setObjectName("usermanagerment");
    tab_users  = new QTableView(this);

    QPushButton *user_add = new QPushButton(this);
    QPushButton *user_del = new QPushButton(this);
    QPushButton *user_snd = new QPushButton(this);

    user_add->setFlat(true);
    user_add->setMinimumSize(97,44);
    user_add->setText(tr("添加用户"));
    user_add->setFocusPolicy(Qt::NoFocus);
    connect(user_add,SIGNAL(clicked(bool)),this,SLOT(addUser()));

    user_del->setFlat(true);
    user_del->setMinimumSize(97,44);
    user_del->setText(tr("删除用户"));
    user_del->setFocusPolicy(Qt::NoFocus);
    connect(user_del,SIGNAL(clicked(bool)),this,SLOT(delUser()));

    user_snd->setFlat(true);
    user_snd->setMinimumSize(97,44);
    user_snd->setText(tr("保存修改"));
    user_snd->setFocusPolicy(Qt::NoFocus);
    connect(user_snd,SIGNAL(clicked(bool)),this,SLOT(sndUser()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(user_add);
    btnLayout->addWidget(user_del);
    btnLayout->addWidget(user_snd);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(tab_users);
    layout->addLayout(btnLayout);

    QGroupBox *group = new QGroupBox(this);
    group->setLayout(layout);
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(group);
    this->setLayout(mainLayout);

    m_users = new StandardItemModel(0, 5);
    QStringList users_header;
    users_header << tr("ID") << tr("角色") << tr("姓名") << tr("密码")
                 << tr("手机") << tr("邮箱");
    m_users->setHorizontalHeaderLabels(users_header);
    tab_users->setModel(m_users);
    tab_users->setColumnWidth(USER_ID,80);
    tab_users->horizontalHeader()->setSectionResizeMode(1,QHeaderView::Stretch);
    tab_users->horizontalHeader()->setSectionResizeMode(2,QHeaderView::Stretch);
    tab_users->horizontalHeader()->setSectionResizeMode(3,QHeaderView::Stretch);
    tab_users->horizontalHeader()->setSectionResizeMode(4,QHeaderView::Stretch);
    tab_users->horizontalHeader()->setSectionResizeMode(5,QHeaderView::Stretch);
    tab_users->horizontalHeader()->setHighlightSections(false);
    tab_users->setItemDelegateForColumn(USER_ID, new ReadOnlyDelegate);
    QStringList roles_header;
    roles_header << tr("商务") << tr("计划") << tr("内勤") << tr("销售");
    delegate = new ComboBoxDelegate(this);
    delegate->setItemHeaders(roles_header);
    tab_users->setItemDelegateForColumn(USER_ROLE, delegate);
    tab_users->setSelectionBehavior(QAbstractItemView::SelectRows);
}

void UserManagerment::initData()
{
    max_user_id = 0;
    for (int i=0; i < json_users.size(); i++) {
        QJsonObject obj = json_users.at(i).toObject();
        QString name = obj.value("erp_username").toString();
        QString password =  obj.value("password").toString();
        QString userrole = obj.value("erp_rolename").toString();
        int id   = obj.value("erp_userid").toInt();
        if (id > max_user_id)
            max_user_id = id;
        m_users->setItem(i,USER_ID,new QStandardItem(QString::number(id)));
        m_users->setItem(i,USER_ROLE,new QStandardItem(userrole));
        m_users->setItem(i,USER_NAME,new QStandardItem(name));
        m_users->setItem(i,USER_PASSWORD,new QStandardItem(password));
    }
}

void UserManagerment::addUser()
{
    m_users->appendRow(NULL);
    int i = m_users->rowCount()-1;
    max_user_id = max_user_id + 1;
    m_users->setItem(i,USER_ID,new QStandardItem(QString::number(max_user_id)));
    m_users->setItem(i,USER_ROLE,new QStandardItem(""));
    m_users->setItem(i,USER_NAME,new QStandardItem(tr("新增")));
    m_users->setItem(i,USER_PASSWORD,new QStandardItem("123456"));
}

void UserManagerment::delUser()
{
    int row = tab_users->currentIndex().row();
    m_users->removeRow(row);
}

void UserManagerment::sndUser()
{
    QStringList tab_names;
    QStringList svr_names;
    QStringList t_password;
    QStringList s_password;
    QStringList tab_roles;
    QStringList svr_roles;
    QList<int> tab_ids;
    QList<int> svr_ids;

    QJsonArray array_user_add;
    QJsonArray array_user_del;
    QJsonArray array_user_upt;
    QJsonObject obj_user_add;
    QJsonObject obj_user_del;
    QJsonObject obj_user_upt;
    QJsonArray user_array;

    for (int i=0; i < m_users->rowCount(); i++) {
        tab_ids.append(m_users->item(i,USER_ID)->text().toInt());
        tab_names.append(m_users->item(i,USER_NAME)->text());
        t_password.append(m_users->item(i,USER_PASSWORD)->text());
        tab_roles.append(m_users->item(i,USER_ROLE)->text());
    }
    for (int i=0; i < json_users.size(); i++) {
        QJsonObject obj = json_users.at(i).toObject();
        svr_ids.append(obj.value("erp_userid").toInt());
        svr_names.append(obj.value("erp_username").toString());
        s_password.append(obj.value("password").toString());
        svr_roles.append(obj.value("erp_rolename").toString());
    }
    for (int i=0; i < tab_ids.size(); i++) {
        int row = svr_ids.indexOf(tab_ids.at(i));
        if (row == -1) { //增加了一个ID
            QJsonObject obj;
            obj.insert("erp_userid",tab_ids.at(i));
            obj.insert("erp_username",tab_names.at(i));
            obj.insert("password",t_password.at(i));
            obj.insert("erp_rolename",tab_roles.at(i));
            array_user_add.append(obj);
            json_users.append(obj);
        } else if (tab_names.at(i) != svr_names.at(row) ||
                   t_password.at(i) != s_password.at(i) ||
                   tab_roles.at(i) != svr_roles.at(i)) { //修改了名称密码角色
            QJsonObject obj;
            obj.insert("erp_userid",tab_ids.at(i));
            obj.insert("erp_username",tab_names.at(i));
            obj.insert("password",t_password.at(i));
            obj.insert("erp_rolename",tab_roles.at(i));
            array_user_upt.append(obj);
            json_users[row] = obj;
        }
    }
    for (int i=svr_ids.size()-1; i >= 0; i--) {
        int row = tab_ids.indexOf(svr_ids.at(i));
        if (row == -1) { //删除了一个ID
            array_user_del.append(json_users[i]);
            json_users.removeAt(i);
        }
    }

    obj_user_add.insert("add", array_user_add);
    obj_user_del.insert("del", array_user_del);
    obj_user_upt.insert("upt", array_user_upt);
    user_array.append(obj_user_add);
    user_array.append(obj_user_del);
    user_array.append(obj_user_upt);
    QByteArray user_byte = QJsonDocument(user_array).toJson();
    QUrl url_user;
    url_user.setQuery("user");
    url_user.setFragment(user_byte.toBase64());
    emit sendSocket(url_user);
}

void UserManagerment::recvSocket(QUrl url)
{
    QString cmd = url.query();
    QString usr = url.userName();
    if (usr != "usermanagerment")
        return;
    QByteArray byte = QByteArray::fromBase64(url.fragment().toUtf8());
    if (cmd == "userinfo") {
        json_users = QJsonDocument::fromJson(byte).array();
        initData();
    } else if (cmd == "roleinfo") {
        QJsonArray json_roles = QJsonDocument::fromJson(byte).array();
        QStringList role_headers;
        for (int i=0; i < json_roles.size(); i++) {
            QJsonObject obj = json_roles.at(i).toObject();
            role_headers.append(obj.value("erp_rolename").toString());
        }
        delegate->setItemHeaders(role_headers);
    } else {
        qDebug() << "recv others" << url.toString();
    }
}

void UserManagerment::showEvent(QShowEvent *e)
{
    QUrl url;
    url.setQuery("userinfo");
    sendSocket(url);

    url.setQuery("roleinfo");
    sendSocket(url);
    e->accept();
}

