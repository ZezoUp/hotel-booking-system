#include "hotelbookingsystem.h"

void HotelBookingSystem::addRoom(const Room& room)
{
    rooms.append(room);
}

const Room* HotelBookingSystem::findRoom(int roomNumber) const
{
    for (const Room& room : rooms) {
        if (room.getRoomNumber() == roomNumber) {
            return &room;
        }
    }
    return nullptr;
}

QVector<Room> HotelBookingSystem::searchAvailableRooms(int persons, const QDate& startDate, const QDate& endDate) const
{
    QVector<Room> result;

    if (!startDate.isValid() || !endDate.isValid() || startDate >= endDate) {
        return result;
    }

    for (const Room& room : rooms) {
        if (room.getBeds() >= persons && isRoomAvailable(room.getRoomNumber(), startDate, endDate)) {
            result.append(room);
        }
    }

    return result;
}

bool HotelBookingSystem::bookRoom(const QString& guestName, int roomNumber, const QDate& startDate, const QDate& endDate)
{
    if (guestName.trimmed().isEmpty()) {
        return false;
    }

    if (!startDate.isValid() || !endDate.isValid() || startDate >= endDate) {
        return false;
    }

    const Room* room = findRoom(roomNumber);
    if (room == nullptr) {
        return false;
    }

    if (!isRoomAvailable(roomNumber, startDate, endDate)) {
        return false;
    }

    double totalPrice = calculatePrice(roomNumber, startDate, endDate);
    bookings.append(Booking(guestName.trimmed(), roomNumber, startDate, endDate, totalPrice));
    return true;
}

bool HotelBookingSystem::isRoomAvailable(int roomNumber, const QDate& startDate, const QDate& endDate) const
{
    for (const Booking& booking : bookings) {
        if (booking.getRoomNumber() == roomNumber) {
            bool overlaps = startDate < booking.getEndDate() && endDate > booking.getStartDate();
            if (overlaps) {
                return false;
            }
        }
    }

    return true;
}

double HotelBookingSystem::calculatePrice(int roomNumber, const QDate& startDate, const QDate& endDate) const
{
    const Room* room = findRoom(roomNumber);
    if (room == nullptr || startDate >= endDate) {
        return 0.0;
    }

    int nights = startDate.daysTo(endDate);
    return nights * room->getPricePerNight();
}

QVector<Booking> HotelBookingSystem::getBookings() const
{
    return bookings;
}
