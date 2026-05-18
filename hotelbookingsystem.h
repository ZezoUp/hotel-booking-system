#ifndef HOTELBOOKINGSYSTEM_H
#define HOTELBOOKINGSYSTEM_H

#include <QVector>
#include <QDate>
#include <QString>
#include "room.h"
#include "booking.h"

class HotelBookingSystem
{
private:
    QVector<Room> rooms;
    QVector<Booking> bookings;

    const Room* findRoom(int roomNumber) const;

public:
    void addRoom(const Room& room);
    QVector<Room> searchAvailableRooms(int persons, const QDate& startDate, const QDate& endDate) const;
    bool bookRoom(const QString& guestName, int roomNumber, const QDate& startDate, const QDate& endDate);
    bool isRoomAvailable(int roomNumber, const QDate& startDate, const QDate& endDate) const;
    double calculatePrice(int roomNumber, const QDate& startDate, const QDate& endDate) const;
    QVector<Booking> getBookings() const;
};

#endif // HOTELBOOKINGSYSTEM_H
