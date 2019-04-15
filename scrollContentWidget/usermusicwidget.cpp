#include "usermusicwidget.h"
#include "ui_usermusicwidget.h"

UserMusicWidget::UserMusicWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserMusicWidget)
{
    ui->setupUi(this);
    this->hide();
}

UserMusicWidget::~UserMusicWidget()
{
    delete ui;
}

