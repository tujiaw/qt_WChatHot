#include "json.h"
#include <QJsonObject>
#include <QJsonDocument>

namespace json
{
    QVariantMap toMap(const QByteArray &val)
    {
        QJsonParseError jError;
        QJsonDocument jDoc = QJsonDocument::fromJson(val, &jError);
        if (jError.error == QJsonParseError::NoError) {
            if (jDoc.isObject()) {
                QJsonObject jObj = jDoc.object();
                return jObj.toVariantMap();
            }
        }
        QVariantMap ret;
        return ret;
    }

    QString toString(const QVariantMap &val)
    {
        QJsonObject jobj = QJsonObject::fromVariantMap(val);
        QJsonDocument jdoc(jobj);
        return QString(jdoc.toJson(QJsonDocument::Compact));
    }
}
