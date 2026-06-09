#include "BookingManager.h"
#include <algorithm>

BookingManager::BookingManager(Hotel* hotel) : hotel(hotel), nextId(1) {}

// Datum "YYYY-MM-DD" -> int z.B. 20260701
int BookingManager::dateToInt(const std::string& date) {
    if (date.size() < 10) return 0;
    try {
        std::string clean = date.substr(0,4) + date.substr(5,2) + date.substr(8,2);
        return std::stoi(clean);
    } catch(...) { return 0; }
}

// Fix 4: Datum + 5 Tage addieren (einfache Implementierung)
static std::string addDays(const std::string& date, int days) {
    // Wir nutzen einen einfachen Trick: QDate wäre ideal aber wir sind in .cpp ohne Qt
    // Stattdessen: direkt Tag addieren (funktioniert für kleine Werte)
    if (date.size() < 10) return date;
    int y = std::stoi(date.substr(0,4));
    int m = std::stoi(date.substr(5,2));
    int d = std::stoi(date.substr(8,2));
    int daysInMonth[] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    // Schaltjahr
    if ((y%4==0 && y%100!=0) || y%400==0) daysInMonth[2]=29;
    d += days;
    while (d > daysInMonth[m]) {
        d -= daysInMonth[m];
        m++;
        if (m > 12) { m=1; y++; if((y%4==0&&y%100!=0)||y%400==0) daysInMonth[2]=29; else daysInMonth[2]=28; }
    }
    char buf[11];
    snprintf(buf, sizeof(buf), "%04d-%02d-%02d", y, m, d);
    return std::string(buf);
}

// Fix 4: Prüfen ob Zimmer im Zeitraum frei (mit 5 Tage Puffer nach checkOut)
bool BookingManager::isRoomAvailableForDates(int roomNumber,
                                             const std::string& checkIn,
                                             const std::string& checkOut) {
    int newIn  = dateToInt(checkIn);
    int newOut = dateToInt(checkOut);

    for (auto& b : bookings) {
        if (b.isCancelled()) continue;
        if (b.getRoom()->getRoomNumber() != roomNumber) continue;

        // Bestehende Buchung + 5 Tage Puffer
        int existIn  = dateToInt(b.getCheckIn());
        int existOut = dateToInt(addDays(b.getCheckOut(), 5)); // +5 Tage Puffer

        // Überschneidung prüfen
        if (newIn < existOut && newOut > existIn) return false;
    }
    return true;
}

Booking* BookingManager::getActiveBookingForRoom(int roomNumber) {
    for (auto& b : bookings) {
        if (!b.isCancelled() && b.getRoom()->getRoomNumber() == roomNumber)
            return &b;
    }
    return nullptr;
}

// Fix 4: Zimmer suchen mit Datum und exakt passende Betten
std::vector<Room*> BookingManager::searchRoomsForDates(int persons,
                                                        const std::string& checkIn,
                                                        const std::string& checkOut) {
    std::vector<Room*> result;
    auto allRooms = hotel->getAllRooms();
    for (auto r : allRooms)
        if (r->getBeds() == persons && isRoomAvailableForDates(r->getRoomNumber(), checkIn, checkOut))
            result.push_back(r);
    return result;
}

std::vector<Room*> BookingManager::searchRooms(int persons) {
    return hotel->getAvailableRooms(persons);
}

Guest* BookingManager::addGuest(std::string name, std::string email, std::string phone) {
    guests.push_back(Guest(name, email, phone));
    return &guests.back();
}

// Fix 3: Gast löschen
bool BookingManager::removeGuest(const std::string& name) {
    for (auto it = guests.begin(); it != guests.end(); ++it) {
        if (it->getName() == name) {
            guests.erase(it);
            return true;
        }
    }
    return false;
}

std::vector<Guest*> BookingManager::getAllGuests() {
    std::vector<Guest*> result;
    for (auto& g : guests) result.push_back(&g);
    return result;
}

int BookingManager::findGuestIndex(const std::string& name) {
    for (int i = 0; i < (int)guests.size(); i++)
        if (guests[i].getName() == name) return i;
    return -1;
}

Booking* BookingManager::createBooking(int roomNumber, int guestIndex,
                                       std::string checkIn, std::string checkOut, int nights) {
    std::string id = "BK-" + std::to_string(nextId++);
    return createBookingWithId(id, roomNumber, guestIndex, checkIn, checkOut, nights);
}

Booking* BookingManager::createBookingWithId(std::string id, int roomNumber, int guestIndex,
                                             std::string checkIn, std::string checkOut, int nights) {
    Room* room = hotel->getRoomByNumber(roomNumber);
    if (!room) return nullptr;
    if (guestIndex < 0 || guestIndex >= (int)guests.size()) return nullptr;
    // Beim Laden nicht auf isAvailable() prüfen da wir es selbst verwalten
    bookings.push_back(Booking(id, room, &guests[guestIndex], checkIn, checkOut, nights));
    int num = 0;
    try { num = std::stoi(id.substr(3)); } catch(...) {}
    if (num >= nextId) nextId = num + 1;
    return &bookings.back();
}

bool BookingManager::cancelBooking(std::string bookingId) {
    for (auto& b : bookings)
        if (b.getBookingId() == bookingId && !b.isCancelled()) { b.cancel(); return true; }
    return false;
}

std::vector<Booking*> BookingManager::getAllBookings() {
    std::vector<Booking*> result;
    for (auto& b : bookings) result.push_back(&b);
    return result;
}

std::vector<Booking*> BookingManager::getBookingsForGuest(const std::string& guestName) {
    std::vector<Booking*> result;
    for (auto& b : bookings)
        if (b.getGuest()->getName() == guestName) result.push_back(&b);
    return result;
}

void BookingManager::setBookingCode(const std::string& bookingId, const std::string& code) {
    bookingCodes[bookingId] = code;
}

std::string BookingManager::getBookingCode(const std::string& bookingId) const {
    auto it = bookingCodes.find(bookingId);
    return it != bookingCodes.end() ? it->second : "";
}

const std::map<std::string,std::string>& BookingManager::getAllCodes() const {
    return bookingCodes;
}