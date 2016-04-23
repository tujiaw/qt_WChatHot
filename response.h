#ifndef RESPONSE_H
#define RESPONSE_H

#include <QObject>
#include <QVariantMap>

class Response : public QObject
{
    Q_OBJECT
public:
    explicit Response(const QByteArray &data, QObject *parent = 0);

    int resCode() const;
    QString resError() const;
    const QVariantMap& body() const;
    QByteArray bodyBase64() const;

protected:
    int m_resCode;
    QString m_resError;
    QVariantMap m_resBody;
};

#endif // RESPONSE_H
