#ifndef CONTROLLERWINDOWS_H
#define CONTROLLERWINDOWS_H

#include <QDialog>
#include "Chat.h"

namespace Ui {
  class ScreenController;
}

class ScreenController : public QDialog{
  Q_OBJECT

  public:
    explicit ScreenController(QWidget* parent = nullptr);
    ~ScreenController();

  public slots:
    void setLoginWindow();
    void setRegistrationWindow();
    void setChatWindow();
    void exit();

    void handleNetworkFailed();

  private:
    void setupSignals();

    Ui::ScreenController* ui;
    Client* client_;
};

#endif // CONTROLLERWINDOWS_H
