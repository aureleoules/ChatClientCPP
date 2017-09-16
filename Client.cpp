#include "Client.h"

Client::Client() {
    setupUi(this);

    socket = new QTcpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(dataReceived()));
    connect(socket, SIGNAL(connected()), this, SLOT(connectUser()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(logOut()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(socketError(QAbstractSocket::SocketError)));

    messageLength = 0;
}

void Client::on_connectButton_clicked() {
    messagesList->append(tr("<em>Connecting...</em>"));
    connectButton->setEnabled(false);

    socket->abort();
    //Connection:
    socket->connectToHost(serverIP->text(), serverPort->value());
}

void Client::on_sendButton_clicked() {
    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly);

    QString messageToSend = tr("<strong>") + nickname->text() + tr("</strong> ") + message->text();
    out << (quint16) 0;
    out << messageToSend;
    out.device()->seek(0);
    out << (quint16) (packet.size() - sizeof(quint16));
    socket->write(packet);
    message->clear();
    message->setFocus();
}

void Client::on_message_returnPressed() {
    on_sendButton_clicked();
}

void Client::dataReceived() {
    QDataStream in(socket);

    if(messageLength ==0) {
        if(socket->bytesAvailable() < (int) sizeof(quint16)) {
            return;
        }
        in >> messageLength;
    }

    if(socket->bytesAvailable() < messageLength) {
        return;
    }

    QString receivedMessage;
    in >> receivedMessage;

    messagesList->append(receivedMessage);

    messageLength = 0;
}

void Client::connectUser() {
    messagesList->append(tr("<em>Successfully connected!</em>"));
    connectButton->setEnabled(true);
}

void Client::logOut() {
    messagesList->append(tr("<em>Logged out.</em>"));
}

void Client::socketError(QAbstractSocket::SocketError error) {
    switch(error) {
        case QAbstractSocket::HostNotFoundError:
            messagesList->append(tr("<em>Error: Could not find server.</em>"));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            messagesList->append(tr("<em>Error: Connection refused.</em>"));
            break;
        case QAbstractSocket::RemoteHostClosedError:
            messagesList->append(tr("<em>Error: Connection closed.</em>"));
            break;
        default:
            messagesList->append(tr("<em>Error: ") + socket->errorString() + tr("</em>"));
    }

    connectButton->setEnabled(true);
}




