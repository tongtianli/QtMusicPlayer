#include "localmusicwidget.h"
#include "ui_localmusicwidget.h"

LocalMusicWidget::LocalMusicWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LocalMusicWidget)
{
    ui->setupUi(this);
    table = ui->tableWidget;
    table->setColumnWidth(0,300);
    table->setColumnWidth(1,100);
    table->setColumnWidth(2,100);
    ui->labLocalMediaNotFound->hide();
    connect(table,&MusicTableWidget::sizeChanged,this,&LocalMusicWidget::setTotalMusicLabel);
}

LocalMusicWidget::~LocalMusicWidget()
{
    table->clear();
    delete ui;
}

void LocalMusicWidget::on_btnAddMedia_clicked()
{
    QString startPath = "E:/音乐/";
    QString dlgTitle = "选择音频文件";
    QString filter = "音频文件(*.mp3 *.wav *.wma);;mp3 文件(*.mp3);;wav 文件(*.wav);;wma 文件(*.wma);;所有文件(*.*)";
    QStringList list = QFileDialog::getOpenFileNames(this,dlgTitle,startPath,filter);
    foreach(QString name,list){
        Music *music = new Music();
        QFileInfo info(name);
        music->ID = -1;
        music->url = QUrl::fromLocalFile(name);
        music->name = info.baseName();
        music->singer = "未知";
        music->duration = "未知";
        music->pic = "";
        music->lrc = "";
        music->local = true;
        music->size = QString::number(info.size()/1000000)+"MB";
        table->append(music);
    }
}

void LocalMusicWidget::setTotalMusicLabel(int size)
{
    ui->labTotalLocalMedia->setText(QString("共%1首").arg(size));
}
