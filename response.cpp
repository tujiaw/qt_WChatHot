#include "response.h"
#include "json.h"

Response::Response(const QByteArray &data, QObject *parent) : QObject(parent)
{
    QVariantMap json = json::toMap(data);
    m_resCode = json["showapi_res_code"].toInt();
    m_resError = json["showapi_res_error"].toString();
    m_resBody = json["showapi_res_body"].toMap();
}

int Response::resCode() const
{
    return m_resCode;
}

QString Response::resError() const
{
    return m_resError;
}

const QVariantMap& Response::body() const
{
    return m_resBody;
}

QByteArray Response::bodyBase64() const
{
    return json::toString(m_resBody).toLocal8Bit().toBase64();
}
