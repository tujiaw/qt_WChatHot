#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class WebEngineView;
class NetManager;
class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void onResponse(const QByteArray &data);
    void onFinished(bool isSuccess);
    void onButtonClicked(int index);

private:
    Ui::Dialog *ui;
    WebEngineView *m_webView;
    NetManager *m_net;
};

#endif // DIALOG_H
