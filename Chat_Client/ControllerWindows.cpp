#include "ControllerWindows.h"
#include "ui_ControllerWindows.h"
#include <QtDebug>
#include <QMessageBox>
#include "SHA.h"

namespace{
  enum{
    LOGIN_WINDOW = 0,
    REGISTRATION_WINDOW,
    CHAT_WINDOW
  };
}

ScreenController::ScreenController(QWidget* parent):
  QDialog(parent), ui(new Ui::ScreenController)
{
  ui->setupUi(this);

  //Настройка сети
  client_ = new Client("127.0.0.1", 7777);

  ui->loginWindow->setClient(client_);
  ui->registrationWindow->setClient(client_);
  ui->chatWindow->setClient(client_);

  //Установка стартового окна
  setLoginWindow();

  setupSignals();
}

ScreenController::~ScreenController()
{
  delete ui;
  qApp->exit(0);
}

void ScreenController::setLoginWindow()
{
  ui->stackedWidget->setCurrentIndex(LOGIN_WINDOW);
}

void ScreenController::setRegistrationWindow()
{
  ui->stackedWidget->setCurrentIndex(REGISTRATION_WINDOW);
}

void ScreenController::setChatWindow()
{
  ui->stackedWidget->setCurrentIndex(CHAT_WINDOW);
  ui->chatWindow->connectNetworkSignal_UpdateMessage();
}

void ScreenController::exit()
{
  this->close();
}

void ScreenController::handleNetworkFailed()
{
  QString message = "Server connection failed. Contact the server administrator or try to connect later.";
  QMessageBox::critical(this, "Error", message);
  this->close();
}

void ScreenController::setupSignals()
{ 
  connect(client_, &Client::failConnection,
        this, &ScreenController::handleNetworkFailed);

  //Закрыть приложение
  connect(ui->loginWindow, &LoginWindow::close,
        this, &ScreenController::exit);

  connect(ui->registrationWindow, &RegistrationWindow::close,
        this, &ScreenController::exit);

  //Переходы окон

  //Переходы из окна логина
  connect(ui->loginWindow, &LoginWindow::toRegistration,
          this, &ScreenController::setRegistrationWindow);

  connect(ui->loginWindow, &LoginWindow::toChat,
          this, &ScreenController::setChatWindow);

  connect(ui->loginWindow, &LoginWindow::toChat,
          ui->chatWindow, &ChatWindow::setUserLogin);

  //Переходы из окна регистрации

  connect(ui->registrationWindow, &RegistrationWindow::toLogin,
          this, &ScreenController::setLoginWindow);
  connect(ui->registrationWindow, &RegistrationWindow::toChat,
          this, &ScreenController::setChatWindow);

  connect(ui->registrationWindow, &RegistrationWindow::toChat,
          ui->chatWindow, &ChatWindow::setUserLogin);

  //Переходы из окна чата
  connect(ui->chatWindow, &ChatWindow::toLogin,
          this, &ScreenController::setLoginWindow);
}
