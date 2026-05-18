#ifndef BOOKING_H
#define BOOKING_H

#include <QString>
#include <QDate>

class Booking
{
private:
    QString guestName;
    int roomNumber;
    QDate startDate;
    QDate endDate;
    double totalPrice;

public:
    Booking();
    Booking(const QString& guestName, int roomNumber, const QDate& startDate, const QDate& endDate, double totalPrice);

    QString getGuestName() const;
    int getRoomNumber() const;
    QDate getStartDate() const;
    QDate getEndDate() const;
    double getTotalPrice() const;
};

#endif // BOOKING_H
