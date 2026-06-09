#include "LoginDialog.h"

LoginDialog::LoginDialog(UserManager* userManager, QWidget* parent)
    : QDialog(parent), userManager(userManager) {
    setWindowTitle("Hotel Stuttgart — Login");
    setFixedSize(500, 500);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->setContentsMargins(0,0,0,0);

    stack = new QStackedWidget();
    stack->addWidget(createLoginPage());
    stack->addWidget(createRegisterPage());
    stack->setCurrentIndex(0);

    layout->addWidget(stack);
}

// ── Login Seite ──
QWidget* LoginDialog::createLoginPage() {
    loginPage = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(loginPage);
    layout->setSpacing(12);
    layout->setContentsMargins(30,30,30,30);

    QLabel* title = new QLabel("Hotel Stuttgart");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size:22px; font-weight:bold; color:#1a3c5e;");
    layout->addWidget(title);

    QLabel* sub = new QLabel("Einloggen");
    sub->setAlignment(Qt::AlignCenter);
    sub->setStyleSheet("font-size:13px; color:gray;");
    layout->addWidget(sub);

    QGroupBox* box = new QGroupBox();
    box->setStyleSheet("QGroupBox{border:1px solid #ccc; border-radius:8px; padding:12px;}");
    QVBoxLayout* bl = new QVBoxLayout(box);

    loginEmail    = new QLineEdit(); loginEmail->setPlaceholderText("Email");
    loginEmail->setFixedHeight(36);
    loginEmail->setStyleSheet("border:1px solid #ccc; border-radius:4px; padding:4px 8px; font-size:14px; background:white; color:black;");

    loginPassword = new QLineEdit(); loginPassword->setPlaceholderText("Passwort");
    loginPassword->setEchoMode(QLineEdit::Password);
    loginPassword->setFixedHeight(36);
    loginPassword->setStyleSheet("border:1px solid #ccc; border-radius:4px; padding:4px 8px; font-size:14px; background:white; color:black;");

    bl->addWidget(new QLabel("Email:"));   bl->addWidget(loginEmail);
    bl->addWidget(new QLabel("Passwort:")); bl->addWidget(loginPassword);
    layout->addWidget(box);

    loginError = new QLabel("");
    loginError->setAlignment(Qt::AlignCenter);
    loginError->setStyleSheet("color:red; font-size:12px;");
    loginError->setWordWrap(true);
    layout->addWidget(loginError);

    QPushButton* btnLogin = new QPushButton("Einloggen");
    btnLogin->setFixedHeight(40);
    btnLogin->setStyleSheet("QPushButton{background:#1a3c5e; color:white; font-size:14px; font-weight:bold; border-radius:6px;}"
                            "QPushButton:hover{background:#245a8a;}");
    layout->addWidget(btnLogin);

    // Trennlinie
    QLabel* sep = new QLabel("─────────────────────────");
    sep->setAlignment(Qt::AlignCenter);
    sep->setStyleSheet("color:#ccc;");
    layout->addWidget(sep);

    // Hinweis Admin
    QLabel* adminHint = new QLabel("Admin: admin@hotel.de / admin123");
    adminHint->setAlignment(Qt::AlignCenter);
    adminHint->setStyleSheet("color:#aaa; font-size:11px;");
    layout->addWidget(adminHint);

    QPushButton* btnToRegister = new QPushButton("Noch kein Konto? Jetzt registrieren");
    btnToRegister->setStyleSheet("QPushButton{background:transparent; color:#1a3c5e; font-size:12px; border:none; text-decoration:underline;}"
                                 "QPushButton:hover{color:#245a8a;}");
    layout->addWidget(btnToRegister);

    connect(btnLogin,      &QPushButton::clicked,      this, &LoginDialog::onLogin);
    connect(loginPassword, &QLineEdit::returnPressed,  this, &LoginDialog::onLogin);
    connect(btnToRegister, &QPushButton::clicked,      this, &LoginDialog::showRegisterPage);
    return loginPage;
}

// ── Registrier Seite ──
QWidget* LoginDialog::createRegisterPage() {
    registerPage = new QWidget();
    QVBoxLayout* layout = new QVBoxLayout(registerPage);
    layout->setSpacing(8);
    layout->setContentsMargins(30,20,30,20);

    QLabel* title = new QLabel("Neues Konto erstellen");
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("font-size:18px; font-weight:bold; color:#1a3c5e;");
    layout->addWidget(title);

    QGroupBox* box = new QGroupBox();
    box->setStyleSheet("QGroupBox{border:1px solid #ccc; border-radius:8px; padding:10px;}");
    QVBoxLayout* bl = new QVBoxLayout(box);

    QString fieldStyle = "border:1px solid #ccc; border-radius:4px; padding:4px 8px; font-size:13px; background:white; color:black;";

    regName = new QLineEdit(); regName->setPlaceholderText("Vor- und Nachname");
    regName->setFixedHeight(34); regName->setStyleSheet(fieldStyle);

    regEmail = new QLineEdit(); regEmail->setPlaceholderText("Email-Adresse");
    regEmail->setFixedHeight(34); regEmail->setStyleSheet(fieldStyle);

    regPhone = new QLineEdit(); regPhone->setPlaceholderText("Telefonnummer (optional)");
    regPhone->setFixedHeight(34); regPhone->setStyleSheet(fieldStyle);

    regPassword = new QLineEdit(); regPassword->setPlaceholderText("Passwort (mind. 4 Zeichen)");
    regPassword->setEchoMode(QLineEdit::Password);
    regPassword->setFixedHeight(34); regPassword->setStyleSheet(fieldStyle);

    regPasswordConfirm = new QLineEdit(); regPasswordConfirm->setPlaceholderText("Passwort wiederholen");
    regPasswordConfirm->setEchoMode(QLineEdit::Password);
    regPasswordConfirm->setFixedHeight(34); regPasswordConfirm->setStyleSheet(fieldStyle);

    bl->addWidget(new QLabel("Name:*"));       bl->addWidget(regName);
    bl->addWidget(new QLabel("Email:*"));       bl->addWidget(regEmail);
    bl->addWidget(new QLabel("Telefon:"));      bl->addWidget(regPhone);
    bl->addWidget(new QLabel("Passwort:*"));    bl->addWidget(regPassword);
    bl->addWidget(new QLabel("Wiederholen:*")); bl->addWidget(regPasswordConfirm);
    layout->addWidget(box);

    regError = new QLabel("");
    regError->setAlignment(Qt::AlignCenter);
    regError->setStyleSheet("color:red; font-size:12px;");
    regError->setWordWrap(true);
    layout->addWidget(regError);

    QPushButton* btnRegister = new QPushButton("Registrieren");
    btnRegister->setFixedHeight(38);
    btnRegister->setStyleSheet("QPushButton{background:#1a6e3c; color:white; font-size:14px; font-weight:bold; border-radius:6px;}"
                               "QPushButton:hover{background:#228a4d;}");
    layout->addWidget(btnRegister);

    QPushButton* btnBack = new QPushButton("Zurueck zum Login");
    btnBack->setStyleSheet("QPushButton{background:transparent; color:#1a3c5e; font-size:12px; border:none; text-decoration:underline;}"
                           "QPushButton:hover{color:#245a8a;}");
    layout->addWidget(btnBack);

    connect(btnRegister, &QPushButton::clicked, this, &LoginDialog::onRegister);
    connect(btnBack,     &QPushButton::clicked, this, &LoginDialog::showLoginPage);
    return registerPage;
}

void LoginDialog::showLoginPage()    { stack->setCurrentIndex(0); }
void LoginDialog::showRegisterPage() { stack->setCurrentIndex(1); }

void LoginDialog::onLogin() {
    QString email = loginEmail->text().trimmed();
    QString pass  = loginPassword->text();

    if (email.isEmpty() || pass.isEmpty()) {
        loginError->setText("Bitte Email und Passwort eingeben!"); return;
    }

    // Admin Check
    if (email == "admin@hotel.de" && pass == "admin123") {
        role = UserRole::Admin;
        accept(); return;
    }

    // Gast Check
    const UserAccount* acc = userManager->login(email.toStdString(), pass.toStdString());
    if (acc) {
        role       = UserRole::Guest;
        guestName  = acc->name;
        guestEmail = acc->email;
        accept();
    } else {
        loginError->setText("Falsche Email oder falsches Passwort!");
        loginPassword->clear();
    }
}

void LoginDialog::onRegister() {
    QString name  = regName->text().trimmed();
    QString email = regEmail->text().trimmed().toLower();
    QString phone = regPhone->text().trimmed();
    QString pass  = regPassword->text();
    QString pass2 = regPasswordConfirm->text();

    // Validierung
    if (name.isEmpty() || email.isEmpty() || pass.isEmpty()) {
        regError->setText("Name, Email und Passwort sind Pflichtfelder!"); return;
    }
    if (!email.contains("@") || !email.contains(".")) {
        regError->setText("Bitte eine gueltige Email eingeben!"); return;
    }
    if (pass.length() < 4) {
        regError->setText("Passwort muss mindestens 4 Zeichen haben!"); return;
    }
    if (pass != pass2) {
        regError->setText("Passwoerter stimmen nicht ueberein!"); return;
    }
    if (userManager->emailExists(email.toStdString())) {
        regError->setText("Diese Email ist bereits registriert!"); return;
    }

    // Registrieren
    userManager->registerUser(name.toStdString(), email.toStdString(),
                              phone.toStdString(), pass.toStdString());

    // Direkt einloggen
    role       = UserRole::Guest;
    guestName  = name.toStdString();
    guestEmail = email.toStdString();
    accept();
}

UserRole    LoginDialog::getRole()       const { return role; }
std::string LoginDialog::getGuestName()  const { return guestName; }
std::string LoginDialog::getGuestEmail() const { return guestEmail; }