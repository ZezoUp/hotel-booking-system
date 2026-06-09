
#include "Guest.h"

Guest::Guest(std::string name, std::string email, std::string phone)
    : name(name), email(email), phone(phone) {}

std::string Guest::getName()  const { return name; }
std::string Guest::getEmail() const { return email; }
std::string Guest::getPhone() const { return phone; }
void Guest::setEmail(std::string e) { email = e; }
void Guest::setPhone(std::string p) { phone = p; }
std::string Guest::getInfo() const {
    return "Name: " + name + " | Email: " + email + " | Tel: " + phone;
}
