#ifndef REQUESTHEADER_H
#define REQUESTHEADER_H

#include <QObject>

class Request : public QObject
{
    Q_OBJECT
public:
    explicit Request(QObject *parent = 0);

    virtual QString getUrl() = 0;
    static QString getTimestamp();
    static QString getMd5(const QList<QPair<QString, QString>> &argv, const QString &secret);

protected:
    QString m_address;
    int m_appId;
    QString m_secret;
    QString m_sign;
    QString m_timestamp;
    QString m_signMethod;
};

class ArticleListRequest : public Request
{
    Q_OBJECT
public:
    explicit ArticleListRequest(QObject *parent=0);
    QString getUrl();
};

#endif // REQUESTHEADER_H
