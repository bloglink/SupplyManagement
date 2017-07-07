#include "aboutusscreen.h"

AboutUsScreen::AboutUsScreen(QWidget *parent) : QWidget(parent)
{
    initUI();
}

void AboutUsScreen::initUI()
{
    this->setObjectName("aboutusscreen");
    QString style = ".QFrame{border-image:url(:/images/bear.jpg)}";
    style += "QLabel{font:bold 14pt;color:rgb(0,82,112)}";
    this->setStyleSheet(style);

    QLabel *label_logo = new QLabel(tr("青岛艾普供应链管理系统"));
    QLabel *label_info = new QLabel(tr("Faster Easier Stronger Prettier"));
    QLabel *label_author = new QLabel(tr("Author:Zhaonanlin Zhanghongjian"));
    QLabel *label_version = new QLabel(tr("Version:0.0.0.1"), this);
    QLabel *label_lincence = new QLabel(tr("Lincence: LGPL v3"), this);
    QLabel *label_copyright = new QLabel(tr("Copyright 2017-2018 AIP. All rights reserved"), this);

    QVBoxLayout *text_layout = new QVBoxLayout;
    text_layout->addStretch();
    text_layout->addWidget(label_logo);
    text_layout->addWidget(label_info);
    text_layout->addWidget(label_author);
    text_layout->addWidget(label_version);
    text_layout->addWidget(label_lincence);
    text_layout->addWidget(label_copyright);
    text_layout->addStretch();
    text_layout->setSpacing(20);

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addStretch();
    layout->addLayout(text_layout);

    QFrame *about = new QFrame(this);
    about->setLayout(layout);

    QVBoxLayout* main_layout = new QVBoxLayout;
    main_layout->addWidget(about);
    main_layout->setMargin(0);

    this->setLayout(main_layout);
}

