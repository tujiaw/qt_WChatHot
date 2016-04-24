#include "dialog.h"
#include "ui_dialog.h"
#include "webengineview.h"
#include "request.h"
#include "response.h"
#include "netmanager.h"

#include <QFile>
#include <QDebug>
#include <QWebChannel>

WebCallback::WebCallback(QObject *parent) : QObject(parent)
{
}

void WebCallback::onItemClicked(int typeId)
{
    emit sigOnItemClicked(typeId);
}

void WebCallback::onTitleClicked(const QString &url)
{
    emit sigOnTitleClicked(url);
}

Dialog::Dialog(QWebChannel *webChannel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog), m_curTypeId(0)
{
    ui->setupUi(this);
    m_webView = new WebEngineView(this);
    m_webArticle = new WebEngineView(this);
    ui->webviewLayout->addWidget(m_webView);
    ui->webviewLayout->addWidget(m_webArticle);
    m_net = new NetManager();
    m_webCallback = new WebCallback(this);

    m_webView->load(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/articleList.html"));
    connect(m_webView, SIGNAL(loadFinished(bool)), this, SLOT(onFinished(bool)));
    connect(ui->tabWidget, SIGNAL(sigButtonClicked(int)), this, SLOT(onTabClicked(int)));
    m_webView->page()->setWebChannel(webChannel);

    onTabClicked(0);
    connect(m_webCallback, &WebCallback::sigOnItemClicked, this, &Dialog::onItemClicked);
    connect(m_webCallback, &WebCallback::sigOnTitleClicked, this, &Dialog::onTitleClicked);
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
    m_webView->page()->webChannel()->registerObject(QStringLiteral("webCallback"), m_webCallback);
    m_webView->page()->runJavaScript("initWebSocket()",[this](const QVariant &data) {
        if (0 == ui->tabWidget->currentIndex()) {
            ArticleListRequest request;
            QString url = request.getUrl();
            url = QString::fromLocal8Bit(url.toLocal8Bit().toBase64());
            QString script = QString("showArticleList") + "('" + url + "')";
            m_webView->page()->runJavaScript(script);
        }
    });
}

void Dialog::onTabClicked(int index)
{
    m_webView->setVisible(index != 2);
    m_webArticle->setVisible(index == 2);

    if (0 == index) {
        ArticleListRequest request;
        QString url = request.getUrl();
        url = QString::fromLocal8Bit(url.toLocal8Bit().toBase64());
        QString script = QString("showArticleList") + "('" + url + "')";
        m_webView->page()->runJavaScript(script);
    } else if (1 == index) {
        onItemClicked(m_curTypeId);
    } else if (2 == index) {
        if (m_webArticle->url() != m_curArticleUrl) {
            m_webArticle->load(QUrl(m_curArticleUrl));
        }
    }
}

void Dialog::onItemClicked(int typeId)
{
    m_curTypeId = typeId;
    ArticleTitleRequest request(typeId);
    QString url = request.getUrl();
    url = QString::fromLocal8Bit(url.toLocal8Bit().toBase64());
    QString script = QString("showArticleTitle") + "('" + url + "')";
    qDebug() << request.getUrl();
    m_webView->page()->runJavaScript(script, [](const QVariant &data) {

    });
    ui->tabWidget->setSelectedStyle(1);
}

void Dialog::onTitleClicked(const QString &url)
{
    ui->tabWidget->setSelectedStyle(2);
    m_curArticleUrl = url;
    onTabClicked(2);
}
