#include "TransitEngine.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iomanip>
TransitEngine::TransitEngine(std::string filename) {
    dbFilename = filename;
    loadFromFile();
}
void TransitEngine::addRoute(std::string name, std::string dest, double dist, int delay, double lat, double lon) {
    Route r = {name, dest, dist, delay, lat, lon};
    routes.push_back(r);
    saveToFile();
    std::cout << "✅ Route successfully recorded and saved to local DB!\n";
}
void TransitEngine::viewAllRoutes() {
    if (routes.empty()) {
        std::cout << "📭 No active transit tracks found in local memory.\n";
        return;
    }

    std::cout << "\n----------------------------------------------------------------------------------------\n";
    std::cout << std::left << std::setw(15) << "Route Name" 
              << std::setw(15) << "Destination" 
              << std::setw(15) << "Distance (km)" 
              << std::setw(15) << "Delay (min)" 
              << std::setw(25) << "Coordinates (Lat, Lon)" << "\n";
    std::cout << "----------------------------------------------------------------------------------------\n";

    for (const auto& r : routes) {
        std::cout << std::left << std::setw(15) << r.routeName
                  << std::setw(15) << r.destination
                  << std::setw(15) << r.baseDistance
                  << std::setw(15) << r.trafficDelayMinutes
                  << std::setw(25) << (std::to_string(r.latitude) + ", " + std::to_string(r.longitude)) 
                  << "\n";
    }
    std::cout << "----------------------------------------------------------------------------------------\n";
}

void TransitEngine::sortRoutesByTotalTime() {
    std::sort(routes.begin(), routes.end(), [](const Route& a, const Route& b) {
        double timeA = ((a.baseDistance / 40.0) * 60.0) + a.trafficDelayMinutes;
        double timeB = ((b.baseDistance / 40.0) * 60.0) + b.trafficDelayMinutes;
        return timeA < timeB;
    });

    std::cout << "🎯 Routes optimized and prioritized by total travel time!\n";
}
void TransitEngine::saveToFile() {
    std::ofstream outFile(dbFilename);
    if (!outFile.is_open()) return;

    for (const auto& r : routes) {
        outFile << r.routeName << "|" 
                << r.destination << "|" 
                << r.baseDistance << "|" 
                << r.trafficDelayMinutes << "|" 
                << r.latitude << "|" 
                << r.longitude << "\n";
    }
    outFile.close();
}

void TransitEngine::loadFromFile() {
    std::ifstream inFile(dbFilename);
    if (!inFile.is_open()) return;

    routes.clear(); 
    std::string line;
    
    while (std::getline(inFile, line)) {
        if (line.empty()) continue;

        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);
        size_t p4 = line.find('|', p3 + 1);
        size_t p5 = line.find('|', p4 + 1);

        if (p5 != std::string::npos) {
            Route r;
            r.routeName = line.substr(0, p1);
            r.destination = line.substr(p1 + 1, p2 - p1 - 1);
            r.baseDistance = std::stod(line.substr(p2 + 1, p3 - p2 - 1));
            r.trafficDelayMinutes = std::stoi(line.substr(p3 + 1, p4 - p3 - 1));
            r.latitude = std::stod(line.substr(p4 + 1, p5 - p4 - 1));
            r.longitude = std::stod(line.substr(p5 + 1));
            
            routes.push_back(r);
        }
    }
    inFile.close();
}