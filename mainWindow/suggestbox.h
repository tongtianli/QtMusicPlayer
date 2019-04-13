#ifndef SUGGESTBOX_H
#define SUGGESTBOX_H

#include "music.h"

const QString searchUrl = QStringLiteral("https://api.itooi.cn/music/netease/search?key=579621905&s=%1&type=song&limit=4&offset=0");

namespace Ui{
class SuggestBox;
}

class SuggestBox : public QWidget
{
    Q_OBJECT
public:
    explicit SuggestBox(QWidget *parent = nullptr, QLineEdit *lineEdit = nullptr);
    ~SuggestBox() override;
    void showResult(const QVector<QString> &choices);

public slots:
    void doneSelection();
    void preventSuggest();
    void autoSuggest();
    void handleNetworkData(QNetworkReply *networkReply);

private:
    Ui::SuggestBox *ui;
    QLineEdit *editor = nullptr;
    QListWidget *suggestList = nullptr;
    QTimer timer;
    QNetworkAccessManager networkManager;
    QJsonArray arrayTemp;
    int x;
    int y;

    void connectSlots();

signals:
    void selectSearchedSong(Music *music);

};

#endif // SUGGESTBOX_H
