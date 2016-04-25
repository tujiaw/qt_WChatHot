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

class ArticleTitleRequest : public Request
{
    Q_OBJECT
public:
    explicit ArticleTitleRequest(QObject *parent=0);
    explicit ArticleTitleRequest(int typeId, const QString &key, int page, QObject *parent=0);
    QString getUrl();

    void setTypeId(int typeId) { m_typeId = typeId; }
    int typeId() const { return m_typeId; }

    void setKey(const QString &key) { m_key = key; }
    QString key() const { return m_key; }

    void setPage(int page) { m_page = page; }
    int page() const { return m_page; }

private:
    void init();

private:
    int m_typeId;
    QString m_key;
    int m_page;
};

#endif // REQUESTHEADER_H
