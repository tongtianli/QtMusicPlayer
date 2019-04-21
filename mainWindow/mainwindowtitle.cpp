#include "mainwindowtitle.h"
#include "ui_mainwindowtitle.h"

MainwindowTitle::MainwindowTitle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainwindowTitle)
{
    ui->setupUi(this);
    mainwindow = qobject_cast<QWidget*>(parent);

    setMouseTracking(true);
    movingEnable = false;
    resizeRegion = ResizeRegion::INVALID;
}

MainwindowTitle::~MainwindowTitle()
{
    delete ui;
}

QLineEdit* MainwindowTitle::getSearchLineEdit()
{
    return ui->searchBox;
}

void MainwindowTitle::on_btnExit_clicked()
{
    mainwindow->close();
}

void MainwindowTitle::on_btnFullScreen_clicked()
{
    if(mainwindow->isMaximized()){
        mainwindow->showNormal();
        ui->btnFullScreen->setIcon(QIcon(":/image/resource/fullscreen.png"));
    }
    else{
        mainwindow->showMaximized();
        ui->btnFullScreen->setIcon(QIcon(":/image/resource/nomal.png"));
    }
}

void MainwindowTitle::on_btnHide_clicked()
{
    mainwindow->showMinimized();
}

void MainwindowTitle::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    on_btnFullScreen_clicked();
}

void MainwindowTitle::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        movingEnable = true;
        lastMousePostion = event->globalPos();
    }
}

void MainwindowTitle::mouseMoveEvent(QMouseEvent *event)
{
    QPoint mouseGlobalPos = event->globalPos();
    if(movingEnable){
        QRect size = mainwindow->geometry();
        QPoint sizeChangedValue = mouseGlobalPos-lastMousePostion;
        switch(resizeRegion){
        case ResizeRegion::TopLeft:
            size.setTopLeft(size.topLeft()+sizeChangedValue);
            mainwindow->setGeometry(size);
            break;
        case ResizeRegion::TopRight:
            size.setTopRight(size.topRight()+sizeChangedValue);
            mainwindow->setGeometry(size);
            break;
        default:
            mainwindow->move(mainwindow->pos()+(event->globalPos()-lastMousePostion));
            break;
        }
        lastMousePostion = event->globalPos();
        return;
    }
    if(inSquare(mouseGlobalPos,mainwindow->geometry().topLeft(),5)){
        resizeRegion = ResizeRegion::TopLeft;
        setCursor(Qt::SizeFDiagCursor);
    }
    else if(inReverseSquare(mainwindow->geometry().topRight(),mouseGlobalPos,5)){
        resizeRegion = ResizeRegion::TopRight;
        setCursor(Qt::SizeBDiagCursor);

    }
    else{
        resizeRegion = ResizeRegion::INVALID;
        setCursor(Qt::ArrowCursor);
        return;
    }
}

void MainwindowTitle::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        movingEnable = false;
    }
}

