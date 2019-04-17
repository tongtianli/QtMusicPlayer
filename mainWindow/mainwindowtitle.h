#ifndef MAINWINDOWTITLE_H
#define MAINWINDOWTITLE_H

#include "music.h"

namespace Ui {
class MainwindowTitle;
}

class MainwindowTitle : public QWidget
{
    Q_OBJECT

public:
    explicit MainwindowTitle(QWidget *parent = nullptr);
    ~MainwindowTitle();
    QLineEdit* getSearchLineEdit();

private slots:
    void on_btnExit_clicked();
    void on_btnFullScreen_clicked();
    void on_btnHide_clicked();

private:
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::MainwindowTitle *ui;
    QMainWindow *mainwindow;
    bool movingEnable;
    QPoint lastMousePostion;
};

#endif // MAINWINDOWTITLE_H
