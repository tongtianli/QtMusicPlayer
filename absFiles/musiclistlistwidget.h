#ifndef MUSICLISTLISTWIDGET_H
#define MUSICLISTLISTWIDGET_H

#include "music.h"
#include "localmusicwidget.h"
#include "usermusicwidget.h"
#include "playlistwidget.h"
#include "findmusicwidget.h"
#include "addnewmusiclist.h"

class MusiclistListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit MusiclistListWidget(QWidget *parent = nullptr);
    ~MusiclistListWidget() override;

    void setup(QWidget *parent, QScrollArea *scrollArea, PlayListWidget *playlistWidget);
    void initialDefaultWidgets();
    void buildMenu();
    void loadUserMusiclists();
    QHash<QString,QWidget*> name_widgetHash;

public slots:
    void addUserMusiclist(QString listname);

protected:
    void mousePressEvent(QMouseEvent *event) override;

signals:
    void sizeChanged();

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void currentChanged(const QModelIndex &current,const QModelIndex &previous) override;

private slots:
    void onActionAddTriggered(bool checked);

private:
    const QSet<QString> defaultListnames = {"发现音乐","本地音乐","我喜欢的音乐"};
    QWidget *parent;
    QScrollArea *scrollArea;
    PlayListWidget *playlistWidget;
    QMenu *menu;
    AddNewMusiclist *question;

    void saveUserMusiclist();

};

#endif // MUSICLISTLISTWIDGET_H
