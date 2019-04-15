#ifndef MUSICTABLEWIDGET_H
#define MUSICTABLEWIDGET_H

#include "music.h"

class MusicTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    explicit MusicTableWidget(QWidget *parent = nullptr, QString tableName = "");
    void setName(QString tableName);
    void insertMusic(int index, Music *music);
    void setMusiclist(QList<Music*> list);
    void clearAll();
    void remove(int index);
    void append(Music *music);
    Music* get(int index);
    QList<Music*> getAll();

signals:
    void sizeChanged(int size);
    void musicDoubleClicked(QList<Music*> list, int index);

private:
    QList<Music*> list;
    QString name;
    bool preventChangeSignal;
    
    void load();
    void save();

private slots:
    void onTableItemClicked(QTableWidgetItem *item);
};

#endif // MUSICTABLEWIDGET_H
