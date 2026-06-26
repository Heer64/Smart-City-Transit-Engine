#ifndef TRANSIT_ENGINE_H
#define TRANSIT_ENGINE_H

#include <string>
#include <vector>

struct Route {
    std::string routeName;
    std::string destination;
    double baseDistance;      
    int trafficDelayMinutes;  
    double latitude;
    double longitude;
};

class TransitEngine {
private:
    std::vector<Route> routes;
    std::string dbFilename;

public:
   
    TransitEngine(std::string filename);

    void addRoute(std::string name, std::string dest, double dist, int delay, double lat, double lon);
    void viewAllRoutes();
    void sortRoutesByTotalTime();
    
    void saveToFile();
    void loadFromFile();
};

#endif