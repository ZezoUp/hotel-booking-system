#include "booking.h"

Booking::Booking()
    : guestName(""), roomNumber(0), totalPrice(0.0)
{
}

Booking::Booking(const QString& guestName, int roomNumber, const QDate& startDate, const QDate& endDate, double totalPrice)
    : guestName(guestName), roomNumber(roomNumber), startDate(startDate), endDate(endDate), totalPrice(totalPrice)
{
}

QString Booking::getGuestName() const
{
    return guestName;
}

int Booking::getRoomNumber() const
{
    return roomNumber;
}

QDate Booking::getStartDate() const
{
    return startDate;
}

QDate Booking::getEndDate() const
{
    return endDate;
}

double Booking::getTotalPrice() const
{
    return totalPrice;
}
