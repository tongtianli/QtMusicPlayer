#ifndef MUSICTABLEWIDGET_H
#define MUSICTABLEWIDGET_H

#include "music.h"
#include "filemanager.h"

class MusicTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    explicit MusicTableWidget(QWidget *parent = nullptr, QString tableName = "");
    virtual ~MusicTableWidget();
    void insertMusic(int index, Music *music);
    void setMusiclist(QList<Music*> list);
    void clearAll();
    void remove(int index);
    void append(Music *music);
    Music* get(int index);
    QList<Music*>* getAll();

signals:
    void sizeChanged(int size);
    void itemDoubleClicked(int index);

public slots:

protected:
    QList<Music*> list;
    FileManager manager;
    QString name;

};

#endif // MUSICTABLEWIDGET_H