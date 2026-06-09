#pragma once
#include <string>

class Guest {
private:
    std::string name;
    std::string email;
    std::string phone;
public:
    Guest(std::string name, std::string email, std::string phone);
    std::string getName()  const;
    std::string getEmail() const;
    std::string getPhone() const;
    void setEmail(std::string e);
    void setPhone(std::string p);
    std::string getInfo() const;
};
