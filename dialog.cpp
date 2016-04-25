#include "dialog.h"
#include "ui_dialog.h"
#include "webengineview.h"
#include "netmanager.h"

#include <QFile>
#include <QDebug>
#include <QWebChannel>
#include <QMessageBox>

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

void WebCallback::onTitleSCrollToBottom()
{
    emit sigOnTitleScrollToBottom();
}

Dialog::Dialog(QWebChannel *webChannel, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
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
    connect(m_webCallback, &WebCallback::sigOnTitleScrollToBottom, this, &Dialog::onTitleScrollToBottom);

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
    if (isSuccess) {
        m_webView->page()->webChannel()->registerObject(QStringLiteral("webCallback"), m_webCallback);
        m_webView->page()->runJavaScript("initWebSocket()",[this](const QVariant &data) {
            onTabClicked(0, 0);
        });
    } else {
        QMessageBox::warning(this, tr("error"), tr("load html failed"));
    }
}

void Dialog::onTabClicked(int oldIndex, int newIndex)
{
    m_webView->setVisible(newIndex != 2);
    m_webArticle->setVisible(newIndex == 2);

    qDebug() << "oldIndex:" << oldIndex << ",newIndex:" << newIndex;
    if (0 == newIndex) {
        ArticleListRequest request;
        if (!m_webView->url().toString().contains(request.address())) {
            requestCategoryList();
        }
    } else if (1 == newIndex) {
        m_titleRequest.setPage(1);
        if (!m_webView->url().toString().contains(m_titleRequest.address())) {
            requestTitleList();
        }
    }
}

void Dialog::onItemClicked(int typeId)
{
    m_titleRequest.setTypeId(typeId);
    m_titleRequest.setPage(1);
    ui->tabWidget->setSelectedStyle(1);
    requestTitleList();
}

void Dialog::onTitleClicked(const QString &url)
{
    ui->tabWidget->setSelectedStyle(2);
    m_webView->setVisible(false);
    m_webArticle->setVisible(true);
    if (m_webArticle->url() != url) {
        m_webArticle->load(QUrl(url));
    }
}

void Dialog::onTitleScrollToBottom()
{
    if (ui->tabWidget->currentIndex() == 1) {
        int curPage = qMax(m_titleRequest.page(), 1);
        int newPage = curPage + 1;
        m_titleRequest.setPage(newPage);
        requestTitleList();
    }
}

void Dialog::requestCategoryList()
{
    ArticleListRequest request;
    QString url = request.getUrl();
    url = QString::fromLocal8Bit(url.toLocal8Bit().toBase64());
    QString script = QString("showArticleList") + "('" + url + "')";
    m_webView->page()->runJavaScript(script);
}

void Dialog::requestTitleList()
{
    QString url = m_titleRequest.getUrl();
    QString base64Url = QString::fromLocal8Bit(url.toLocal8Bit().toBase64());
    QString script;
    if (m_titleRequest.page() > 1) {
        script = QString("showArticleTitle") + "('" + base64Url + "', '1')";
    } else {
        script = QString("showArticleTitle") + "('" + base64Url + "')";
    }
    qDebug() << "request title list:" << url;
    m_webView->page()->runJavaScript(script, [](const QVariant &data) {

    });
}
