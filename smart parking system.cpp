#include <iostream>
using namespace std;

// Struct to represent a Parking Spot
struct ParkingSpot {
    int id;
    bool isAvailable;
    SlotSize size; 
    double distanceFromEntrance; 
    double baseRate;             
    double ratePerHour;          
};

// Struct to represent a Vehicle
struct Vehicle {
    
    int driverID;
    string licenseNumber;
    VehicleType type;
    double entryTime;    
};
int main(){
  
return 0;
}
