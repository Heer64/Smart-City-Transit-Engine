// main.cpp
#include "TransitEngine.h"
#include "api_handler.h"
#include <iostream>
#include <string>

void displayMenu() {
    std::cout << "\n=========================================\n";
    std::cout << "     🏙️  SMART CITY TRANSIT SYSTEM       \n";
    std::cout << "=========================================\n";
    std::cout << "1. Add New Transit Route\n";
    std::cout << "2. View All Stored Routes\n";
    std::cout << "3. Optimize & Sort Routes (Fastest First)\n";
    std::cout << "4. Exit System\n";
    std::cout << "=========================================\n";
    std::cout << "Enter your selection (1-4): ";
}

int main() {
    TransitEngine engine("routes_db.txt");
    int choice = 0;

    while (choice != 4) {
        displayMenu();

        if (!(std::cin >> choice)) {
            std::cout << "\n❌ Invalid input! Please enter a number between 1 and 4.\n";
            std::cin.clear();           
            std::cin.ignore(1000, '\n');
            continue;                    
        }

        std::cin.ignore(1000, '\n');

        if (choice == 1) {
            std::string name, dest;
            double distance;
            int delay;

            std::cout << "\nEnter Route Reference Name (e.g., Route-A): ";
            std::getline(std::cin, name);

            std::cout << "Enter Destination City Name (e.g., Ahmedabad): ";
            std::getline(std::cin, dest);

            std::cout << "Enter Route Base Distance (in km): ";
    
            if (!(std::cin >> distance)) {
                std::cout << "❌ Invalid distance value. Exiting route creation.\n";
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                continue;
            }

            std::cout << "Enter Active Traffic Delay (in minutes): ";

            if (!(std::cin >> delay)) {
                std::cout << "❌ Invalid delay value. Exiting route creation.\n";
                std::cin.clear();
                std::cin.ignore(1000, '\n');
                continue;
            }
            std::cin.ignore(1000, '\n');

            std::cout << "\n🛰️  Contacting OpenStreetMap API to fetch geospatial telemetry for " << dest << "...\n";
            Coordinates geoData = APIHandler::fetchCoordinates(dest);

            if (geoData.success) {
                std::cout << "📍 Coordinates Found! Lat: " << geoData.lat << ", Lon: " << geoData.lon << "\n";
        
                engine.addRoute(name, dest, distance, delay, geoData.lat, geoData.lon);
            } else {
                std::cout << "⚠️  Could not resolve coordinates for destination. Using fallback (0.0, 0.0).\n";
                engine.addRoute(name, dest, distance, delay, 0.0, 0.0);
            }

        } else if (choice == 2) {
            engine.viewAllRoutes();

        } else if (choice == 3) {
            engine.sortRoutesByTotalTime();
            engine.viewAllRoutes();

        } else if (choice == 4) {
            std::cout << "\n👋 Shutting down Smart City Transit Engine. All records safely synced.\n";
        } else {
            std::cout << "❌ Invalid selection. Please choose an option from 1 to 4.\n";
        }
    }

    return 0;
}