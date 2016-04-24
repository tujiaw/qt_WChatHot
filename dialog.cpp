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

void WebCallback::onTest(const QString &str)
{
    qDebug() << str;
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

    m_webArticle->hide();
    m_webView->load(QUrl::fromLocalFile(QCoreApplication::applicationDirPath() + "/articleList.html"));
    connect(m_webView, SIGNAL(loadFinished(bool)), this, SLOT(onFinished(bool)));
    connect(ui->tabWidget, SIGNAL(sigButtonClicked(int, int)), this, SLOT(onTabClicked(int, int)));
    m_webView->page()->setWebChannel(webChannel);

    connect(m_webCallback, &WebCallback::sigOnItemClicked, this, &Dialog::onItemClicked);
    connect(m_webCallback, &WebCallback::sigOnTitleClicked, this, &Dialog::onTitleClicked);

    this->setStyleSheet("#Dialog{background:rgb(161, 197, 241)}");
    this->setWindowTitle(tr("wchat hot"));
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
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
        onTabClicked(0, 0);
    });
}

void Dialog::onTabClicked(int oldIndex, int newIndex)
{
    m_webView->setVisible(newIndex != 2);
    m_webArticle->setVisible(newIndex == 2);

    if (0 == newIndex && 2 != oldIndex) {
        ArticleListRequest request;
        QString url = request.getUrl();
        url = QString::fromLocal8Bit(url.toLocal8Bit().toBase64());
        QString script = QString("showArticleList") + "('" + url + "')";
        m_webView->page()->runJavaScript(script);
    } else if (1 == newIndex && 2 != oldIndex) {
        onItemClicked(qMax(0, m_curTypeId));
    } else if (2 == newIndex) {
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
    onTabClicked(1, 2);
}
