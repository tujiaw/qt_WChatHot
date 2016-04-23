#ifndef NETMANAGER_H
#define NETMANAGER_H

#include <QObject>
#include <functional>

class QNetworkAccessManager;
class NetManager : QObject
{
public:
    explicit NetManager(QObject *parent = 0);
    void request(const QString &url, const std::function<void(const QByteArray &data)> &handle);

private:
    QNetworkAccessManager *m_netManager;
};

#endif // NETMANAGER_H
