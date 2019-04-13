#include "musictablewidget.h"
#include "ui_musictablewidget.h"

MusicTableWidget::MusicTableWidget(QWidget *parent, QString tableName) :
    QWidget(parent),
    ui(new Ui::MusicTableWidget)
{
    ui->setupUi(this);
    table = ui->tableWidget;
    manager = new FileManager(tableName);
    name = tableName;
}

MusicTableWidget::~MusicTableWidget()
{
    delete manager;
    delete this;
    delete ui;
}
