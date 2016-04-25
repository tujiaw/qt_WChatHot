#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "request.h"

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
    void sigOnTitleScrollToBottom();

public slots:
    void onTest(const QString &str);
    void onItemClicked(int typeId);
    void onTitleClicked(const QString &url);
    void onTitleSCrollToBottom();
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
    void onTabClicked(int oldIndex, int newIndex);

    void onItemClicked(int typeId);
    void onTitleClicked(const QString &url);
    void onTitleScrollToBottom();

private:
    void requestCategoryList();
    void requestTitleList();

private:
    Ui::Dialog *ui;
    WebEngineView *m_webView;
    WebEngineView *m_webArticle;
    NetManager *m_net;
    WebCallback *m_webCallback;
    ArticleTitleRequest m_titleRequest;
};

#endif // DIALOG_H
