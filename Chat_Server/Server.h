#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QVector>
#include "Data.h"
#include "Database.h"


class Server: public QTcpServer{
    Q_OBJECT

public:
    Server() = delete;
    Server(uint16_t port, Database* database);

public slots:
    void incomingConnection(qintptr socketDescriptor);
    void readData();

private:
    void sendToClient(QTcpSocket* socketClient, const QString& string);

    Data* handler_;

    QTcpSocket* socket_;
    QVector<QTcpSocket*> clientSockets_;
    QByteArray data_;
    //Размер входящего блока данных (quint16 - 2 байта до 65535)
    quint16 incomingDataSize_;
};

#endif // SERVER_H
