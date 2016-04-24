#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class WebEngineView;
class NetManager;
class QWebChannel;

class WebCallback : public QObject
{
    Q_OBJECT
public:
    WebCallback(QObject *parent = 0);

signals:
    void sigOnItemClicked(int typeId);
    void sigOnTitleClicked(const QString &url);

public slots:
    void onItemClicked(int typeId);
    void onTitleClicked(const QString &url);
};

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWebChannel *webChannel, QWidget *parent = 0);
    ~Dialog();

private slots:
    void onResponse(const QByteArray &data);
    void onFinished(bool isSuccess);
    void onTabClicked(int index);
    void onItemClicked(int typeId);
    void onTitleClicked(const QString &url);

private:
    Ui::Dialog *ui;
    WebEngineView *m_webView;
    WebEngineView *m_webArticle;
    NetManager *m_net;
    WebCallback *m_webCallback;
    int m_curTypeId;
    QString m_curArticleUrl;
};

#endif // DIALOG_H
