#ifndef MUSICTABLEWIDGET_H
#define MUSICTABLEWIDGET_H

#include "music.h"
#include "filemanager.h"

namespace Ui {
class MusicTableWidget;
}

class MusicTableWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MusicTableWidget(QWidget *parent = nullptr, QString tableName = "");
    virtual ~MusicTableWidget();
    void initColumnCats(QList<QString> categories);
    void insertMedia(int index, Music *music);
    void setMusiclist(QList<Music*> list);
    void clear();
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
    QTableWidget* table;
    FileManager* manager;
    QString name;

private:
    Ui::MusicTableWidget *ui;
};

#endif // MUSICTABLEWIDGET_H
