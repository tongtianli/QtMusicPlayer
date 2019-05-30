#ifndef MUSICLISTLISTWIDGET_H
#define MUSICLISTLISTWIDGET_H

#include "music.h"
#include "localmusicwidget.h"
#include "usermusicwidget.h"
#include "playlistwidget.h"
#include "findmusicwidget.h"

class MusiclistListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit MusiclistListWidget(QWidget *parent = nullptr);
    ~MusiclistListWidget() override;

    void setup(QWidget *parent, QScrollArea *scrollArea, PlayListWidget *playlistWidget);
    void initialDefaultWidgets();
    void loadUserMusiclists();
    void addUserMusiclist(QString listname);

    QHash<QString,QWidget*> name_widgetHash;

signals:
    void sizeChanged();

public slots:

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void currentChanged(const QModelIndex &current,const QModelIndex &previous) override;

private:
    const QSet<QString> defaultListnames = {"发现音乐","本地音乐","我喜欢的音乐"};
    QWidget *parent;
    QScrollArea *scrollArea;
    PlayListWidget *playlistWidget;


    void saveUserMusiclist();

};

#endif // MUSICLISTLISTWIDGET_H
