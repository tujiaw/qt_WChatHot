#include "netmanager.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>


NetManager::NetManager(QObject *parent) : QObject(parent)
{
    m_netManager = new QNetworkAccessManager(this);
}

void NetManager::request(const QString &url, const std::function<void(const QByteArray &data)> &handle)
{
    QUrl aUrl(url);
    QNetworkReply *reply = m_netManager->get(QNetworkRequest(aUrl));
    QObject::connect(reply, &QNetworkReply::finished, [reply, handle, this] () {
        handle(reply->readAll());
        reply->deleteLater();
    });

    QObject::connect(reply, &QNetworkReply::readyRead, [reply, this] () {
        qDebug() << "ready read";
    });

    QObject::connect(reply, &QNetworkReply::downloadProgress, [this] (qint64 recv, qint64 total) {
        qDebug() << "progress:" << recv << "," << total;
    });
}
