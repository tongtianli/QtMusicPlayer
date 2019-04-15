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
    MusicTableWidget *table;

    explicit UserMusicWidget(QWidget *parent = nullptr);
    ~UserMusicWidget();

    void setTableName(QString name);

private:
    Ui::UserMusicWidget *ui;

};

#endif // USERMUSICWIDGET_H
