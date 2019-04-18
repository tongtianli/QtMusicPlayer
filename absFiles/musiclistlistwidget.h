#ifndef MUSICLISTLISTWIDGET_H
#define MUSICLISTLISTWIDGET_H

#include "music.h"
#include "localmusicwidget.h"
#include "usermusicwidget.h"
#include "playlistwidget.h"

class MusiclistListWidget : public QListWidget
{
    Q_OBJECT
public:
    MusiclistListWidget();
    explicit MusiclistListWidget(QObject *parent);
    ~MusiclistListWidget();

    void setup(QMainWindow *parent, QScrollArea *scrollArea, PlayListWidget *playlistWidget);
    void initialDefaultWidgets();
    void loadUserMusiclists();
    void addUserMusiclist(QString listname);

signals:
    void sizeChanged();

public slots:

protected:
    virtual void resizeEvent(QResizeEvent *event);

private:
    const QSet<QString> defaultListnames = {"发现音乐","本地音乐","我喜欢的音乐"};
    QWidget *parent;
    QScrollArea *scrollArea;
    PlayListWidget *playlistWidget;
    QHash<QString,QWidget*> name_widgetHash;


    void saveUserMusiclist();

};

#endif // MUSICLISTLISTWIDGET_H
