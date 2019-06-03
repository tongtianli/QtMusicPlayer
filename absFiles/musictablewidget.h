#ifndef MUSICTABLEWIDGET_H
#define MUSICTABLEWIDGET_H

#include "music.h"
#include "musicdownloader.h"
#include "defaultmusiclist.h"

class MusicTableWidget : public QTableWidget
{
    Q_OBJECT

public:
    explicit MusicTableWidget(QWidget *parent = nullptr, QString tableName = "");
    ~MusicTableWidget() override;
    void setName(QString tableName);
    void insertMusic(int index, Music *music);
    void setMusiclist(QList<Music*> list);
    void clearAll();
    void remove(int index);
    void remove(Music *music);
    void append(Music *music);
    Music* get(int index);
    QList<Music*> getAll();
    void downloadAllMusic();
    void play();
    void playLater();
    bool contains(int id);

public slots:
    void buildBookMenu(QHash<QString,QWidget*> name_widgetHash);

signals:
    void sizeChanged(int size);
    void musicDoubleClicked(QList<Music*> list, int index);
    void playThisListLater(QList<Music*> list);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    virtual void selectionChanged(const QItemSelection &selected,
                          const QItemSelection &deselected) override;

private:
    QList<Music*> list;
    QSet<int> idSet;
    QHash<QString, QWidget*> name_widgetHash;
    QString name;
    bool preventChangeSignal;


    QAction *actPlay;
    QAction *actPlayLater;
    QMenu *menu;
    QMenu *bookMenu;
    QAction *actDownload;
    QAction *actRemove;

    QList<int> sortedSelectionIndexes;
    QString bookSelection;


private slots:
    void onTableItemDoubleClicked(QTableWidgetItem *item);
    void addSelectionsToMusiclist(QAction *action);
    void onActionRemoveTriggered();
    void onActionPlayTriggered();
    void onActionPlayLaterTriggered();
    void onActionDownloadTriggered();
    void saveCurrentSelection();
};

#endif // MUSICTABLEWIDGET_H
