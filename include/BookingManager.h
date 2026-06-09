#pragma once
#include <vector>
#include <string>
#include <map>
#include "Hotel.h"
#include "Guest.h"
#include "Booking.h"

class BookingManager {
private:
    Hotel*               hotel;
    std::vector<Guest>   guests;
    std::vector<Booking> bookings;
    std::map<std::string, std::string> bookingCodes;
    int                  nextId;

    // Hilfsfunktion: Datum "YYYY-MM-DD" zu int YYYYMMDD für Vergleich
    static int dateToInt(const std::string& date);

public:
    BookingManager(Hotel* hotel);
    std::vector<Room*>    searchRooms(int persons);

    // Fix 4: Zimmer suchen mit Datum + exakt passende Betten
    std::vector<Room*>    searchRoomsForDates(int persons, const std::string& checkIn,
                                            const std::string& checkOut);
    // Fix 4: Prüfen ob Zimmer für Zeitraum frei (mit 5 Tage Puffer)
    bool                  isRoomAvailableForDates(int roomNumber, const std::string& checkIn,
                                 const std::string& checkOut);
    // Fix 4: Gibt aktive Buchung für ein Zimmer zurück (oder nullptr)
    Booking*              getActiveBookingForRoom(int roomNumber);

    Guest*                addGuest(std::string name, std::string email, std::string phone);
    // Fix 3: Gast löschen
    bool                  removeGuest(const std::string& name);
    std::vector<Guest*>   getAllGuests();
    int                   findGuestIndex(const std::string& name);
    Booking*              createBooking(int roomNumber, int guestIndex,
                           std::string checkIn, std::string checkOut, int nights);
    Booking*              createBookingWithId(std::string id, int roomNumber, int guestIndex,
                                 std::string checkIn, std::string checkOut, int nights);
    bool                  cancelBooking(std::string bookingId);
    std::vector<Booking*> getAllBookings();
    std::vector<Booking*> getBookingsForGuest(const std::string& guestName);
    void                  setBookingCode(const std::string& bookingId, const std::string& code);
    std::string           getBookingCode(const std::string& bookingId) const;
    const std::map<std::string,std::string>& getAllCodes() const;
};