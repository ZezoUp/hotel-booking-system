#ifndef ROOM_H
#define ROOM_H

class Room
{
private:
    int roomNumber;
    int beds;
    double pricePerNight;

public:
    Room();
    Room(int roomNumber, int beds, double pricePerNight);

    int getRoomNumber() const;
    int getBeds() const;
    double getPricePerNight() const;
};

#endif // ROOM_H
