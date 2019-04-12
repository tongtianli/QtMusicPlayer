#ifndef SUGGESTBOX_H
#define SUGGESTBOX_H

#include "music.h"

const QString gsearchUrl = QStringLiteral("http://google.com/complete/search?output=toolbar&q=%1");

class SuggestBox : public QObject
{
    Q_OBJECT
public:
    explicit SuggestBox(QLineEdit *parent = nullptr);
    ~SuggestBox() override;
    bool eventFilter(QObject *obj, QEvent *ev) override;
    void showCompletion(const QVector<QString> &choices);

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

signals:

public slots:
};

#endif // SUGGESTBOX_H
