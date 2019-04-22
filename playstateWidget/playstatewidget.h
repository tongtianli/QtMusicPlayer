#ifndef PLAYSTATEWIDGET_H
#define PLAYSTATEWIDGET_H

#include "music.h"

namespace Ui {
class PlayStateWidget;
}

class PlayStateWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlayStateWidget(QWidget *parent = nullptr);
    ~PlayStateWidget();

public slots:
    void changeCurrentDisplay(Music *music);
    void setPicture(QNetworkReply *reply);

private:
    Ui::PlayStateWidget *ui;
    QNetworkAccessManager manager;
    QPixmap defaultPix;
};

#endif // PLAYSTATEWIDGET_H
