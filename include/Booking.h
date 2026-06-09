#pragma once
#include <string>
#include "Room.h"
#include "Guest.h"

class Booking {
private:
    std::string bookingId;
    Room*       room;
    Guest*      guest;
    std::string checkIn;
    std::string checkOut;
    int         nights;
    double      totalPrice;
    bool        cancelled;
public:
    Booking(std::string id, Room* room, Guest* guest,
            std::string checkIn, std::string checkOut, int nights);
    std::string getBookingId()  const;
    Room*       getRoom()       const;
    Guest*      getGuest()      const;
    std::string getCheckIn()    const;
    std::string getCheckOut()   const;
    int         getNights()     const;
    double      getTotalPrice() const;
    bool        isCancelled()   const;
    void        cancel();
    std::string getInfo()       const;
};
