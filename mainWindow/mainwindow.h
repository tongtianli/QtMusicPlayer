#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "music.h"
#include "mainwindowtitle.h"
#include "suggestbox.h"
#include "playlistwidget.h"
#include "playstatewidget.h"
#include "localmusicwidget.h"
#include "usermusicwidget.h"
#include "currentmusicwidget.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void initSliderVolume(int volume);

signals:
    void sliderPlayPositionChanged(int pos);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event)override;

public slots:
    void onPlayerPositionChanged(qint64 position);
    void onPlayerDurationChanged(qint64 duration);
    void onPlayerStateChanged(QMediaPlayer::State);

private slots:
    void resizeSubwidget();
    void on_btnOpenPlaylist_clicked();
    void on_btnPlay_clicked();
    void onChangelistRequested(int index,QList<Music*> list);
    void on_sliderPlay_sliderReleased();
    void on_btnLast_clicked();
    void on_btnNext_clicked();
    void setPlaylistBtnTextbycurSize(int size);
    void aboutToExit();
    void onAddMusic(Music* music);
    void showCurMusicInfo(Music *music);

private:
    void loadData();
    void saveData();
    ResizeRegion resizeRegion;
    const int headHeight = 38;
    const int bottomHeight = 38;

    bool mouseLeftButtonPressed;
    QPoint mouseLastGlobalPos;

    Ui::MainWindow *ui;
    MainwindowTitle *title;
    SuggestBox *suggestBox;
    PlayListWidget *playlistWidget;
    PlayStateWidget *playStateWidget;
    CurrentMusicWidget *currentMusicWidget;
    qint64 duration;
    QString posiText;
    QString duraText;
    QHash<int,Music*> musicpool;

    void connectSlots();

};

#endif // MAINWINDOW_H
