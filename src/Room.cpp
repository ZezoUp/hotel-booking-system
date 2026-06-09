#include "Room.h"

Room::Room(int roomNumber, int beds, double pricePerNight)
    : roomNumber(roomNumber), beds(beds), pricePerNight(pricePerNight), available(true) {}

int    Room::getRoomNumber()    const { return roomNumber; }
int    Room::getBeds()          const { return beds; }
double Room::getPricePerNight() const { return pricePerNight; }
bool   Room::isAvailable()      const { return available; }
void   Room::setAvailable(bool a)     { available = a; }
double Room::calculatePrice(int nights) const { return pricePerNight * nights; }

std::string Room::getInfo() const {
    return "Zimmer " + std::to_string(roomNumber) +
           " | Betten: " + std::to_string(beds) +
           " | Preis: " + std::to_string((int)pricePerNight) + " EUR/Nacht" +
           " | " + (available ? "Frei" : "Belegt");
}
