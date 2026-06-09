#pragma once
#include <string>
#include <vector>
#include <map>

struct UserAccount {
    std::string name;
    std::string email;
    std::string phone;
    std::string password; // Im echten Projekt würde man das hashen!
};

class UserManager {
private:
    std::vector<UserAccount> accounts;
public:
    // Registrieren — gibt false zurück wenn Email schon existiert
    bool registerUser(const std::string& name, const std::string& email,
                      const std::string& phone, const std::string& password);

    // Login — gibt nullptr zurück wenn falsch
    const UserAccount* login(const std::string& email, const std::string& password) const;

    // Prüfen ob Email schon registriert
    bool emailExists(const std::string& email) const;

    const std::vector<UserAccount>& getAllAccounts() const;
};
