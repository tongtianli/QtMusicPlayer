#ifndef LOCALMUSICWIDGET_H
#define LOCALMUSICWIDGET_H

#include "music.h"
#include "musicpool.h"

namespace Ui {
class LocalMusicWidget;
}

namespace LOCALTABLE {
const int COLUMN_NAME = 0;
const int COLUMN_DURATION = 1;
const int COLUMN_SIZE = 2;
}

class LocalMusicWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LocalMusicWidget(QWidget *parent = nullptr);
    ~LocalMusicWidget();

    void refreshTable(QList<Music*> list);
    void load();
    void save();


signals:
    void localWidgetAddMusic(Music *music);
    void localWidgetLoadList(QList<int> idlist);
    void playLocalMusiclist(int index,QList<Music*> playlist);

private slots:
    void on_btnAddMedia_clicked();


    void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

private:
    Ui::LocalMusicWidget *ui;
    void addMusicToTable(Music *music);
    QList<Music*> list;

};

#endif // LOCALMUSICWIDGET_H
