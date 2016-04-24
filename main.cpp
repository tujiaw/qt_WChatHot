#include "dialog.h"
#include "websockettransport.h"
#include <QApplication>
#include <QtWebSockets/QWebSocketServer>
#include <QWebChannel>
#include <QFileInfo>
#include <QDir>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    QString curRunDir = QCoreApplication::applicationDirPath();
//    QStringList fileList;
//    fileList << "articleList.html";
//    fileList << "qwebchannel.js";
//    QDir dir(curRunDir);
//    foreach (const QString &file, fileList) {
//        QFile::remove(curRunDir + "/" + file);
//        QFileInfo jsFileInfo(curRunDir + "/" + file);
//        if (!jsFileInfo.exists()) {
//            QFile::copy(QString(":/rc/%1").arg(file), jsFileInfo.absoluteFilePath());
//            qDebug() << jsFileInfo.absoluteFilePath();
//        }
//    }

    QWebSocketServer server(QStringLiteral("WChatHot"), QWebSocketServer::NonSecureMode);
    if (!server.listen(QHostAddress::LocalHost, 12345)) {
        qFatal("Failed to open web socket server.");
        return 1;
    }
    QWebChannel channel;
    QObject::connect(&server, &QWebSocketServer::newConnection, [&server, &channel] {
        QWebSocket *socket = server.nextPendingConnection();
        WebSocketTransport *transport = new WebSocketTransport(socket);
        channel.connectTo(transport);
    });

    Dialog w(&channel);
    w.show();

    return a.exec();
}
