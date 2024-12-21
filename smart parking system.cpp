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
public:
    Driver(int totalSpots, const vector<vector<int>> &graph) : SmartParkingManagement(totalSpots, graph) {}

    // Reserve a spot based on vehicle type
    void reserveSpot() {
        int driverID;
        string licenseNumber;
        int vehicleChoice;
        VehicleType type;

        cout << "=== Reserve Parking Spot ===\n";
        cout << "Enter Driver ID: ";
        cin >> driverID;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        if (reservations.find(driverID) != reservations.end()) {
            cout << "Driver ID " << driverID << " already has a reserved spot.\n";
            return;
        }

        cout << "Enter License Number: ";
        getline(cin, licenseNumber);

        cout << "Select Vehicle Type:\n1. Motorcycle\n2. Car\n3. Truck\nEnter your choice: ";
        while (!(cin >> vehicleChoice) || vehicleChoice < 1 || vehicleChoice > 3) {
            cout << "Invalid input. Please enter a number between 1 and 3: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');//numeric_limits<streamsize>::max() tells cin.ignore() to ignore up to the maximum number of characters that can fit in the input buffer
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        type = static_cast<VehicleType>(vehicleChoice);// converting the integer vehical type to enum

        int spotID = findBestFitSpot(type);
        if (spotID != -1) {
            // Reserve the spot
            for (auto &spot : parkingSpots) {
                if (spot.id == spotID) {
                    spot.isAvailable = false;
                    double entryTime = static_cast<double>(time(0));
                    reservations[driverID] = {spotID, entryTime};
                    entryExitLogs.emplace_back(spotID, entryTime);
                    cout << "Spot ID " << spotID << " reserved for Driver ID " << driverID << ".\n";
                    cout << "Vehicle Type: " << ((type == VehicleType::MOTORCYCLE) ? "Motorcycle" :
                                                (type == VehicleType::CAR) ? "Car" : "Truck") << "\n";
                    return;
                }
            }
        } else {
            cout << "No suitable spots available for your vehicle type.\n";
        }
    }
};
class ParkingLotManager : public SmartParkingManagement {

};
class Admin{

};

int main(){
    srand(static_cast<unsigned int>(time(0))); // Seed for random number generation

    int totalSpots;
    cout << "=== Smart Parking Management System ===\n";
    cout << "Enter total number of parking spots: ";
    while (!(cin >> totalSpots) || totalSpots <= 0) {
        cout << "Invalid input. Please enter a positive integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

    vector<vector<int>> graph(totalSpots, vector<int>(totalSpots, 1)); // 1 indicates a direct connection
    
    vector<string> managerNames = {"Alice", "Bob", "Charlie"};

    Driver driver(totalSpots, graph);
    ParkingLotManager manager(totalSpots, graph, managerNames);
    Admin admin(managerNames);

    driver.loadData();
    int choice;
  do {
        cout << "\n=== Main Menu ===\n";
        cout << "Select Role:\n";
        cout << "1. Driver\n";
        cout << "2. Parking Lot Manager\n";
        cout << "3. Admin\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        while (!(cin >> choice) || choice < 1 || choice > 4) {
               cout << "Invalid input. Please enter a number between 1 and 4: ";
                 cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
             }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
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
                    while (!(cin >> driverChoice) || driverChoice < 1 || driverChoice > 4) {
                        cout << "Invalid input. Please enter a number between 1 and 4: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
                    switch (driverChoice) {
                        case 1: { // Display Available Spots
                            int vehicleChoice;
                            cout << "Select Vehicle Type to View Available Spots:\n1. Motorcycle\n2. Car\n3. Truck\nEnter your choice: ";
                            while (!(cin >> vehicleChoice) || vehicleChoice < 1 || vehicleChoice > 3) {
                                cout << "Invalid input. Please enter a number between 1 and 3: ";
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            }
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
                            VehicleType type = static_cast<VehicleType>(vehicleChoice); // converting integer to enum
                            driver.displayAvailableSpots(type);
                            break;
                        }
                        case 2:{
                            driver.reserveSpot();
                            break;
                        }
                        case 3:{
                            
                        }
                        case 4: { 
                            cout << "Returning to Main Menu...\n";
                            break;
                        }
                        default:
                            cout << "Invalid choice. Please try again.\n";
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
