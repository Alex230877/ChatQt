#ifndef CHAT_H
#define CHAT_H

#include <QTcpSocket>

class Client: public QTcpSocket {
  Q_OBJECT

  public:
    Client() = delete;
    explicit Client(QString hostIP, int port);

    void requestIsPasswordCorrect(const QString& login,
                                  const QString& passwordHash);
    void requestIsUserRegistered(const QString& login);
    void requestAddUser(const QString& login,
                        const QString& passwordHash);
    void requestUserLogins();
    void requestCommonMessage();
    void requestPrivateMessage(const QString& receiver);
    void sendCommonMessage(const QString& sender,
                           const QString& message);
    void sendPrivateMessage(const QString& sender,
                            const QString& receiver,
                            const QString& message);
    void requestRemoveUser(const QString& login);
    QString getResponse();

  signals:
    void dataReceived();
    void failConnection();

  private:
    void sendRequest(int commandType, const QString& args);
    void getData();

    QString response_; //Ответ сервера
    QTcpSocket* socket_; //Подключение к сети
    QByteArray data_; //Данные на сервер
    quint16 nextBlockSize_; //Размер входящего блока данных (2 байта)
};

#endif // CHAT_H
