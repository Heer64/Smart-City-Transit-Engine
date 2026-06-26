// api_handler.h
#ifndef API_HANDLER_H
#define API_HANDLER_H

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>

struct Coordinates {
    double lat = 0.0;
    double lon = 0.0;
    bool success = false;
};

class APIHandler {
public:
    static Coordinates fetchCoordinates(std::string locationName) {
        Coordinates coords;
        
        std::string searchName = "";
        for (char c : locationName) {
            if (c == ' ') searchName += "+";
            else searchName += c;
        }

        std::string url = "https://nominatim.openstreetmap.org/search?q=" + searchName + "&format=json&limit=1";
        
        std::string command = "curl -s -A \"SmartCityTransitAgent/1.0\" \"" + url + "\" > temp_geo.txt";
        
        int result = std::system(command.c_str());
        if (result != 0) {
            std::cerr << "⚠️ Network request failed. Check internet connection.\n";
            return coords;
        }

        std::ifstream inFile("temp_geo.txt");
        if (!inFile.is_open()) return coords;

        std::string line;
        while (std::getline(inFile, line)) {
            size_t latPos = line.find("\"lat\":\"");
            size_t lonPos = line.find("\"lon\":\"");

            if (latPos != std::string::npos && lonPos != std::string::npos) {
              
                size_t latStart = latPos + 7;
                size_t latEnd = line.find("\"", latStart);
                std::string latStr = line.substr(latStart, latEnd - latStart);

                size_t lonStart = lonPos + 7;
                size_t lonEnd = line.find("\"", lonStart);
                std::string lonStr = line.substr(lonStart, lonEnd - lonStart);

                coords.lat = std::stod(latStr);
                coords.lon = std::stod(lonStr);
                coords.success = true;
                break;
            }
        }
        inFile.close();
        std::remove("temp_geo.txt");
        return coords;
    }
};

#endif