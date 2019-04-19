#include "usermusicwidget.h"
#include "ui_usermusicwidget.h"

UserMusicWidget::UserMusicWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserMusicWidget)
{
    ui->setupUi(this);
    this->hide();
    table = ui->tableWidget;
    table->setColumnWidth(0,300);
    table->setColumnWidth(1,100);
    table->setColumnWidth(2,100);
}

UserMusicWidget::~UserMusicWidget()
{
    delete ui;
}

void UserMusicWidget::setTableName(QString name)
{
    table->setName(name);
}

void UserMusicWidget::load()
{
    table->load();
}

