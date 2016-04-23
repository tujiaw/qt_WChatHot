#ifndef JSON_H
#define JSON_H

#include <QVariantMap>

namespace json
{
    QVariantMap toMap(const QByteArray &val);
    QString toString(const QVariantMap &val);
}

#endif // JSON_H
