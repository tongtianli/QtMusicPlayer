#ifndef PLAYSTATEWIDGET_H
#define PLAYSTATEWIDGET_H

#include <QWidget>

namespace Ui {
class PlayStateWidget;
}

class PlayStateWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlayStateWidget(QWidget *parent = nullptr);
    ~PlayStateWidget();

private:
    Ui::PlayStateWidget *ui;
};

#endif // PLAYSTATEWIDGET_H
