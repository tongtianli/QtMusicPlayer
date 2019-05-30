#ifndef HEADER_H
#define HEADER_H

#include <QObject>
#include <QWidget>
#include <QMainWindow>
#include <QTableView>
#include <QAbstractItemModel>
#include <QDebug>
#include <QLayout>
#include <QDir>
#include <QUrl>
#include <QHash>
#include <QTime>
#include <QHashIterator>
#include <QException>
#include <QFileDialog>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QListWidgetItem>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QMediaPlaylist>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QMetaDataReaderControl>
#include <QtWidgets>
#include <QLineEdit>
#include <QNetworkAccessManager>
#include <QTimer>
#include <QTreeWidget>
#include <QtNetwork>
#include <qnetworkreply.h>
#include <QMessageBox>

enum Playmode{loop,single,random};

typedef struct Music{
    int ID;
    QString name;
    QString singer;
    QString duration;
    QUrl pic;
    QUrl lrc;
    QUrl url;
    bool local;
    bool like;
    QString size;
}Music;


#endif // HEADER_H
