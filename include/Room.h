#pragma once
#include <string>

class Room {
private:
    int    roomNumber;
    int    beds;
    double pricePerNight;
    bool   available;
public:
    Room(int roomNumber, int beds, double pricePerNight);
    int    getRoomNumber()     const;
    int    getBeds()           const;
    double getPricePerNight()  const;
    bool   isAvailable()       const;
    void   setAvailable(bool a);
    double calculatePrice(int nights) const;
    std::string getInfo() const;
};
