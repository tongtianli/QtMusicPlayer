#ifndef LOCALMUSICWIDGET_H
#define LOCALMUSICWIDGET_H

#include "music.h"
#include "musictablewidget.h"

namespace Ui {
class LocalMusicWidget;
}

class LocalMusicWidget : public QWidget
{
    Q_OBJECT

public:
    MusicTableWidget *table;
    explicit LocalMusicWidget(QWidget *parent = nullptr);
    ~LocalMusicWidget();

signals:
    void addMusic(Music* music);

private slots:
    void on_btnAddMedia_clicked();
    void setTotalMusicLabel(int size);

private:
    Ui::LocalMusicWidget *ui;

};

#endif // LOCALMUSICWIDGET_H
