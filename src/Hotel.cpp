#include "Hotel.h"

Hotel::Hotel(std::string name, std::string address)
    : name(name), address(address) {}

void Hotel::addRoom(int number, int beds, double price) {
    rooms.push_back(Room(number, beds, price));
}

std::vector<Room*> Hotel::getAvailableRooms(int persons) {
    std::vector<Room*> result;
    for (auto& r : rooms)
        if (r.isAvailable() && r.getBeds() >= persons)
            result.push_back(&r);
    return result;
}

std::vector<Room*> Hotel::getAllRooms() {
    std::vector<Room*> result;
    for (auto& r : rooms) result.push_back(&r);
    return result;
}

Room* Hotel::getRoomByNumber(int number) {
    for (auto& r : rooms)
        if (r.getRoomNumber() == number) return &r;
    return nullptr;
}

std::string Hotel::getName()    const { return name; }
std::string Hotel::getAddress() const { return address; }
