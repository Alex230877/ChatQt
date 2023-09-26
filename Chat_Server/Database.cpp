#include "Database.h"
#include <QtDebug>

#define HOSTNAME "localhost"
#define DATABASENAME "postgres"
#define USERNAME "postgres"
#define PASSWORD "123456"

namespace {
//Имя получателя сообщений для обычных сообщений
const std::string ALL = "ALL";
}

Database::Database(): database_(QSqlDatabase::addDatabase("QPSQL"))
{
    //Настройки базы
    database_.setDatabaseName(DATABASENAME);
    database_.setHostName(HOSTNAME);
    database_.setUserName(USERNAME);
    database_.setPassword(PASSWORD);
    if (database_.open()){
        qDebug() << "Database is opened";
    }
    else{
        qDebug() << database_.lastError().text();
        exit(1);
    }

    query_ = new QSqlQuery(database_);
    query_->exec("CREATE TABLE Users(Login TEXT, PasswordHash TEXT, Ban BOOL);");
    query_->exec("CREATE TABLE Messages(Sender TEXT, Receiver TEXT, Message TEXT);");
}

bool Database::isPasswordCorrect(const std::string& login, const std::string& passwordHash) const
{
    if (login.empty() || passwordHash.empty()){
        return false;
    }
    if (!isUserRegistered(login)){
        return false;
    }

    //Получить хэш пароля из базы данных
    auto request = "Select PasswordHash from Users WHERE Login = '" +
                   QString::fromStdString(login) + "';";
    query_->exec(request);

    QString databasePasswordHash;
    while (query_->next()) {
        databasePasswordHash = query_->value(0).toString();
    }

    //Неверный пароль
    if (databasePasswordHash != QString::fromStdString(passwordHash) ){
        return false;
    }

    return true;
}

bool Database::isUserRegistered(const std::string& login) const
{
    if (login.empty()){
        return false;
    }

    auto request = "Select * from Users WHERE Login = '" +
                   QString::fromStdString(login) + "';";
    query_->exec(request);
    //User not found
    if (!query_->next()){
        return false;
    }

    return true;
}

bool Database::addUser(const std::string& login, const std::string& passwordHash)
{
    if (login.empty() || passwordHash.empty()){
        return false;
    }

    //Пользователь уже существует
    if (isUserRegistered(login)) {
        return false;
    }

    //Добавление пользователя выполнено
    auto request = "INSERT INTO Users(Login, PasswordHash, Ban) VALUES('" +
                   QString::fromStdString(login) + "', '" +
                   QString::fromStdString(passwordHash) + "', false);";
    query_->exec(request);

    emit updateDatabase();

    return true;
}

bool Database::addCommonMessage(const std::string& sender, const std::string& message)
{
    if (message.empty()){
        return false;
    }

    //Пользователь не найден
    if (!isUserRegistered(sender)) {
        return false;
    }

    //Отправитель заблокирован
    if (getIsBanned(sender)){
        return false;
    }

    //Добавление сообщения выполнено
    auto request = "INSERT INTO Messages(Sender, Receiver, Message) VALUES('" +
                   QString::fromStdString(sender) + "', '" +
                   QString::fromStdString(ALL) + "', '" +
                   QString::fromStdString(message) + "');";
    query_->exec(request);

    emit updateDatabase();

    return true;
}

bool Database::addPrivateMessage(const std::string& sender, const std::string& receiver, const std::string& message)
{
    if (message.empty()){
        return false;
    }

    //Отправитель не найден
    if (!isUserRegistered(sender)) {
        return false;
    }

    //Получатель не найден
    if (!isUserRegistered(receiver)) {
        return false;
    }

    //Отправитель заблокирован
    if (getIsBanned(sender)){
        return false;
    }

    //Добавление сообщения выполнено
    auto request = "INSERT INTO Messages(Sender, Receiver, Message) VALUES('" +
                   QString::fromStdString(sender) + "', '" +
                   QString::fromStdString(receiver) + "', '" +
                   QString::fromStdString(message) + "');";
    query_->exec(request);

    emit updateDatabase();

    return true;
}

std::string Database::getCommonMessages() const
{
    auto request = "Select * from Messages WHERE Receiver = '" +
                   QString::fromStdString(ALL) + "';";
    query_->exec(request);

    std::string messages;
    while (query_->next()) {
        auto sender = query_->value(0).toString();
        auto receiver = query_->value(1).toString();
        auto text = query_->value(2).toString();

        auto line = "<" + sender + ">: " + text + "\n";
        messages.append(line.toStdString());
    }
    return messages;
}

std::string Database::getPrivateMessages(const std::string& receiver) const
{
    //Отправленные и полученные личные сообщения, за исключением сообщений всем пользователям.
    auto request = "Select * from Messages WHERE Receiver = '" +
                   QString::fromStdString(receiver) + "' OR Sender = '" +
                   QString::fromStdString(receiver) + "' AND Receiver NOT IN ('" +
                   QString::fromStdString(ALL) + "');";

    query_->exec(request);

    std::string messages;
    while (query_->next()) {
        auto sender = query_->value(0).toString();
        auto addressee = query_->value(1).toString();
        auto text = query_->value(2).toString();

        auto line = "<" + sender + "> -> <" + addressee + ">: " + text + "\n";
        messages.append(line.toStdString());
    }

    return messages;
}

std::list<std::string> Database::getLogins() const
{
    query_->exec("Select * from Users;");

    std::list<std::string> logins;
    while (query_->next()) {
        auto login = query_->value(0).toString();
        logins.push_back(login.toStdString());
    }
    return logins;
}

bool Database::deleteUser(const std::string& login)
{
    if (login.empty()){
        return false;
    }

    //Пользователь не найден
    if (!isUserRegistered(login)) {
        return false;
    }

    //Удалить личные сообщения пользователю
    auto request = "DELETE FROM Messages WHERE Receiver = '" +
                   QString::fromStdString(login) + "';";
    query_->exec(request);

    //Удалить пользователя
    request.clear();
    request = "DELETE FROM Users WHERE Login = '" +
              QString::fromStdString(login) + "';";
    query_->exec(request);

    emit updateDatabase();

    return true;
}

bool Database::deleteMessage(const std::string& message)
{
    if (message.empty()){
        return false;
    }

    auto request = "DELETE FROM Messages WHERE Message = '" +
                   QString::fromStdString(message) + "';";
    query_->exec(request);

    emit updateDatabase();

    return true;
}

void Database::switchBanUser(const std::string& login)
{
    if(getIsBanned(login)){
        //Unban
        auto request = "UPDATE Users SET Ban = false WHERE Login = '" +
                       QString::fromStdString(login) + "';";
        query_->exec(request);
    }
    else{
        //Ban
        auto request = "UPDATE Users SET Ban = true WHERE Login = '" +
                       QString::fromStdString(login) + "';";
        query_->exec(request);
    }

    emit updateDatabase();
}

QSqlDatabase* Database::getDatabase()
{
    return &database_;
}

bool Database::getIsBanned(const std::string& login) const
{
    if (login.empty()){
        return true;
    }
    if (!isUserRegistered(login)){
        return true;
    }

    auto request = "Select Ban from Users WHERE Login = '" +
                   QString::fromStdString(login) + "';";
    query_->exec(request);

    bool isBanned = true;
    while (query_->next()) {
        isBanned = query_->value(0).toBool();
    }
    return isBanned;
}
