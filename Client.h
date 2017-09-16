#ifndef CLIENT_H
#define CLIENT_H

#include <QtGui>
#include <QtNetwork>
#include "ui_Client.h"

class Client : public QWidget, private Ui::Client {

    Q_OBJECT

    public:
        Client();

    private slots:
        void on_connectButton_clicked();
        void on_sendButton_clicked();
        void on_message_returnPressed();
        void dataReceived();
        void connectUser();
        void logOut();
        void socketError(QAbstractSocket::SocketError error);

    private:
        QTcpSocket *socket;
        quint16 messageLength;
};

#endif // CLIENT_H
