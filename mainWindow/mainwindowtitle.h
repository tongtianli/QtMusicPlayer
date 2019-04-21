#ifndef MAINWINDOWTITLE_H
#define MAINWINDOWTITLE_H

#include "music.h"

inline bool inSquare(QPoint rightBottom, QPoint leftTop, int squareSize){
    return rightBottom.x()-leftTop.x()>=0 && rightBottom.y()-leftTop.y()>=0 &&
            rightBottom.x()-leftTop.x()<=squareSize && rightBottom.y()-leftTop.y()<=squareSize;
}
inline bool inReverseSquare(QPoint rightTop, QPoint leftBottom, int squareSize){
    return rightTop.x()-leftBottom.x()>=0 && rightTop.y()-leftBottom.y()<=0 &&
            rightTop.x()-leftBottom.x()<=squareSize && rightTop.y()-leftBottom.y()>=(-squareSize);
}

namespace Ui {
class MainwindowTitle;
}

enum ResizeRegion{TopLeft,BottomLeft,TopRight,BottomRight,INVALID};

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
    QWidget *mainwindow;
    bool movingEnable;
    QPoint lastMousePostion;
    ResizeRegion resizeRegion;
};

#endif // MAINWINDOWTITLE_H
