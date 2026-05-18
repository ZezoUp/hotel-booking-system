#include "room.h"

Room::Room()
    : roomNumber(0), beds(0), pricePerNight(0.0)
{
}

Room::Room(int roomNumber, int beds, double pricePerNight)
    : roomNumber(roomNumber), beds(beds), pricePerNight(pricePerNight)
{
}

int Room::getRoomNumber() const
{
    return roomNumber;
}

int Room::getBeds() const
{
    return beds;
}

double Room::getPricePerNight() const
{
    return pricePerNight;
}
