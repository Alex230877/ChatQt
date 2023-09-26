Cетевой чат, имеющий клиент-серверную архитектуру, с GUI посредством Qt, хранение информации осуществлео в базе данных.
Настройки сервера заданы внутри исходного кода.
Проект выполнен в Qt Creator 11.0.2 (Based on Qt 6.4.2 (MSVC 2019, x86_64)), используемая база данных PostgresSQL 15.

Сервер
- обработка запросов клиентов
- обращение к Базе данных
- удаление пользователей
- удаление сообщений
- блокировка / разблокировка пользователей (при блокировке нет возможности отправлять сообщения)

Клиент
- вход в чат по логину и паролю
- регистрация пользователей - логин, пароль
- отправка сообщений конкретному пользователю или всем сразу
- выход из чата 
- удаление аккаунта пользователя
