#ifndef USERMUSICWIDGET_H
#define USERMUSICWIDGET_H

#include "music.h"
#include "musictablewidget.h"


namespace Ui {
class UserMusicWidget;
}

class UserMusicWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserMusicWidget(QWidget *parent = nullptr);
    ~UserMusicWidget();

private:
    Ui::UserMusicWidget *ui;
    MusicTableWidget *list;

};

#endif // USERMUSICWIDGET_H
