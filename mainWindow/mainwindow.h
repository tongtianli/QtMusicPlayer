#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "filemanager.h"
#include "playlistwidget.h"
#include "playstatewidget.h"
#include "localmusicwidget.h"
#include "music.h"
#include "player.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void sliderPlayPositionChanged(int pos);

private slots:
    void on_btnOpenPlaylist_clicked();
    void on_btnPlay_clicked();
    void addMusicToPool(Music* music);
    void initLocalWidget(QList<int> idlist);
    void onPositionChanged(int pos,QString posiText);
    void onMaxPositionChanged(QString duraText);

    void on_sliderPlay_sliderReleased();

private:
    Ui::MainWindow *ui;
    PlayListWidget *musicListWidget;
    LocalMusicWidget *localMusicWidget;
    PlayStateWidget *playStateWidget;
    MusicPool defaultMusicPool;
    Player *player;
    void connectSlots();
    void allocateNewID(Music *music);
    void refreshLabelPlayState();
    QString posiText;
    QString duraText;
};

#endif // MAINWINDOW_H
