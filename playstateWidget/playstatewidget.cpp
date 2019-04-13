#include "playstatewidget.h"
#include "ui_playstatewidget.h"

PlayStateWidget::PlayStateWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayStateWidget)
{
    ui->setupUi(this);
    this->hide();
}

PlayStateWidget::~PlayStateWidget()
{
    delete ui;
}
