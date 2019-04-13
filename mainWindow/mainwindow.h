#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "filemanager.h"
#include "playlistwidget.h"
#include "playstatewidget.h"
#include "localmusicwidget.h"
#include "music.h"
#include "suggestbox.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
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

public slots:
    void onPlayerPositionChanged(qint64 position);
    void onPlayerDurationChanged(qint64 duration);
    void onPlayerStateChanged(QMediaPlayer::State);

private slots:
    void addMusicToPool(Music* music);
    void initLocalWidget(QList<int> idlist);
    void resizeSubwidget();

    void on_btnOpenPlaylist_clicked();
    void on_btnPlay_clicked();
    void onChangelistRequested(int index,QList<Music*> list);
    void on_sliderPlay_sliderReleased();
    void on_btnLast_clicked();
    void on_btnNext_clicked();


private:
    Ui::MainWindow *ui;
    SuggestBox *suggestBox;
    PlayListWidget *playlistWidget;
    LocalMusicWidget *localMusicWidget;
    PlayStateWidget *playStateWidget;
    MusicPool defaultMusicPool;
    void connectSlots();
    qint64 duration;
    QString posiText;
    QString duraText;



};

#endif // MAINWINDOW_H
