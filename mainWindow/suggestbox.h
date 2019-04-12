#ifndef SUGGESTBOX_H
#define SUGGESTBOX_H

#include "music.h"

const QString searchUrl = QStringLiteral("https://api.itooi.cn/music/netease/search?key=579621905&s=%1&type=song&limit=4&offset=0");

class SuggestBox : public QObject
{
    Q_OBJECT
public:
    explicit SuggestBox(QLineEdit *parent = nullptr);
    ~SuggestBox() override;
    bool eventFilter(QObject *obj, QEvent *ev) override;
    void showResult(const QVector<QString> &choices);

public slots:
    void doneCompletion();
    void preventSuggest();
    void autoSuggest();
    void handleNetworkData(QNetworkReply *networkReply);

private:
    QLineEdit *editor = nullptr;
    QTreeWidget *popup = nullptr;
    QTimer timer;
    QNetworkAccessManager networkManager;
    QJsonArray arrayTemp;
    int curIndex;

signals:

public slots:
};

#endif // SUGGESTBOX_H
