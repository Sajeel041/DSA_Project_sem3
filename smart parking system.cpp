#include <iostream>
#include <vector>
#include <unordered_map>
#include <list>
#include <ctime>
#include <algorithm>
#include <cctype>    // for tolower
#include <string>
#include <limits>
#include <sstream>
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
public:
    // Constructor to initialize parking spots and adjacency matrix with deterministic sizes
    SmartParkingManagement(int totalSpots, const vector<vector<int>> &graph) {
        // Initialize adjacency matrix
        adjacencyMatrix = graph;

        // Initialize parking spots with fixed sizes for testing
        for (int i = 0; i < totalSpots; i++) {
            SlotSize size;
            // Assign sizes based on spot ID for predictability
            if (i == 0) {
                size = SlotSize::REGULAR; // Suitable for Car
            } else if (i == 1 || i == 2) {
                size = SlotSize::COMPACT; // Suitable for Motorcycle
            } else {
                size = SlotSize::LARGE; // Suitable for Truck
            }
            double distance = static_cast<double>(rand() % 100 + 1); // Distance between 1 and 100 meters
            double baseRate = 5.0;    // Base fee
            double ratePerHour = 3.0; // Hourly rate

            parkingSpots.push_back({i, true, size, distance, baseRate, ratePerHour});
        }
        sortSpotsByProximity();
    }

    // Getter for parkingSpots (returns const reference to prevent modification)
    const vector<ParkingSpot>& getParkingSpots() const {
        return parkingSpots;
    }

    // Display available parking spots based on vehicle type
    virtual void displayAvailableSpots(VehicleType type) const {
        cout << "Available Parking Spots for ";
        switch (type) {
            case VehicleType::MOTORCYCLE:
                cout << "Motorcycle:\n";
                break;
            case VehicleType::CAR:
                cout << "Car:\n";
                break;
            case VehicleType::TRUCK:
                cout << "Truck:\n";
                break;
            default:
                cout << "Unknown Vehicle Type:\n";
        }
        bool anyAvailable = false;
        for (const auto &spot : parkingSpots) {
            if (spot.isAvailable && canFit(type, spot.size)) {
                anyAvailable = true;
                cout << "Spot ID: " << spot.id
                     << ", Size: " << ((spot.size == SlotSize::COMPACT) ? "Compact" :
                                      (spot.size == SlotSize::REGULAR) ? "Regular" : "Large")
                     << ", Distance: " << spot.distanceFromEntrance << " meters\n";
            }
        }
        if (!anyAvailable) {
            cout << "No available spots for this vehicle type.\n";
        }
    }
// This function saves all the parking spot info and reservations to a file
void saveData() const {
    // Open (or create) a file named "parking_data.txt" where we'll save our data
    ofstream outFile("parking_data.txt");
    
    // If the file didn't open for some reason, let the user know and stop here
    if (!outFile) {
        cerr << "Error opening file for writing.\n";
        return;
    }
    
    // Loop through each parking spot and write its details to the file, separated by commas
    for (const auto &spot : parkingSpots) {
        outFile << spot.id << "," << spot.isAvailable << "," 
                << static_cast<int>(spot.size) << "," 
                << spot.distanceFromEntrance << "," 
                << spot.baseRate << "," 
                << spot.ratePerHour << "\n";
    }
    
    // Now, go through all the reservations and save them too
    for (const auto &res : reservations) {
        outFile << res.first << "," << res.second.first << "," << res.second.second << "\n";
    }
    
    // Close the file to make sure everything is saved properly
    outFile.close();
    
    // Let the user know that the data was saved successfully
    cout << "Data saved successfully.\n";
}
// This function loads parking spot info and reservations from a file
void loadData() {
    // Try to open the "parking_data.txt" file to read data from it
    ifstream inFile("parking_data.txt");
    
    // If the file doesn't exist or can't be opened, tell the user and start fresh
    if (!inFile) {
        cout << "No existing data found. Starting fresh.\n";
        return;
    }
    
    // Clear out any current parking spots so we don't mix old and new data
    parkingSpots.clear();
    
    string line; // This will hold each line we read from the file
    
    // First, read all the parking spots from the file
    while (getline(inFile, line)) {
        stringstream ss(line); // Break the line into parts separated by commas
        string token; // Temporary variable to hold each part
        
        // Get the spot ID
        getline(ss, token, ',');
        int id = stoi(token); // Convert the string to an integer
        
        // Get whether the spot is available
        getline(ss, token, ',');
        bool isAvailable = stoi(token); // Convert "1" or "0" to true or false
        
        // Get the size of the spot
        getline(ss, token, ',');
        SlotSize size = static_cast<SlotSize>(stoi(token)); // Convert to SlotSize enum
        
        // Get the distance from the entrance
        getline(ss, token, ',');
        double distance = stod(token); // Convert to a double
        
        // Get the base rate
        getline(ss, token, ',');
        double baseRate = stod(token); // Convert to a double
        
        // Get the rate per hour
        getline(ss, token, ',');
        double ratePerHour = stod(token); // Convert to a double
        
        // Add this spot to our parkingSpots list
        parkingSpots.push_back({id, isAvailable, size, distance, baseRate, ratePerHour});
    }
    
    // Now, we need to read the reservations
    // First, clear any existing reservations
    reservations.clear();
    
    // Reset the file pointer back to the beginning of the file
    inFile.clear();
    inFile.seekg(0, ios::beg);
    
    // Skip over the parking spot lines since we've already read them
    for (int i = 0; i < parkingSpots.size(); i++) {
        getline(inFile, line);
    }
    
    // Now, read the reservation lines
    while (getline(inFile, line)) {
        if (line.empty()) continue; // Skip any empty lines
        
        stringstream ss(line); // Break the line into parts separated by commas
        string token; // Temporary variable to hold each part
        
        // Get the driver ID
        getline(ss, token, ',');
        int driverID = stoi(token); // Convert to an integer
        
        // Get the spot ID they've reserved
        getline(ss, token, ',');
        int spotID = stoi(token); // Convert to an integer
        
        // Get the time they reserved the spot
        getline(ss, token, ',');
        double entryTime = stod(token); // Convert to a double (timestamp)
        
        // Add this reservation to our reservations map
        reservations[driverID] = {spotID, entryTime};
        
        // Find the corresponding parking spot and mark it as unavailable
        for (auto &spot : parkingSpots) {
            if (spot.id == spotID) {
                spot.isAvailable = false;
                break; // No need to keep looking once we found the spot
            }
        }
    }
    
    // Close the file now that we're done reading everything
    inFile.close();
    
    // Let the user know that the data was loaded successfully
    cout << "Data loaded successfully.\n";
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
    // Release a reserved spot and calculate parking fee
    void releaseSpot() {
        int driverID;

        cout << "=== Release Parking Spot ===\n";
        cout << "Enter Driver ID: ";
        cin >> driverID;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        auto it = reservations.find(driverID);
        if (it != reservations.end()) {
            int spotID = it->second.first;
            double entryTime = it->second.second;
            double exitTime = static_cast<double>(time(0));

            double duration = (exitTime - entryTime) / 3600.0; // Convert seconds to hours
            if (duration < 0.0) duration = 0.0; // Prevent negative duration

            // Find the parking spot details
            auto spotIt = find_if(parkingSpots.begin(), parkingSpots.end(),
                                  [spotID](const ParkingSpot &spot) { return spot.id == spotID; });

            if (spotIt != parkingSpots.end()) {
                double fee = spotIt->baseRate + (duration * spotIt->ratePerHour);
                spotIt->isAvailable = true;
                reservations.erase(it);
                entryExitLogs.emplace_back(spotID, exitTime);

                cout << "Spot ID " << spotID << " released for Driver ID " << driverID << ".\n";
                cout << "Total Duration: " << fixed << setprecision(2) << duration << " hours\n";
                cout << "Parking Fee: $" << fixed << setprecision(2) << fee << "\n";
            } else {
                cout << "Error: Spot ID " << spotID << " not found.\n";
            }
        } else {
            cout << "No reservation found for Driver ID " << driverID << ".\n";
        }
    }
};
class ParkingLotManager : public SmartParkingManagement {
protected:
    vector<string> &managerNames;

public:
    ParkingLotManager(int totalSpots, const vector<vector<int>> &graph, vector<string> &names)
        : SmartParkingManagement(totalSpots, graph), managerNames(names) {}

    // Authenticate manager by name (simple authentication)
    bool authenticateManager(const string &name) const {
        string lowerName = toLowerCase(name); // Now accessible as it's protected
        for (const auto &manager : managerNames) {
            string lowerManager = toLowerCase(manager);
            if (lowerName == lowerManager)
                return true;
        }
        return false;
    }
void addParkingSpot() {
        int id, sizeChoice;
        double distance, baseRate, ratePerHour;

        cout << "=== Add New Parking Spot ===\n";
        cout << "Enter Spot ID: ";
        cin >> id;

        // Check if spot ID already exists
        if (isValidSpotID(id)) {
            cout << "Spot ID " << id << " already exists.\n";
            return;
        }

        cout << "Select Spot Size:\n1. Compact\n2. Regular\n3. Large\nEnter your choice: ";
        while (!(cin >> sizeChoice) || sizeChoice < 1 || sizeChoice > 3) {
            cout << "Invalid input. Please enter a number between 1 and 3: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        SlotSize size;
        switch (sizeChoice) {
            case 1:
                size = SlotSize::COMPACT;
                break;
            case 2:
                size = SlotSize::REGULAR;
                break;
            case 3:
                size = SlotSize::LARGE;
                break;
            default:
                size = SlotSize::REGULAR;
        }

        cout << "Enter Distance from Entrance (in meters): ";
        cin >> distance;
        cout << "Enter Base Rate: $";
        cin >> baseRate;
        cout << "Enter Rate Per Hour: $";
        cin >> ratePerHour;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer

        // Add the new spot
        parkingSpots.push_back({id, true, size, distance, baseRate, ratePerHour});
        sortSpotsByProximity();
        cout << "Added new parking spot with ID " << id << ".\n";
    }
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
                             driver.releaseSpot();
                            break;
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
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        if (manager.authenticateManager(managerName)) {
                            int managerChoice;
                        do {
                            cout << "\n=== Parking Lot Manager Menu ===\n";
                            cout << "1. Display Available Spots\n";
                            cout << "2. Add Parking Spot\n";
                            cout << "3. Update Parking Spot\n";
                            cout << "4. Back to Main Menu\n";
                            cout << "Enter your choice: ";
                            while (!(cin >> managerChoice) || managerChoice < 1 || managerChoice > 4) {
                                cout << "Invalid input. Please enter a number between 1 and 4: ";
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            }
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
                            switch(managerChoice){
                                case 1:{
                                    cout << "\n=== Available Parking Spots ===\n";
                                    for (const auto &spot : manager.getParkingSpots()) { // Use getter
                                        if (spot.isAvailable) {
                                            cout << "Spot ID: " << spot.id<< ", Size: " << ((spot.size == SlotSize::COMPACT) ? "Compact" :(spot.size == SlotSize::REGULAR) ? "Regular" : "Large") << ", Distance: " << spot.distanceFromEntrance << " meters\n";        
                                        }
                                    }
                                break;
                                }
                                case 2:{
                                    manager.addParkingSpot();
                                    break;
                                }
                                case 3:{}
                                case 4:{
                                    cout << "Returning to Main Menu...\n";
                                    break;
                                }
                                default:
                                    cout << "Invalid choice. Please try again.\n";      
                            }    
                        }while(mangerchoice!=4);
                        }else 
                            cout << "Invalid manager name. Returning to Main Menu.\n";
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
