#include "Server.h"
#include "Data.h"

Server::Server(uint16_t port, Database* database)
{
    this->listen(QHostAddress::Any, port);
    incomingDataSize_ = 0;
    socket_ = new QTcpSocket;
    handler_ = new Data(database);
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    socket_->setSocketDescriptor(socketDescriptor);
    connect(socket_, &QTcpSocket::readyRead, this, &Server::readData);
    connect(socket_, &QTcpSocket::disconnected, socket_, &QTcpSocket::deleteLater);
    clientSockets_.push_back(socket_);
}

void Server::readData()
{
    socket_ = (QTcpSocket*) sender();
    QDataStream input(socket_);
    input.setVersion(QDataStream::Qt_5_6);
    if (input.status() == QDataStream::Ok){
        for(;;){
            if (incomingDataSize_ == 0){
                if (socket_->bytesAvailable() < 2){
                    break;
                }
                input >> incomingDataSize_;
            }
            if (socket_->bytesAvailable() < incomingDataSize_){
                break;
            }
            QString commandArgs;
            int commandType;
            input >> commandType >> commandArgs;

            //Обработать запрос
            auto response = handler_->handle(commandType, commandArgs);
            //Отправить ответ
            sendToClient(socket_, response);

            incomingDataSize_ = 0;
            break;
        }
    }
}

void Server::sendToClient(QTcpSocket* socketClient, const QString& string)
{
    data_.clear();
    QDataStream output(&data_, QIODevice::WriteOnly);
    output.setVersion(QDataStream::Qt_5_6);
    //Очистить первые 2 байта
    output << quint16(0) << string;
    output.device()->seek(0);  //Установить указатель на начало сообщения
    //Рассчитать размер сообщения без первых 2 байтов
    output << quint16(data_.size() - sizeof(quint16));
    socketClient->write(data_);
}
