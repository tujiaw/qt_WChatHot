#include "dialog.h"
#include "ui_dialog.h"
#include "webengineview.h"
#include "request.h"
#include "response.h"
#include "netmanager.h"

#include <QFile>
#include <QDebug>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    m_webView = new WebEngineView(this);
    ui->webviewLayout->addWidget(m_webView);
    m_net = new NetManager();

    connect(m_webView, SIGNAL(loadFinished(bool)), this, SLOT(onFinished(bool)));
    connect(ui->tabWidget, SIGNAL(sigButtonClicked(int)), this, SLOT(onButtonClicked(int)));

    QFile file(":/rc/articleList.html");
    if (file.open(QIODevice::ReadOnly)) {
        m_webView->setHtml(QString::fromLocal8Bit(file.readAll()));
    }
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::onResponse(const QByteArray &data)
{
}

void Dialog::onFinished(bool isSuccess)
{
    ArticleListRequest request;
    QString url = request.getUrl();
    url = QString::fromLocal8Bit(url.toLocal8Bit().toBase64());
    QString script = QString("getDataFromUrl") + "('" + url + "')";
    qDebug() << script;
    m_webView->page()->runJavaScript(script, [](const QVariant &data) {
        qDebug() << "xxxxxxxxxxxxx";
    });
}

void Dialog::onButtonClicked(int index)
{
    if (0 == index) {

    } else if (1 == index) {

    } else if (2 == index) {

    }
}
