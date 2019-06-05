#ifndef CURRENTMUSICWIDGET_H
#define CURRENTMUSICWIDGET_H

#include "music.h"

namespace Ui {
class CurrentMusicWidget;
}

class CurrentMusicWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CurrentMusicWidget(QWidget *parent = nullptr);
    ~CurrentMusicWidget();
    void requestPicture(QUrl picUrl);
    void requestLrc(QUrl lrcUrl);

private slots:
    void setPicture(QNetworkReply *reply);
    void setLrc(QNetworkReply *reply);

private:
    Ui::CurrentMusicWidget *ui;
    QNetworkAccessManager pic;
    QNetworkAccessManager lrc;
};

#endif // CURRENTMUSICWIDGET_H
