#include "rolemanagerment.h"

RoleManagerment::RoleManagerment(QWidget *parent) : QWidget(parent)
{
    initUI();
    max_role_id = 0;
}

void RoleManagerment::initUI()
{
    this->setObjectName("rolemanagerment");
    tab_roles = new QTableView(this);

    QPushButton *role_add = new QPushButton(this);
    QPushButton *role_del = new QPushButton(this);
    QPushButton *role_snd = new QPushButton(this);

    role_add->setFlat(true);
    role_add->setMinimumSize(97,44);
    role_add->setText(tr("添加角色"));
    role_add->setFocusPolicy(Qt::NoFocus);
    connect(role_add,SIGNAL(clicked(bool)),this,SLOT(addRole()));

    role_del->setFlat(true);
    role_del->setMinimumSize(97,44);
    role_del->setText(tr("删除角色"));
    role_del->setFocusPolicy(Qt::NoFocus);
    connect(role_del,SIGNAL(clicked(bool)),this,SLOT(delRole()));

    role_snd->setFlat(true);
    role_snd->setMinimumSize(97,44);
    role_snd->setText(tr("保存修改"));
    role_snd->setFocusPolicy(Qt::NoFocus);
    connect(role_snd,SIGNAL(clicked(bool)),this,SLOT(sndRole()));

    QHBoxLayout *btnLayout = new QHBoxLayout;
    btnLayout->addStretch();
    btnLayout->addWidget(role_add);
    btnLayout->addWidget(role_del);
    btnLayout->addWidget(role_snd);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(tab_roles);
    layout->addLayout(btnLayout);

    QGroupBox *group = new QGroupBox(this);
    group->setLayout(layout);
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(group);
    this->setLayout(mainLayout);

    m_roles = new StandardItemModel(0, 2);
    QStringList roles_header;
    roles_header << tr("ID") << tr("角色") << tr("备注");
    m_roles->setHorizontalHeaderLabels(roles_header);
    tab_roles->setModel(m_roles);
    tab_roles->setColumnWidth(ROLE_ID,50);
    tab_roles->setColumnWidth(ROLE_NAME,200);
    tab_roles->horizontalHeader()->setSectionResizeMode(ROLE_BACKUP,QHeaderView::Stretch);
    tab_roles->horizontalHeader()->setHighlightSections(false);
    tab_roles->setItemDelegateForColumn(ROLE_ID, new ReadOnlyDelegate);
}

void RoleManagerment::initData()
{
    max_role_id = 0;
    for (int i=0; i < json_roles.size(); i++) {
        QJsonObject obj = json_roles.at(i).toObject();
        QString role = obj.value("erp_rolename").toString();
        int id   = obj.value("erp_roleid").toInt();
        if (id > max_role_id)
            max_role_id = id;
        m_roles->setItem(i, ROLE_ID, new QStandardItem(QString::number(id)));
        m_roles->setItem(i, ROLE_NAME, new QStandardItem(role));
        m_roles->setItem(i, ROLE_BACKUP, new QStandardItem(""));
    }
}

void RoleManagerment::addRole()
{
    int i = m_roles->rowCount();
    max_role_id = max_role_id + 1;
    m_roles->appendRow(NULL);
    m_roles->setItem(i, ROLE_ID, new QStandardItem(QString::number(max_role_id)));
    m_roles->setItem(i, ROLE_NAME, new QStandardItem(tr("新增")));
    m_roles->setItem(i, ROLE_BACKUP, new QStandardItem(""));
}

void RoleManagerment::delRole()
{
    int row = tab_roles->currentIndex().row();
    m_roles->removeRow(row);
}

void RoleManagerment::sndRole()
{
    QStringList tab_names;
    QStringList svr_names;
    QList<int> tab_ids;
    QList<int> svr_ids;

    QJsonArray array_role_add;
    QJsonArray array_role_del;
    QJsonArray array_role_upt;
    QJsonObject obj_role_add;
    QJsonObject obj_role_del;
    QJsonObject obj_role_upt;
    QJsonArray role_array;

    for (int i=0; i < m_roles->rowCount(); i++) {
        tab_names.append(m_roles->item(i,ROLE_NAME)->text());
        tab_ids.append(m_roles->item(i,ROLE_ID)->text().toInt());
    }
    for (int i=0; i < json_roles.size(); i++) {
        QJsonObject obj = json_roles.at(i).toObject();
        svr_ids.append(obj.value("erp_roleid").toInt());
        svr_names.append(obj.value("erp_rolename").toString());
    }
    for (int i=0; i < tab_ids.size(); i++) {
        int row = svr_ids.indexOf(tab_ids.at(i));
        if (row == -1) { //增加了一个ID
            QJsonObject obj;
            obj.insert("erp_roleid",tab_ids.at(i));
            obj.insert("erp_rolename",tab_names.at(i));
            array_role_add.append(obj);
            json_roles.append(obj);
        } else if (tab_names.at(i) != svr_names.at(row)) { //修改了名称
            QJsonObject obj;
            obj.insert("erp_roleid",tab_ids.at(i));
            obj.insert("erp_rolename",tab_names.at(i));
            array_role_upt.append(obj);
            json_roles[row] = obj;
        }
    }
    for (int i=svr_ids.size()-1; i >= 0; i--) {
        int row = tab_ids.indexOf(svr_ids.at(i));
        if (row == -1) { //删除了一个ID
            array_role_del.append(json_roles[i]);
            json_roles.removeAt(i);
        }
    }

    obj_role_add.insert("add", array_role_add);
    obj_role_del.insert("del", array_role_del);
    obj_role_upt.insert("upt", array_role_upt);
    role_array.append(obj_role_add);
    role_array.append(obj_role_del);
    role_array.append(obj_role_upt);
    QByteArray role_byte = QJsonDocument(role_array).toJson();
    QUrl url;
    url.setQuery("role");
    url.setFragment(role_byte.toBase64());
    emit sendSocket(url);

    refresh();
}

void RoleManagerment::refresh()
{
    QUrl url;
    url.setQuery("roleinfo");
    emit sendSocket(url);
}

void RoleManagerment::recvSocket(QUrl url)
{
    QString cmd = url.query();
    QString usr = url.userName();
    if (usr != "rolemanagerment")
        return;
    QByteArray byte = QByteArray::fromBase64(url.fragment().toUtf8());
    if (cmd == "roleinfo") {
        json_roles = QJsonDocument::fromJson(byte).array();
        initData();
    } else {
        qDebug() << "recv others" << url.toString();
    }
}

void RoleManagerment::showEvent(QShowEvent *e)
{
    refresh();
    e->accept();
}

