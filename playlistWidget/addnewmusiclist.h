#ifndef ADDNEWMUSICLIST_H
#define ADDNEWMUSICLIST_H

#include <QDialog>

namespace Ui {
class AddNewMusiclist;
}

class AddNewMusiclist : public QDialog
{
    Q_OBJECT

public:
    explicit AddNewMusiclist(QWidget *parent = nullptr);
    ~AddNewMusiclist();

signals:
    void addlist(QString listname);
    void cancel();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEdit_returnPressed();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::AddNewMusiclist *ui;
};

#endif // ADDNEWMUSICLIST_H
