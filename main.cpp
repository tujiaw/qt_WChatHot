#include "dialog.h"
#include "websockettransport.h"
#include <QApplication>
#include <QtWebSockets/QWebSocketServer>
#include <QWebChannel>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator *trans = new QTranslator();
    trans->load(":/ch.qm");
    a.installTranslator(trans);

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
