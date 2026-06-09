#pragma once
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QStackedWidget>
#include <string>
#include "UserManager.h"

enum class UserRole { Admin, Guest };

class LoginDialog : public QDialog {
    Q_OBJECT
public:
    explicit LoginDialog(UserManager* userManager, QWidget* parent = nullptr);
    UserRole    getRole()      const;
    std::string getGuestName() const;
    std::string getGuestEmail() const;

private slots:
    void onLogin();
    void onRegister();
    void showLoginPage();
    void showRegisterPage();

private:
    UserManager* userManager;
    UserRole     role;
    std::string  guestName;
    std::string  guestEmail;

    QStackedWidget* stack;

    // Login Seite
    QWidget*     loginPage;
    QLineEdit*   loginEmail;
    QLineEdit*   loginPassword;
    QLabel*      loginError;

    // Registrier Seite
    QWidget*     registerPage;
    QLineEdit*   regName;
    QLineEdit*   regEmail;
    QLineEdit*   regPhone;
    QLineEdit*   regPassword;
    QLineEdit*   regPasswordConfirm;
    QLabel*      regError;

    QWidget* createLoginPage();
    QWidget* createRegisterPage();
};
