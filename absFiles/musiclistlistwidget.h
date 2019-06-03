#ifndef MUSICLISTLISTWIDGET_H
#define MUSICLISTLISTWIDGET_H

#include "music.h"
#include "localmusicwidget.h"
#include "usermusicwidget.h"
#include "playlistwidget.h"
#include "findmusicwidget.h"
#include "addnewmusiclist.h"
#include "defaultmusiclist.h"
#include "datamanager.h"

class MusiclistListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit MusiclistListWidget(QWidget *parent = nullptr);
    ~MusiclistListWidget() override;

    void setup(QWidget *parent, QScrollArea *scrollArea, PlayListWidget *playlistWidget);
    void initialDefaultWidgets(QHash<int,Music*> musicpool);
    void buildMenu();
    void loadUserMusiclists(QHash<int,Music*> musicpool);
    void saveUserMusiclist();
    void saveLocalMusiclist();
    void deleteList(QString name);
    QHash<QString,QWidget*> name_widgetHash;

    const int USERLIST_STARTINDEX = 5;

public slots:
    void addUserMusiclist(QString listname);

signals:
    void sizeChanged();
    void listChanged(QHash<QString,QWidget*> name_widgetHash);
    void addMusic(Music* music);

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void currentChanged(const QModelIndex &current,const QModelIndex &previous) override;
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void onActionAddTriggered(bool checked);
    void onActionPlayTriggered(bool checked);
    void onActionPlayLaterTriggered(bool checked);
    void onActionEditTriggered(bool checked);
    void onActionDownloadTriggered(bool checked);
    void onActionDeletelistTriggered(bool checked);
    void sendAddSignal(Music *music);

private:
    QWidget *parent;
    QScrollArea *scrollArea;
    PlayListWidget *playlistWidget;
    QMenu *menu;
    QMenu *itemMenu_user;
    QMenu *itemMenu_default;
    AddNewMusiclist *addNewMusiclist;
    QString curListName ;
};

#endif // MUSICLISTLISTWIDGET_H
