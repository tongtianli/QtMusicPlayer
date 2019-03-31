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

private:
    Ui::PlayStateWidget *ui;
    QMediaPlayer *player;
    QMediaPlaylist playlist;
};

#endif // PLAYSTATEWIDGET_H
