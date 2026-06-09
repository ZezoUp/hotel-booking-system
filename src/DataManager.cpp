#include "DataManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <functional>

const std::string DataManager::FILENAME = "hotel_data.json";

std::string DataManager::escapeJson(const std::string& s) {
    std::string r;
    for (char c : s) {
        if (c == '"')       r += "\\\"";
        else if (c == '\\') r += "\\\\";
        else r += c;
    }
    return r;
}

void DataManager::saveAll(Hotel* hotel, BookingManager* manager, UserManager* userManager) {
    std::ofstream file(FILENAME);
    if (!file.is_open()) return;

    file << "{\n";

    // Hotel
    file << "  \"hotel\":{\"name\":\"" << escapeJson(hotel->getName())
         << "\",\"address\":\"" << escapeJson(hotel->getAddress()) << "\"},\n";

    // Zimmer
    file << "  \"rooms\":[\n";
    auto rooms = hotel->getAllRooms();
    for (int i=0; i<(int)rooms.size(); i++) {
        Room* r = rooms[i];
        file << "    {\"number\":" << r->getRoomNumber() << ",\"beds\":" << r->getBeds()
             << ",\"price\":" << r->getPricePerNight() << "}";
        if (i<(int)rooms.size()-1) file << ",";
        file << "\n";
    }
    file << "  ],\n";

    // User Accounts
    file << "  \"accounts\":[\n";
    auto& accounts = userManager->getAllAccounts();
    for (int i=0; i<(int)accounts.size(); i++) {
        file << "    {\"name\":\"" << escapeJson(accounts[i].name)
        << "\",\"email\":\"" << escapeJson(accounts[i].email)
        << "\",\"phone\":\"" << escapeJson(accounts[i].phone)
        << "\",\"password\":\"" << escapeJson(accounts[i].password) << "\"}";
        if (i<(int)accounts.size()-1) file << ",";
        file << "\n";
    }
    file << "  ],\n";

    // Gaeste (für BookingManager intern)
    file << "  \"guests\":[\n";
    auto guests = manager->getAllGuests();
    for (int i=0; i<(int)guests.size(); i++) {
        file << "    {\"name\":\"" << escapeJson(guests[i]->getName())
        << "\",\"email\":\"" << escapeJson(guests[i]->getEmail())
        << "\",\"phone\":\"" << escapeJson(guests[i]->getPhone()) << "\"}";
        if (i<(int)guests.size()-1) file << ",";
        file << "\n";
    }
    file << "  ],\n";

    // Buchungen
    file << "  \"bookings\":[\n";
    auto bookings = manager->getAllBookings();
    auto& codes   = manager->getAllCodes();
    for (int i=0; i<(int)bookings.size(); i++) {
        Booking* b = bookings[i];
        std::string code;
        auto it = codes.find(b->getBookingId());
        if (it != codes.end()) code = it->second;
        file << "    {\"id\":\"" << escapeJson(b->getBookingId())
             << "\",\"guestName\":\"" << escapeJson(b->getGuest()->getName())
             << "\",\"roomNumber\":" << b->getRoom()->getRoomNumber()
             << ",\"checkIn\":\"" << escapeJson(b->getCheckIn())
             << "\",\"checkOut\":\"" << escapeJson(b->getCheckOut())
             << "\",\"nights\":" << b->getNights()
             << ",\"cancelled\":" << (b->isCancelled()?"true":"false")
             << ",\"code\":\"" << escapeJson(code) << "\"}";
        if (i<(int)bookings.size()-1) file << ",";
        file << "\n";
    }
    file << "  ]\n}\n";
    file.close();
}

static std::string extractValue(const std::string& line, const std::string& key) {
    size_t pos = line.find("\"" + key + "\":");
    if (pos == std::string::npos) return "";
    pos += key.size() + 3;
    while (pos < line.size() && (line[pos]==' '||line[pos]=='\t')) pos++;
    if (line[pos] == '"') {
        pos++;
        std::string val;
        while (pos < line.size() && line[pos] != '"') {
            if (line[pos]=='\\' && pos+1<line.size()) { pos++; }
            val += line[pos++];
        }
        return val;
    } else {
        std::string val;
        while (pos < line.size() && line[pos]!=',' && line[pos]!='\n' && line[pos]!='}') val += line[pos++];
        while (!val.empty() && (val.back()==' '||val.back()=='\r')) val.pop_back();
        return val;
    }
}

static void loadSection(const std::string& content, const std::string& startKey,
                        const std::string& endKey, std::function<void(const std::string&)> lineHandler) {
    size_t start = content.find("\"" + startKey + "\":");
    size_t end   = endKey.empty() ? content.size() : content.find("\"" + endKey + "\":");
    if (start == std::string::npos) return;
    std::istringstream ss(content.substr(start, end == std::string::npos ? content.size()-start : end-start));
    std::string line;
    while (std::getline(ss, line)) lineHandler(line);
}

bool DataManager::loadAll(Hotel* hotel, BookingManager* manager, UserManager* userManager) {
    std::ifstream file(FILENAME);
    if (!file.is_open()) return false;
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Zimmer
    loadSection(content, "rooms", "accounts", [&](const std::string& line){
        static int number=0, beds=0; static double price=0; static bool in=false;
        if (line.find("{")!=std::string::npos && line.find("rooms")==std::string::npos) in=true;
        if (!in) return;
        std::string v;
        if (!(v=extractValue(line,"number")).empty()) number = std::stoi(v);
        if (!(v=extractValue(line,"beds")).empty())   beds   = std::stoi(v);
        if (!(v=extractValue(line,"price")).empty())  price  = std::stod(v);
        if (line.find("}")!=std::string::npos && in) { hotel->addRoom(number,beds,price); in=false; }
    });

    // Accounts
    loadSection(content, "accounts", "guests", [&](const std::string& line){
        static std::string name,email,phone,password; static bool in=false;
        if (line.find("{")!=std::string::npos && line.find("accounts")==std::string::npos) in=true;
        if (!in) return;
        std::string v;
        if (!(v=extractValue(line,"name")).empty())     name     = v;
        if (!(v=extractValue(line,"email")).empty())    email    = v;
        if (!(v=extractValue(line,"phone")).empty())    phone    = v;
        if (!(v=extractValue(line,"password")).empty()) password = v;
        if (line.find("}")!=std::string::npos && in) {
            userManager->registerUser(name,email,phone,password); in=false;
        }
    });

    // Gaeste
    loadSection(content, "guests", "bookings", [&](const std::string& line){
        static std::string name,email,phone; static bool in=false;
        if (line.find("{")!=std::string::npos && line.find("guests")==std::string::npos) in=true;
        if (!in) return;
        std::string v;
        if (!(v=extractValue(line,"name")).empty())  name  = v;
        if (!(v=extractValue(line,"email")).empty()) email = v;
        if (!(v=extractValue(line,"phone")).empty()) phone = v;
        if (line.find("}")!=std::string::npos && in) { manager->addGuest(name,email,phone); in=false; }
    });

    // Buchungen
    loadSection(content, "bookings", "", [&](const std::string& line){
        static std::string bId,gName,checkIn,checkOut,code;
        static int roomNr=0,nights=0; static bool cancelled=false,in=false;
        if (line.find("{")!=std::string::npos && line.find("bookings")==std::string::npos) in=true;
        if (!in) return;
        std::string v;
        if (!(v=extractValue(line,"id")).empty())         bId      = v;
        if (!(v=extractValue(line,"guestName")).empty())  gName    = v;
        if (!(v=extractValue(line,"roomNumber")).empty()) roomNr   = std::stoi(v);
        if (!(v=extractValue(line,"checkIn")).empty())    checkIn  = v;
        if (!(v=extractValue(line,"checkOut")).empty())   checkOut = v;
        if (!(v=extractValue(line,"nights")).empty())     nights   = std::stoi(v);
        if (!(v=extractValue(line,"cancelled")).empty())  cancelled= (v=="true");
        if (!(v=extractValue(line,"code")).empty())       code     = v;
        if (line.find("}")!=std::string::npos && in) {
            int gIdx = manager->findGuestIndex(gName);
            if (gIdx>=0) {
                Booking* b = manager->createBookingWithId(bId,roomNr,gIdx,checkIn,checkOut,nights);
                if (b) {
                    if (!code.empty()) manager->setBookingCode(bId,code);
                    if (cancelled) b->cancel();
                }
            }
            in=false; code=""; bId=""; gName=""; checkIn=""; checkOut="";
        }
    });
    return true;
}