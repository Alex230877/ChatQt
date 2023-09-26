#include "Data.h"

namespace{
//Команды
enum Command{
    IS_PASSWORD_CORRECT,
    IS_USER_REGISTERED,
    ADD_USER,
    ADD_COMMON_MESSAGE,
    ADD_PRIVATE_MESSAGE,
    GET_COMMON_MESSAGE,
    GET_PRIVATE_MESSAGE,
    GET_USER_LOGINS,
    DELETE_USER
};
}

Data::Data(Database* database): database_(database) {}

QString Data::handle(int commandType, const QString& args)
{
    QString response;
    switch(commandType){
    case IS_PASSWORD_CORRECT:
        response = isPasswordCorrect(args);
        break;
    case IS_USER_REGISTERED:
        response = isUserRegistered(args);
        break;
    case ADD_USER:
        response = addUser(args);
        break;
    case ADD_COMMON_MESSAGE:
        response = addCommonMessage(args);
        break;
    case ADD_PRIVATE_MESSAGE:
        response = addPrivateMessage(args);
        break;
    case GET_COMMON_MESSAGE:
        response = getCommonMessage();
        break;
    case GET_PRIVATE_MESSAGE:
        response = getPrivateMessage(args);
        break;
    case GET_USER_LOGINS:
        response = getUserLogins();
        break;
    case DELETE_USER:
        response = deleteUser(args);
        break;
    default:
        break;
    }
    return response;
}

QString Data::isPasswordCorrect(const QString& args)
{
    //ЛОГИН|ХЭШПАРОЛЬ
    QStringList listArgs = args.split("|", Qt::KeepEmptyParts);
    const std::string login = listArgs.at(0).toStdString();
    const std::string passwordHash = listArgs.at(1).toStdString();

    auto isOk = database_->isPasswordCorrect(login, passwordHash);
    QString response = "false";
    if (isOk){
        response = "true";
    }
    return response;
}

QString Data::isUserRegistered(const QString& args)
{
    //ЛОГИН
    QStringList listArgs = args.split("|", Qt::KeepEmptyParts);
    const std::string login = listArgs.at(0).toStdString();
    auto isOk = database_->isUserRegistered(login);
    QString response = "false";
    if (isOk){
        response = "true";
    }
    return response;
}

QString Data::addUser(const QString& args)
{
    //ЛОГИН|ХЭШПАРОЛЬ
    QStringList listArgs = args.split("|", Qt::KeepEmptyParts);
    const std::string login = listArgs.at(0).toStdString();
    const std::string passwordHash = listArgs.at(1).toStdString();

    auto isOk = database_->addUser(login, passwordHash);
    QString response = "false";
    if (isOk){
        response = "true";
    }
    return response;
}

QString Data::addCommonMessage(const QString& args)
{
    //ОТПРАВИТЕЛЬ|ТЕКСТ СООБЩЕНИЯ
    QStringList listArgs = args.split("|", Qt::KeepEmptyParts);
    const std::string sender = listArgs.at(0).toStdString();
    const std::string message = listArgs.at(1).toStdString();

    auto isOk = database_->addCommonMessage(sender, message);
    QString response = "false";
    if (isOk){
        response = "true";
    }
    return response;
}

QString Data::addPrivateMessage(const QString& args)
{
    //ОТПРАВИТЕЛЬ|ПОЛУЧАТЕЛЬ|ТЕКСТ СООБЩЕНИЯ
    QStringList listArgs = args.split("|", Qt::KeepEmptyParts);
    const std::string sender = listArgs.at(0).toStdString();
    const std::string receiver = listArgs.at(1).toStdString();
    const std::string message = listArgs.at(2).toStdString();

    auto isOk = database_->addPrivateMessage(sender, receiver, message);
    QString response = "false";
    if (isOk){
        response = "true";
    }
    return response;
}

QString Data::getCommonMessage()
{
    auto messages = database_->getCommonMessages();
    auto response = QString::fromStdString(messages);
    return response;
}

QString Data::getPrivateMessage(const QString& args)
{
    //ПОЛУЧАТЕЛЬ
    QStringList listArgs = args.split("|", Qt::KeepEmptyParts);
    const std::string receiver = listArgs.at(0).toStdString();

    auto messages = database_->getPrivateMessages(receiver);
    auto response = QString::fromStdString(messages);
    return response;
}

QString Data::getUserLogins()
{
    auto users = database_->getLogins();
    QString response;
    for (const auto& user : users){
        response += QString::fromStdString(user) + "|";
    }
    return response;
}

QString Data::deleteUser(const QString& args)
{
    //ЛОГИН
    QStringList listArgs = args.split("|", Qt::KeepEmptyParts);
    const std::string login = listArgs.at(0).toStdString();

    auto isOk = database_->deleteUser(login);
    QString response = "false";
    if (isOk){
        response = "true";
    }

    return response;
}
