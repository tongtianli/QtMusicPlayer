#include "addnewmusiclist.h"
#include "ui_addnewmusiclist.h"

AddNewMusiclist::AddNewMusiclist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddNewMusiclist)
{
    ui->setupUi(this);
    setWindowFlag(Qt::WindowType::FramelessWindowHint);
    this->hide();
    setFocusProxy(ui->lineEdit);
    ui->pushButton->setEnabled(false);
}

AddNewMusiclist::~AddNewMusiclist()
{
    delete ui;
}
void AddNewMusiclist::on_lineEdit_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
        ui->pushButton->setEnabled(false);
    else {
        ui->pushButton->setEnabled(true);
    }
}

void AddNewMusiclist::on_lineEdit_returnPressed()
{
    on_pushButton_clicked();
}

void AddNewMusiclist::on_pushButton_clicked()
{
    emit addlist(ui->lineEdit->text());
    ui->lineEdit->clear();
    ui->pushButton->setEnabled(false);
    this->hide();
}

void AddNewMusiclist::on_pushButton_2_clicked()
{
    this->hide();
}
