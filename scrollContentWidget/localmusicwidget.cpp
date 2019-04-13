#include "localmusicwidget.h"
#include "ui_localmusicwidget.h"

LocalMusicWidget::LocalMusicWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LocalMusicWidget)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnWidth(0,300);
    ui->tableWidget->setColumnWidth(1,100);
    ui->tableWidget->setColumnWidth(2,100);
    ui->labLocalMediaNotFound->hide();
}

LocalMusicWidget::~LocalMusicWidget()
{
    ui->tableWidget->clear();
    delete ui;
}



void LocalMusicWidget::addMusicToTable(Music *music)
{
    QTableWidget *table = ui->tableWidget;
    int index = table->rowCount();
    table->insertRow(index);
    QTableWidgetItem *item = new QTableWidgetItem(music->name);
    table->setItem(index,LOCALTABLE::COLUMN_NAME,item);
    item = new QTableWidgetItem(music->duration);
    table->setItem(index,LOCALTABLE::COLUMN_DURATION,item);
    item = new QTableWidgetItem(music->size);
    table->setItem(index,LOCALTABLE::COLUMN_SIZE,item);
}

void LocalMusicWidget::refreshTable(QList<Music*> list)
{
    ui->tableWidget->clear();
    foreach(Music* music,list){
        addMusicToTable(music);
    }
    this->list = list;
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
        music->url = QUrl::fromLocalFile(name);
        music->name = info.baseName();
        music->ID = 0;
        music->duration = "未知";
        music->author = "未知";
        music->size = QString::number(info.size()/1000000)+"MB";
        addMusicToTable(music);
        this->list.append(music);
        emit localWidgetAddMusic(music);
    }
}

void LocalMusicWidget::load()
{
    QDir dir = QDir::currentPath()+"/data/defaultMusicPool.xml";
    QFile file(dir.path());
    if(not file.open(QIODevice::ReadOnly|QIODevice::Text)){
        qDebug()<<"can not open"<<dir<<endl;
        return;
    }
    QXmlStreamReader reader(&file);
    QList<int> idlist;
    while(!reader.atEnd()){
        if(reader.name()=="mediaID")
            idlist.append(reader.readElementText().toInt());
        reader.readNext();
        }
    file.close();
    emit localWidgetLoadList(idlist);
}

void LocalMusicWidget::save()
{
    return;
}



void LocalMusicWidget::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    emit playLocalMusiclist(item->row(),list);
}
