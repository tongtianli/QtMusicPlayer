#include "musictablewidget.h"

MusicTableWidget::MusicTableWidget(QWidget *parent, QString tableName) :
    QTableWidget(parent)
{
    table = this;
    manager = new FileManager(tableName);
    name = tableName;
}

MusicTableWidget::~MusicTableWidget()
{
    delete manager;
    delete this;
}

void MusicTableWidget::initColumnCats(QList<QString> categories)
{
    int n = categories.size();
    QTableWidgetItem *headitem;
    table->setColumnCount(n);
    for(int i=0;i<categories.size();i++){
        headitem = new QTableWidgetItem(categories.at(i));
        QFont font = headitem->font();
        font.setBold(true);
        font.setPointSize(12);
        headitem->setFont(font);
        table->setHorizontalHeaderItem(i,headitem);
    }
}
