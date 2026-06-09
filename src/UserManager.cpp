#include "UserManager.h"

bool UserManager::registerUser(const std::string& name, const std::string& email,
                                const std::string& phone, const std::string& password) {
    if (emailExists(email)) return false; // Email schon vergeben
    accounts.push_back({name, email, phone, password});
    return true;
}

const UserAccount* UserManager::login(const std::string& email, const std::string& password) const {
    for (const auto& acc : accounts)
        if (acc.email == email && acc.password == password)
            return &acc;
    return nullptr;
}

bool UserManager::emailExists(const std::string& email) const {
    for (const auto& acc : accounts)
        if (acc.email == email) return true;
    return false;
}

const std::vector<UserAccount>& UserManager::getAllAccounts() const {
    return accounts;
}
