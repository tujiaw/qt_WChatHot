#include "request.h"
#include <QDateTime>
#include <QCryptographicHash>
#include <QDebug>

Request::Request(QObject *parent)
    : QObject(parent)
{
    m_timestamp = getTimestamp();
}

QString Request::getTimestamp()
{
    QDateTime dateTime = QDateTime::currentDateTime();
    return dateTime.toString("yyyyMMddHHmmss");
}

QString Request::getMd5(const QList<QPair<QString, QString> > &argv, const QString &secret)
{
   QList<QPair<QString, QString>> list = argv;
   qSort(list.begin(), list.end(), [](const QPair<QString, QString> &left, const QPair<QString, QString> &right) {
        return left.first < right.first;
   });
   QString str;
   for (auto iter = list.begin(); iter != list.end(); ++iter) {
       str += ((*iter).first + (*iter).second);
   }
   str += secret.toLower();
   return QCryptographicHash::hash(str.toLocal8Bit(), QCryptographicHash::Md5).toHex();
}

ArticleListRequest::ArticleListRequest(QObject *parent) : Request(parent)
{
    m_address = "http://route.showapi.com/582-1";
    m_signMethod = "md5";
    m_appId = 17262;
    m_secret = "21b693f98bd64e71a9bdbb5f7c76659c";
}

QString ArticleListRequest::getUrl()
{
    QList<QPair<QString, QString>> argv;
    argv << QPair<QString, QString>("showapi_appid", QString::number(m_appId));
    argv << QPair<QString, QString>("showapi_timestamp", m_timestamp);

    m_sign = getMd5(argv, m_secret);
    QString result = m_address + "?showapi_sign=" + m_sign;
    for (auto iter = argv.begin(); iter != argv.end(); ++iter) {
        result += ("&" + (*iter).first + "=" + (*iter).second);
    }
    return result;
}
