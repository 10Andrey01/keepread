#include "wserver.h"
#include <QWebSocket>

wserver::wserver (quint16 port, QObject *parent)
    : QObject (parent), m_port (port),
      m_pWebSocketServer (
          new QWebSocketServer (QStringLiteral ("Keepread server"),
                                QWebSocketServer::NonSecureMode),
          this)

{
  if (!m_pWebSocketServer->listen (QHostAddress::Any, m_port))
    {
      qFatal () << "ERROR: WebSocket server Keepread not started";
      return;
    }
  qInfo () << "INFO: WebSocker server Keepread started on " << m_port
           << " port";
  connect (m_pWebSocketServer, &QWebSocketServer::newConnection, this,
           &wserver::onNewConnection);
  connect (m_pWebSocketServer, &QWebSocketServer::closed, this,
           &wserver::closed);
}

void
wserver::onNewConnection ()
{
  QWebSocket *pNewConnection = m_pWebSocketServer->maxPendingConnections ();

  connect (pNewConnection, &QWebSocket::textMessageReceived, this,
           &wserver::processTextMessage);
  connect (pNewConnection, &QWebSocket::binaryMessageReceived, this,
           &wserver::processBinaryMessage);
  connect (pNewConnection, &QWebSocket::disconnected, this,
           &wserver::socketDisconected);
  m_clients << pNewConnection;
}

void
wserver::processTextMessage (QString message)
{
  QWebSocket *pClient = qobject_cast<QWebSocket *> (sender ());
  qDebug () << "Text message: " << pClient->peerName () << message;
}

void
wserver::processBinaryMessage (QByteArray message)
{
  QWebSocket *pClient = qobject_cast<QWebSocket *> (sender ());
  qDebug () << "Binary message: " << pClient->peerName () << message;
}

void
wserver::socketDisconected ()
{
  QWebSocket *pClient = qobject_cast<QWebSocket *> (sender ());
  qInfo () << "INFO: client " << pClient->peerName () << " disconnected";
  if (pClient)
    {
      m_clients.removeAll (pClient);
      pClient->deleteLater ();
    }
}
