#include "mainwindowtitle.h"
#include "ui_mainwindowtitle.h"

MainwindowTitle::MainwindowTitle(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainwindowTitle)
{
    ui->setupUi(this);
    mainwindow = qobject_cast<QMainWindow*>(parent);
    movingEnable = false;
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
    if(movingEnable){
        mainwindow->move(mainwindow->pos()+(event->globalPos()-lastMousePostion));
        lastMousePostion = event->globalPos();
    }
}

void MainwindowTitle::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        movingEnable = false;
    }
}

