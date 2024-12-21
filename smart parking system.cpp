#include <iostream>
using namespace std;

// Struct to represent a Parking Spot
enum class SlotSize { COMPACT = 1, REGULAR, LARGE };
enum class VehicleType { MOTORCYCLE = 1, CAR, TRUCK };

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
class SmartParkingManagement{
protected:// all of the fuctions present in the protected area will be used in the child classes ok
    vector<ParkingSpot> parkingSpots;
    unordered_map<int, pair<int, double>> reservations; // driverID -> (spotID, entryTime)
    list<pair<int, double>> entryExitLogs;              // (spotID, timestamp)
    vector<vector<int>> adjacencyMatrix;                // Graph representation of parking lot

    // Helper function to convert string to lowercase
string toLowerCase(const string& str) const {
    string lowerStr = str;
    for (char &ch : lowerStr) {
        ch = tolower(ch);  // Convert each character to lowercase
    }
    return lowerStr;
    }
bool isValidSpotID(int id) const {
    // Iterate through each parking spot to check if the id exists
    for (const auto &spot : parkingSpots) {
        if (spot.id == id) {
            return true; // id found, return true
        }
    }
    return false; // id not found, return false
}
int findBestFitSpot(VehicleType type) const {
        //spots to find the smallest available spot that fits the vehicle
        for (const auto &spot : parkingSpots) {
            if (spot.isAvailable && canFit(type, spot.size)) {
                return spot.id;
            }
        }
        return -1; // No suitable spot found
    }
 // Check if a vehicle type can fit into a slot size
    bool canFit(VehicleType type, SlotSize size) const {
        if (type == VehicleType::MOTORCYCLE) {
            return (size == SlotSize::COMPACT);
        } else if (type == VehicleType::CAR) {
            return (size == SlotSize::REGULAR || size == SlotSize::LARGE);
        } else if (type == VehicleType::TRUCK) {
            return (size == SlotSize::LARGE);
        }
        return false;
    }

// Display the adjacency matrix (Graph Representation)
    void displayGraph() const {
        cout << "Parking Lot Graph (Adjacency Matrix):\n";
        for (const auto &row : adjacencyMatrix) {
            for (const auto &value : row) {
                cout << value << " ";
            }
            cout << "\n";
        }
    }
// Check if a spot ID is valid
    bool isValidSpotID(int id) const {
    for (const auto &spot : parkingSpots) {
        if (spot.id == id) {
            return true; 
        }
    }
    return false; 
}
};
class Driver : public SmartParkingManagement{

};
class ParkingLotManager : public SmartParkingManagement {

};
class Admin{

};

int main(){
    int choice;
  do {
        cout << "\n=== Main Menu ===\n";
        cout << "Select Role:\n";
        cout << "1. Driver\n";
        cout << "2. Parking Lot Manager\n";
        cout << "3. Admin\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";

      cin>>choice;
      switch(choice){
              case 1: { // Driver
                int driverChoice;
                do {
                    cout << "\n=== Driver Menu ===\n";
                    cout << "1. Display Available Spots\n";
                    cout << "2. Reserve Spot\n";
                    cout << "3. Release Spot\n";
                    cout << "4. Back to Main Menu\n";
                    cout << "Enter your choice: ";
                    switch(driverChoice){
                        case 1:{}
                        case 2:{
                        }
                        case 3:{}
                        case 4:{}
                        default:
                    }
                }(while (driverChoice != 4);
                  break;
                }
                    case 2:{ // Parking Lot Manager
                        string managerName;
                        cout << "Enter Manager Name: ";
                        cin >> managerName;
                        int managerChoice;
                    do {
                        cout << "\n=== Parking Lot Manager Menu ===\n";
                        cout << "1. Display Available Spots\n";
                        cout << "2. Add Parking Spot\n";
                        cout << "3. Update Parking Spot\n";
                        cout << "4. Back to Main Menu\n";
                        cout << "Enter your choice: ";
                        switch(managerChoice){
                            case 1:{}
                        case 2:{
                        }
                        case 3:{}
                        case 4:{}
                        default:
                            
                        }
                    }while(mangerchoice!=4);
                        break;
                    }
                    case 3:{ // Parking Lot Manager
                        string managerName;
                        cout << "Enter Manager Name: ";
                        cin >> managerName;
                        int managerChoice;
                    do {
                        cout << "\n=== Admin Menu ===\n";
                        cout << "1. Add Manager\n";
                        cout << "2. Remove Manager\n";
                        cout << "3. Change Security Code\n";
                        cout << "4. Back to Main Menu\n";
                        cout << "Enter your choice: ";
                        switch(managerChoice){
                            case 1:{}
                        case 2:{
                        }
                        case 3:{}
                        case 4:{}
                        default:
                            
                        }
                    }while(mangerchoice!=4);
                        break;
                    }
                    case 4: { // Exit
                    driver.saveData(); // Save data before exiting
                    cout << "Exiting the system. Goodbye!\n";
                    break;
                    }
                    default:
                    cout << "Invalid choice. Please try again.\n";
                    }

                    }

          }

          
      }

  }while(choice !=4);
    
return 0;
}
