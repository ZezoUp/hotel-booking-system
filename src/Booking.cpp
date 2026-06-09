#include "Booking.h"

Booking::Booking(std::string id, Room* room, Guest* guest,
                 std::string checkIn, std::string checkOut, int nights)
    : bookingId(id), room(room), guest(guest),
    checkIn(checkIn), checkOut(checkOut), nights(nights),
    totalPrice(room->calculatePrice(nights)), cancelled(false) {
    // Fix 4: Zimmer NICHT mehr auf unavailable setzen
    // Verfügbarkeit wird jetzt über Datum-Logik geprüft
}

std::string Booking::getBookingId()  const { return bookingId; }
Room*       Booking::getRoom()       const { return room; }
Guest*      Booking::getGuest()      const { return guest; }
std::string Booking::getCheckIn()    const { return checkIn; }
std::string Booking::getCheckOut()   const { return checkOut; }
int         Booking::getNights()     const { return nights; }
double      Booking::getTotalPrice() const { return totalPrice; }
bool        Booking::isCancelled()   const { return cancelled; }

void Booking::cancel() {
    cancelled = true;
    // Fix 4: Zimmer wieder freigeben nicht mehr nötig
}

std::string Booking::getInfo() const {
    return "ID: " + bookingId + " | Gast: " + guest->getName() +
           " | Zimmer: " + std::to_string(room->getRoomNumber()) +
           " | " + checkIn + " bis " + checkOut +
           " | " + std::to_string((int)totalPrice) + " EUR" +
           " | " + (cancelled ? "Storniert" : "Aktiv");
}