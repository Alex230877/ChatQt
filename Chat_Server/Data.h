#ifndef DATA_H
#define DATA_H

#include <QString>
#include "Database.h"

class Data{
public:
    Data() = delete;
    explicit Data(Database* database);

    /**
    Request
    \param[in] commandType Command type
    \param[in] args Command arguments
    \return Response to client
    */
    QString handle(int commandType, const QString& args);

private:
    QString isPasswordCorrect(const QString& args);
    QString isUserRegistered(const QString& args);
    QString addUser(const QString& args);
    QString addCommonMessage(const QString& args);
    QString addPrivateMessage(const QString& args);
    QString getCommonMessage();
    QString getPrivateMessage(const QString& args);
    QString getUserLogins();
    QString deleteUser(const QString& args);

    Database* database_;
};

#endif // DATA_H
