#ifndef MUSICTABLEWIDGET_H
#define MUSICTABLEWIDGET_H

#include "music.h"
#include "musicdownloader.h"

class MusicTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    explicit MusicTableWidget(QWidget *parent = nullptr, QString tableName = "");
    ~MusicTableWidget();
    void setName(QString tableName);
    void insertMusic(int index, Music *music);
    void setMusiclist(QList<Music*> list);
    void clearAll();
    void remove(int index);
    void remove(Music *music);
    void append(Music *music);
    void play();
    void playLater();
    Music* get(int index);
    QList<Music*> getAll();
    void load();
    void downloadAllMusic();


signals:
    void sizeChanged(int size);
    void musicDoubleClicked(QList<Music*> list, int index);
    void playThisListLater(QList<Music*> list);

private:
    QList<Music*> list;
    QSet<QString> musicSet;
    QString name;
    bool preventChangeSignal;
    void save();                                        //当析构时调用


private slots:
    void onTableItemDoubleClicked(QTableWidgetItem *item);
};

#endif // MUSICTABLEWIDGET_H
