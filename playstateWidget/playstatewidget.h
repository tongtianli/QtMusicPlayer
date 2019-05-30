#ifndef PLAYSTATEWIDGET_H
#define PLAYSTATEWIDGET_H

#include "music.h"
#include "musicdownloader.h"

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

private slots:
    void on_downloadBtn_clicked();

private:
    Ui::PlayStateWidget *ui;
    QNetworkAccessManager manager;
    MusicDownloader downloader;
    QPixmap defaultPix;
    Music *curMusic;
};

#endif // PLAYSTATEWIDGET_H
