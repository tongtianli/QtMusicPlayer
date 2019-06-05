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
    ~PlayStateWidget() override;

signals:
    void likeMusic(Music *music);
    void dislikeMusic(Music *music);
    void curPictureClicked(Music *music);

public slots:
    void changeCurrentDisplay(Music *music);
    void setPicture(QNetworkReply *reply);

protected:
    bool eventFilter(QObject *ob, QEvent *e) override;

private slots:
    void on_downloadBtn_clicked();
    void on_likeBtn_clicked();

private:
    Ui::PlayStateWidget *ui;
    QNetworkAccessManager manager;
    MusicDownloader downloader;
    QPixmap defaultPix;
    Music *curMusic;
    bool likeCurrentMusic;
};

#endif // PLAYSTATEWIDGET_H
