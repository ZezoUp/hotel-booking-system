#pragma once
#include <string>
#include "Hotel.h"
#include "BookingManager.h"
#include "UserManager.h"

class DataManager {
public:
    static void saveAll(Hotel* hotel, BookingManager* manager, UserManager* userManager);
    static bool loadAll(Hotel* hotel, BookingManager* manager, UserManager* userManager);
private:
    static const std::string FILENAME;
    static std::string escapeJson(const std::string& s);
};