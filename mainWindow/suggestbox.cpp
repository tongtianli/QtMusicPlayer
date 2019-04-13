#include "suggestbox.h"
#include "ui_suggestbox.h"

SuggestBox::SuggestBox(QWidget *parent, QLineEdit *lineEdit) :
    QWidget(parent),
    ui(new Ui::SuggestBox)
{
    ui->setupUi(this);
    editor = lineEdit;
    suggestList = ui->suggestList;
    x = 200;
    y = 30;
    this->setGeometry(x,y,this->width(),this->height());
    this->setFocusProxy(lineEdit);
    suggestList->setFrameStyle(QFrame::Box | QFrame::Plain);
    timer.setSingleShot(true);
    timer.setInterval(500);
    connectSlots();
    this->hide();
}

SuggestBox::~SuggestBox()
{
    delete ui;
}

void SuggestBox::connectSlots()
{
    connect(suggestList, SIGNAL(itemClicked(QListWidgetItem*)),
            SLOT(doneSelection()));
    connect(&timer, SIGNAL(timeout()), SLOT(autoSuggest()));
    connect(editor, SIGNAL(textEdited(QString)), &timer, SLOT(start()));
    connect(&networkManager, SIGNAL(finished(QNetworkReply*)),
            this, SLOT(handleNetworkData(QNetworkReply*)));
}


void SuggestBox::showResult(const QVector<QString> &choices)
{
    if (choices.isEmpty())
        return;

//    const QPalette &pal = editor->palette();
//    QColor color = pal.color(QPalette::Disabled, QPalette::WindowText);

//    suggestList->setUpdatesEnabled(false);
    suggestList->clear();

    for (const auto &choice : choices) {
        auto item  = new QListWidgetItem();
        item->setText(choice);
//        item->setTextColor(color);
        ui->suggestList->addItem(item);
    }

    suggestList->setCurrentRow(0);
//    suggestList->setUpdatesEnabled(true);

//    suggestList->move(editor->mapToGlobal(QPoint(0, editor->height())));
    suggestList->setFocus();
    this->show();
}

void SuggestBox::doneSelection()
{
    timer.stop();
    suggestList->hide();
    editor->setFocus();
    QListWidgetItem *item = suggestList->currentItem();
    int curIndex = suggestList->currentRow();
    qDebug()<<"item = "<<item;
    qDebug()<<"curIndex="<<curIndex;
    if (item) {
        editor->setText(item->text());
        QJsonObject object = arrayTemp.at(curIndex).toObject();
        QUrl url = QUrl(object["url"].toString());
        QString name = object["name"].toString();
        QString singer = object["singer"].toString();
        int id = object["id"].toInt();
        QUrl pic = QUrl(object["pic"].toString());
        QUrl lrc = QUrl(object["lrc"].toString());
        int durInt = object["time"].toInt();
        int min = durInt/60;
        int sec = durInt%60;
        Music *music = new Music();
        music->ID = id;
        music->url = url;
        music->name = name;
        music->singer = singer;
        music->duration = QString("%1:%2").
                arg(min,2,10,QLatin1Char('0')).arg(sec,2,10,QLatin1Char('0'));
        music->lrc = lrc;
        music->pic = pic;
        music->local = false;
        emit selectSearchedSong(music);
    }
}

void SuggestBox::autoSuggest()
{
    QString str = editor->text();
    QString url = searchUrl.arg(str);
    qDebug()<<url;
    networkManager.get(QNetworkRequest(url));
}

void SuggestBox::preventSuggest()
{
    timer.stop();
}

void SuggestBox::handleNetworkData(QNetworkReply *networkReply)
{
    if (networkReply->error() != QNetworkReply::NoError) {
        qDebug()<<"NetworkReply error!";
        return;
    }
    QVector<QString> choices;
    QByteArray response(networkReply->readAll());
    QJsonParseError jsonError;
    QJsonDocument parseDocument = QJsonDocument::fromJson(response,&jsonError);
    if(jsonError.error!=QJsonParseError::NoError){
        qDebug()<<"Json parse error!";
    }
    QJsonObject object = parseDocument.object();
    if(object["result"].toString()=="ERROR")
    {
        qDebug()<<"什么都没有找到";
        return;
    }
    QJsonArray array = object["data"].toArray();
    for(int i=0;i<array.size();i++){
        QString songname = array[i].toObject()["name"].toString();
        QString singer = array[i].toObject()["singer"].toString();
        choices.append(songname+" - "+singer);
    }
    showResult(choices);
    arrayTemp = array;
}
