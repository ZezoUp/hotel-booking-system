#pragma once
#include <string>
#include <vector>
#include "Room.h"

class Hotel {
private:
    std::string       name;
    std::string       address;
    std::vector<Room> rooms;
public:
    Hotel(std::string name, std::string address);
    void               addRoom(int number, int beds, double price);
    std::vector<Room*> getAvailableRooms(int persons);
    std::vector<Room*> getAllRooms();
    Room*              getRoomByNumber(int number);
    std::string        getName()    const;
    std::string        getAddress() const;
};
