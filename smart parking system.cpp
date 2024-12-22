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
#include <fstream>   // for file I/O
#include <iomanip>   
#include<cmath>
#include<stack>
#include <queue> 
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
// ------------------- AVL Tree Implementation for ParkingSpots -------------------
/*
    AVL Tree is a self-balancing binary search tree. In this implementation, the AVL Tree
    is used to manage ParkingSpot nodes sorted by their Spot ID. This allows efficient
    search, insertion, and deletion operations with O(log n) time complexity.
*/

void merge(vector<ParkingSpot> &spots, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<ParkingSpot> L(n1), R(n2);

    for(int i = 0; i < n1; i++)
        L[i] = spots[left + i];
    for(int j = 0; j < n2; j++)
        R[j] = spots[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i].distanceFromEntrance <= R[j].distanceFromEntrance) {
            spots[k] = L[i];
            i++;
        }
        else {
            spots[k] = R[j];
            j++;
        }
        k++;
    }
    // Copy leftovers
    while (i < n1) {
        spots[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        spots[k] = R[j];
        j++;
        k++;
    }
}

void mergeSortSpots(vector<ParkingSpot> &spots, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSortSpots(spots, left, mid);
        mergeSortSpots(spots, mid + 1, right);
        merge(spots, left, mid, right);
    }
}
struct AVLNode {
    ParkingSpot spot;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(const ParkingSpot& s)
        : spot(s), left(nullptr), right(nullptr), height(1) {}
};

class AVLTree {
private:
    AVLNode* root;

    // Utility function to get height of the node
    int getHeight(AVLNode* node) const {
        return node ? node->height : 0;
    }

    // Utility function to get balance factor of node
    int getBalanceFactor(AVLNode* node) const {
        if (!node)
            return 0;
        return getHeight(node->left) - getHeight(node->right);
    }

    // Right rotate subtree rooted with y
    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

        // Return new root
        return x;
    }

    // Left rotate subtree rooted with x
    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        // Update heights
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

        // Return new root
        return y;
    }

    // Insert a ParkingSpot into the AVL tree
    AVLNode* insert(AVLNode* node, const ParkingSpot& spot) {
        // 1. Perform normal BST insertion
        if (!node)
            return new AVLNode(spot);

        if (spot.id < node->spot.id)
            node->left = insert(node->left, spot);
        else if (spot.id > node->spot.id)
            node->right = insert(node->right, spot);
        else // Duplicate Spot IDs not allowed
            return node;

        // 2. Update height of this ancestor node
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        // 3. Get the balance factor
        int balance = getBalanceFactor(node);

        // 4. Balance the tree

        // Left Left Case
        if (balance > 1 && spot.id < node->left->spot.id)
            return rightRotate(node);

        // Right Right Case
        if (balance < -1 && spot.id > node->right->spot.id)
            return leftRotate(node);

        // Left Right Case
        if (balance > 1 && spot.id > node->left->spot.id) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }

        // Right Left Case
        if (balance < -1 && spot.id < node->right->spot.id) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        // Return the unchanged node pointer
        return node;
    }

    // Find the node with minimum Spot ID value
    AVLNode* minValueNode(AVLNode* node) const {
        AVLNode* current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }

    // Delete a ParkingSpot from the AVL tree
    AVLNode* deleteNode(AVLNode* root, int spotID) {
        // Perform standard BST delete
        if (!root)
            return root;

        if (spotID < root->spot.id)
            root->left = deleteNode(root->left, spotID);
        else if (spotID > root->spot.id)
            root->right = deleteNode(root->right, spotID);
        else {
            // Node with only one child or no child
            if ((root->left == nullptr) || (root->right == nullptr)) {
                AVLNode* temp = root->left ? root->left : root->right;

                // No child case
                if (!temp) {
                    temp = root;
                    root = nullptr;
                }
                else // One child case
                    *root = *temp; // Copy the contents of the non-empty child

                delete temp;
            }
            else {
                // Node with two children: Get the inorder successor (smallest in the right subtree)
                AVLNode* temp = minValueNode(root->right);

                // Copy the inorder successor's data to this node
                root->spot = temp->spot;

                // Delete the inorder successor
                root->right = deleteNode(root->right, temp->spot.id);
            }
        }

        // If the tree had only one node then return
        if (!root)
            return root;

        // Update height
        root->height = 1 + max(getHeight(root->left), getHeight(root->right));

        // Get balance factor
        int balance = getBalanceFactor(root);

        // Balance the tree

        // Left Left Case
        if (balance > 1 && getBalanceFactor(root->left) >= 0)
            return rightRotate(root);

        // Left Right Case
        if (balance > 1 && getBalanceFactor(root->left) < 0) {
            root->left = leftRotate(root->left);
            return rightRotate(root);
        }

        // Right Right Case
        if (balance < -1 && getBalanceFactor(root->right) <= 0)
            return leftRotate(root);

        // Right Left Case
        if (balance < -1 && getBalanceFactor(root->right) > 0) {
            root->right = rightRotate(root->right);
            return leftRotate(root);
        }

        return root;
    }

    // Search for a ParkingSpot by Spot ID
    AVLNode* search(AVLNode* root, int spotID) const {
        if (!root || root->spot.id == spotID)
            return root;

        if (spotID < root->spot.id)
            return search(root->left, spotID);
        else
            return search(root->right, spotID);
    }

    }

public:
    AVLTree() : root(nullptr) {}

    // Insert a ParkingSpot into the AVL tree
    void insert(const ParkingSpot& spot) {
        root = insert(root, spot);
    }

    // Delete a ParkingSpot from the AVL tree by Spot ID
    void deleteSpot(int spotID) {
        root = deleteNode(root, spotID);
    }

    // Search for a ParkingSpot by Spot ID
    bool searchSpot(int spotID, ParkingSpot& foundSpot) const {
        AVLNode* result = search(root, spotID);
        if (result) {
            foundSpot = result->spot;
            return true;
        }
        return false;
    }

};
class Admin {
private:
    vector<string> &managerNames;
    int securityCode = 18041; 

    // Data structures to hold revenue
    stack<double> revenueStack;  
    queue<double> revenueQueue;  

public:
    Admin(vector<string> &names) : managerNames(names) {}

    // Add a new manager
    void addManager() {
        string name;
        cout << "=== Add New Manager ===\n";
        cout << "Enter Manager Name: ";
        cin >> name;
        managerNames.push_back(name);
        cout << "Added new manager: " << name << ".\n";
    }

    // Remove an existing manager
    void removeManager() {
        string name;
        cout << "=== Remove Manager ===\n";
        cout << "Enter Manager Name to Remove: ";
        cin >> name;

        // Convert to lowercase for case-insensitive comparison
        string lowerName = toLowerCase(name);

        auto it = find_if(managerNames.begin(), managerNames.end(),
                          [&lowerName, this](const string &manager) {
                              string lowerManager = toLowerCase(manager);
                              return lowerManager == lowerName;
                          });
        if (it != managerNames.end()) {
            cout << "Removed manager: " << *it << ".\n";
            managerNames.erase(it);
        } else {
            cout << "Manager " << name << " not found.\n";
        }
    }

    // Authenticate admin by security code
    bool authenticateAdmin() const {
        int enteredCode;
        cout << "=== Admin Authentication ===\n";
        cout << "Enter Admin Security Code: ";
        cin >> enteredCode;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
        if (enteredCode == securityCode) {
            cout << "Authentication successful.\n";
            return true;
        } else {
            cout << "Invalid security code.\n";
            return false;
        }
    }

    // Change the security code
    void changeSecurityCode() {
        int newCode;
        cout << "=== Change Security Code ===\n";
        cout << "Enter New Security Code: ";
        cin >> newCode;
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear input buffer
        securityCode = newCode;
        cout << "Security code updated successfully.\n";
    }

    // Record parking revenue in stack & queue
    void addRevenue(double amount) {
        revenueStack.push(amount);
        revenueQueue.push(amount);
    }

    // Display revenue in either FIFO or LIFO
    void displayRevenue() {
        cout << "\n=== Display Revenue ===\n";
        cout << "1. FIFO (Queue)\n";
        cout << "2. LIFO (Stack)\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            // Display FIFO
            if (revenueQueue.empty()) {
                cout << "No revenue recorded yet.\n";
                return;
            }
            queue<double> temp = revenueQueue;
            cout << "Revenue in FIFO order:\n";
            while (!temp.empty()) {
                cout << "$" << fixed << setprecision(2) << temp.front() << "\n";
                temp.pop();
            }
        }
        else if (choice == 2) {
            // Display LIFO
            if (revenueStack.empty()) {
                cout << "No revenue recorded yet.\n";
                return;
            }
            stack<double> temp = revenueStack;
            cout << "Revenue in LIFO order:\n";
            while (!temp.empty()) {
                cout << "$" << fixed << setprecision(2) << temp.top() << "\n";
                temp.pop();
            }
        }
        else {
            cout << "Invalid choice.\n";
        }
    }

private:
    // Helper function to convert string to lowercase
    string toLowerCase(const string& str) const {
        string lowerStr = str;
        transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
        return lowerStr;
    }
};
class SmartParkingManagement {
protected:
    vector<ParkingSpot> parkingSpots;
    unordered_map<int, pair<int, double>> reservations; // driverID -> (spotID, entryTime)
    list<pair<int, double>> entryExitLogs;              // (spotID, timestamp)
    vector<vector<int>> adjacencyMatrix;                // Graph representation
    AVLTree spotTree;                                  // AVL Tree for ParkingSpots

    // Helper function to convert string to lowercase
    string toLowerCase(const string& str) const {
        string lowerStr = str;
        for (char &ch : lowerStr) {
            ch = tolower(ch);
        }
        return lowerStr;
    }

    // Merge Sort implementation for sorting parking spots by proximity
    void sortSpotsByProximity() {
        if (!parkingSpots.empty()) {
            // Perform Merge Sort on the parkingSpots vector
            mergeSortSpots(parkingSpots, 0, static_cast<int>(parkingSpots.size()) - 1);
        }
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

    // Find the best-fit spot based on vehicle type (first one that can fit)
    int findBestFitSpot(VehicleType type) const {
        for (const auto &spot : parkingSpots) {
            if (spot.isAvailable && canFit(type, spot.size)) {
                return spot.id;
            }
        }
        return -1; // No suitable spot found
    }

    // Display the adjacency matrix
    void displayGraph() const {
        cout << "Parking Lot Graph (Adjacency Matrix):\n";
        for (const auto &row : adjacencyMatrix) {
            for (const auto &value : row) {
                cout << value << " ";
            }
            cout << "\n";
        }
    }

    // Check if a spot ID is valid using AVL Tree for efficient search
    bool isValidSpotID(int id) const {
        ParkingSpot dummy;
        return spotTree.searchSpot(id, dummy);
    }
public:
    // Constructor to initialize parking spots and adjacency matrix with deterministic sizes
    // Constructor
    SmartParkingManagement(int totalSpots, const vector<vector<int>> &graph) {
        adjacencyMatrix = graph;

        // Initialize parking spots with fixed sizes
        for (int i = 0; i < totalSpots; i++) {
            SlotSize size;
            if (i == 0) {
                size = SlotSize::REGULAR;
            } else if (i == 1 || i == 2) {
                size = SlotSize::COMPACT;
            } else {
                size = SlotSize::LARGE;
            }
            double distance = static_cast<double>(rand() % 100 + 1);
            double baseRate = 5.0;
            double ratePerHour = 3.0;

            ParkingSpot newSpot = {i, true, size, distance, baseRate, ratePerHour};
            parkingSpots.push_back(newSpot);
            spotTree.insert(newSpot); // Insert into AVL Tree
        }
        // Sort by proximity using merge sort
        sortSpotsByProximity();
    }

    // Getter for parkingSpots
    const vector<ParkingSpot>& getParkingSpots() const {
        return parkingSpots;
    }

    // Display available parking spots based on vehicle type
    virtual void displayAvailableSpots(VehicleType type) const {
        cout << "Available Parking Spots for ";
        switch (type) {
            case VehicleType::MOTORCYCLE:
                cout << "Motorcycle:\n"; break;
            case VehicleType::CAR:
                cout << "Car:\n"; break;
            case VehicleType::TRUCK:
                cout << "Truck:\n"; break;
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

    // Save parking data to a file
    void saveData() const {
        ofstream outFile("parking_data.txt");
        if (!outFile) {
            cerr << "Error opening file for writing.\n";
            return;
        }
        // Save parking spots
        for (const auto &spot : parkingSpots) {
            outFile << spot.id << "," << spot.isAvailable << "," 
                    << static_cast<int>(spot.size) << ","
                    << spot.distanceFromEntrance << ","
                    << spot.baseRate << ","
                    << spot.ratePerHour << "\n";
        }
        // Save reservations
        for (const auto &res : reservations) {
            outFile << res.first << "," << res.second.first << "," << res.second.second << "\n";
        }
        outFile.close();
        cout << "Data saved successfully.\n";
    }
// This function loads parking spot info and reservations from a file
    void loadData() {
        ifstream inFile("parking_data.txt");
        if (!inFile) {
            cout << "No existing data found. Starting fresh.\n";
            return;
        }
        parkingSpots.clear();
        spotTree = AVLTree(); // Reset the AVL Tree
        string line;

        // Temporary vector for reading lines
        vector<string> lines;
        while (getline(inFile, line)) {
            if (!line.empty()) {
                lines.push_back(line);
            }
        }
        inFile.close();

        // Parse parking spots
        int idx = 0;
        while (idx < lines.size()) {
            stringstream ss(lines[idx]);
            string token;
            vector<string> tokens;
            while (getline(ss, token, ',')) {
                tokens.push_back(token);
            }
            if (tokens.size() == 6) {
                // It's a parking spot
                int spotID = stoi(tokens[0]);
                bool isAvail = stoi(tokens[1]);
                SlotSize sz = static_cast<SlotSize>(stoi(tokens[2]));
                double dist = stod(tokens[3]);
                double bRate = stod(tokens[4]);
                double rHour = stod(tokens[5]);
                ParkingSpot newSpot = {spotID, isAvail, sz, dist, bRate, rHour};
                parkingSpots.push_back(newSpot);
                spotTree.insert(newSpot); // Insert into AVL Tree
                idx++;
            }
            else {
                break; // Move to reservations
            }
        }

        // Parse reservations
        reservations.clear();
        while (idx < lines.size()) {
            stringstream ss(lines[idx]);
            vector<string> tokens;
            string token;
            while (getline(ss, token, ',')) {
                tokens.push_back(token);
            }
            if (tokens.size() == 3) {
                int driverID = stoi(tokens[0]);
                int spotID = stoi(tokens[1]);
                double entryTime = stod(tokens[2]);
                reservations[driverID] = {spotID, entryTime};
                // Mark the spot as unavailable
                ParkingSpot foundSpot;
                if (spotTree.searchSpot(spotID, foundSpot)) {
                    // Update availability in vector
                    for (auto &spot : parkingSpots) {
                        if (spot.id == spotID) {
                            spot.isAvailable = false;
                            break;
                        }
                    }
                }
            }
            idx++;
        }

        cout << "Data loaded successfully.\n";
    }

};
class Driver : public SmartParkingManagement{
private:
    Admin* adminPtr;
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
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        auto it = reservations.find(driverID);
        if (it != reservations.end()) {
            int spotID = it->second.first;
            double entryTime = it->second.second;
            double exitTime = static_cast<double>(time(0));

            double duration = (exitTime - entryTime) / 3600.0; 
            if (duration < 0.0) duration = 0.0;

            // Find the parking spot
            ParkingSpot foundSpot;
            if (spotTree.searchSpot(spotID, foundSpot)) {
                // Update availability in vector
                for (auto &spot : parkingSpots) {
                    if (spot.id == spotID) {
                        spot.isAvailable = true;
                        break;
                    }
                }

                double fee = foundSpot.baseRate + (duration * foundSpot.ratePerHour);
                reservations.erase(it);
                entryExitLogs.emplace_back(spotID, exitTime);

                // Add fee to Admin's revenue structures
                if (adminPtr) {
                    adminPtr->addRevenue(fee);
                }

                cout << "Spot ID " << spotID << " released for Driver ID " << driverID << ".\n";
                cout << "Total Duration: " << fixed << setprecision(2) << duration << " hours\n";
                cout << "Parking Fee: $" << fixed << setprecision(2) << fee << "\n";
            }
            else {
                cout << "Error: Spot ID " << spotID << " not found in AVL Tree.\n";
            }
        }
        else {
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

        if (isValidSpotID(id)) {
            cout << "Spot ID " << id << " already exists.\n";
            return;
        }

        cout << "Select Spot Size:\n"
             << "1. Compact\n2. Regular\n3. Large\n"
             << "Enter your choice: ";
        while (!(cin >> sizeChoice) || sizeChoice < 1 || sizeChoice > 3) {
            cout << "Invalid input. Please enter a number between 1 and 3: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        SlotSize size;
        switch (sizeChoice) {
            case 1: size = SlotSize::COMPACT;  break;
            case 2: size = SlotSize::REGULAR;  break;
            case 3: size = SlotSize::LARGE;    break;
            default: size = SlotSize::REGULAR; break;
        }

        cout << "Enter Distance from Entrance (meters): ";
        cin >> distance;
        cout << "Enter Base Rate: $";
        cin >> baseRate;
        cout << "Enter Rate Per Hour: $";
        cin >> ratePerHour;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        ParkingSpot newSpot = {id, true, size, distance, baseRate, ratePerHour};
        parkingSpots.push_back(newSpot);
        spotTree.insert(newSpot); // Insert into AVL Tree
        // Re-sort after adding new spot
        sortSpotsByProximity();
        cout << "Added new parking spot with ID " << id << ".\n";
    }
void updateParkingSpot() {
        int id, availabilityChoice;
        cout << "=== Update Parking Spot ===\n";
        cout << "Enter Spot ID to Update: ";
        cin >> id;

        if (!isValidSpotID(id)) {
            cout << "Spot ID " << id << " does not exist.\n";
            return;
        }

        cout << "Set Availability:\n1. Available\n2. Occupied\n"
             << "Enter your choice: ";
        while (!(cin >> availabilityChoice) || (availabilityChoice != 1 && availabilityChoice != 2)) {
            cout << "Invalid input. Please enter 1 or 2: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        bool isAvail = (availabilityChoice == 1);
        // Update availability in vector
        for (auto &spot : parkingSpots) {
            if (spot.id == id) {
                spot.isAvailable = isAvail;
                break;
            }
        }
        cout << "Spot ID " << id << " updated to "
             << (isAvail ? "Available" : "Occupied") << ".\n";
    }

    // Public function to display the adjacency matrix
    void displayGraph() const {
        SmartParkingManagement::displayGraph();
    }
};
int main() {
    srand(static_cast<unsigned int>(time(0))); 

    int totalSpots;
    cout << "=== Smart Parking Management System ===\n";
    cout << "Enter total number of parking spots: ";
    while (!(cin >> totalSpots) || totalSpots <= 0) {
        cout << "Invalid input. Please enter a positive integer: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    // Create a simple adjacency matrix (fully connected for now)
    // Note: For a more meaningful adjacency matrix, consider implementing a grid or specific layout
    vector<vector<int>> graph(totalSpots, vector<int>(totalSpots, 1));

    // Manager names
    vector<string> managerNames = {"Alice", "Bob", "Charlie"};

    // Create Admin instance
    Admin admin(managerNames);

    // Create Driver, passing pointer to admin
    Driver driver(totalSpots, graph, &admin);

    // Create Manager
    ParkingLotManager manager(totalSpots, graph, managerNames);

    // Load existing data
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
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

        switch (choice) {
            case 1: {
                // Driver Menu
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
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');

                    switch (driverChoice) {
                        case 1: {
                            int vehicleChoice;
                            cout << "Select Vehicle Type:\n1. Motorcycle\n2. Car\n3. Truck\n"
                                 << "Enter your choice: ";
                            while (!(cin >> vehicleChoice) || vehicleChoice < 1 || vehicleChoice > 3) {
                                cout << "Invalid input. Please enter a number between 1 and 3: ";
                                cin.clear();
                                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            }
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            VehicleType vType = static_cast<VehicleType>(vehicleChoice);
                            driver.displayAvailableSpots(vType);
                            break;
                        }
                        case 2: {
                            driver.reserveSpot();
                            break;
                        }
                        case 3: {
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
                } while (driverChoice != 4);
                break;
            }
            case 2: {
                // Manager Menu
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
                        cout << "4. Display Graph\n"; // Added Display Graph option
                        cout << "5. Back to Main Menu\n";
                        cout << "Enter your choice: ";
                        while (!(cin >> managerChoice) || managerChoice < 1 || managerChoice > 5) {
                            cout << "Invalid input. Please enter a number between 1 and 5: ";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        switch (managerChoice) {
                            case 1: {
                                cout << "\n=== Available Parking Spots ===\n";
                                for (const auto &spot : manager.getParkingSpots()) {
                                    if (spot.isAvailable) {
                                        cout << "Spot ID: " << spot.id
                                             << ", Size: " << ((spot.size == SlotSize::COMPACT) ? "Compact" :
                                                              (spot.size == SlotSize::REGULAR) ? "Regular" : "Large")
                                             << ", Distance: " << spot.distanceFromEntrance << " meters\n";
                                    }
                                }
                                break;
                            }
                            case 2: {
                                manager.addParkingSpot();
                                break;
                            }
                            case 3: {
                                manager.updateParkingSpot();
                                break;
                            }
                            case 4: {
                                manager.displayGraph(); // Now accessible since displayGraph() is public
                                break;
                            }
                            case 5: {
                                cout << "Returning to Main Menu...\n";
                                break;
                            }
                            default:
                                cout << "Invalid choice. Please try again.\n";
                        }
                    } while (managerChoice != 5);
                }
                else {
                    cout << "Invalid manager name. Returning to Main Menu.\n";
                }
                break;
            }
            case 3: {
                // Admin Menu
                bool isAuthenticated = admin.authenticateAdmin();
                if (isAuthenticated) {
                    int adminChoice;
                    do {
                        cout << "\n=== Admin Menu ===\n";
                        cout << "1. Add Manager\n";
                        cout << "2. Remove Manager\n";
                        cout << "3. Change Security Code\n";
                        cout << "4. Display Revenue\n";
                        cout << "5. Back to Main Menu\n";
                        cout << "Enter your choice: ";
                        while (!(cin >> adminChoice) || adminChoice < 1 || adminChoice > 5) {
                            cout << "Invalid input. Please enter a number between 1 and 5: ";
                            cin.clear();
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        }
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        switch (adminChoice) {
                            case 1: {
                                admin.addManager();
                                break;
                            }
                            case 2: {
                                admin.removeManager();
                                break;
                            }
                            case 3: {
                                admin.changeSecurityCode();
                                break;
                            }
                            case 4: {
                                admin.displayRevenue();
                                break;
                            }
                            case 5: {
                                cout << "Returning to Main Menu...\n";
                                break;
                            }
                            default:
                                cout << "Invalid choice. Please try again.\n";
                        }
                    } while (adminChoice != 5);
                }
                else {
                    cout << "Authentication failed. Returning to Main Menu.\n";
                }
                break;
            }
            case 4: {
                // Exit
                driver.saveData();
                cout << "Exiting the system. Goodbye!\n";
                break;
            }
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 4);

    return 0;
}
