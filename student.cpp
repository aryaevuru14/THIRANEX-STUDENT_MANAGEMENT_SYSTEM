#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <limits>
#include <algorithm>

class Student {
private:
    int id;
    std::string name;
    int age;
    double gpa;

public:
    // Constructors
    Student() : id(0), name(""), age(0), gpa(0.0) {}
    Student(int id, std::string name, int age, double gpa) 
        : id(id), name(std::move(name)), age(age), gpa(gpa) {}

    // Getters
    [[nodiscard]] int getId() const { return id; }
    [[nodiscard]] std::string getName() const { return name; }
    [[nodiscard]] int getAge() const { return age; }
    [[nodiscard]] double getGpa() const { return gpa; }

    // Setters
    void setName(const std::string& newName) { name = newName; }
    void setAge(int newAge) { age = newAge; }
    void setGpa(double newGpa) { gpa = newGpa; }

    // Serialize for file storage
    [[nodiscard]] std::string toCSV() const {
        return std::to_string(id) + "," + name + "," + std::to_string(age) + "," + std::to_string(gpa);
    }
};

class StudentManager {
private:
    std::vector<Student> database;
    const std::string dbFilename = "students_db.csv";

    // Helper: Clears input buffer to prevent infinite loops on invalid input
    void clearInputBuffer() const {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    // Helper: Prompts for a valid integer
    int getValidInt(const std::string& prompt) const {
        int value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value) {
                clearInputBuffer();
                return value;
            }
            std::cout << "[-] Invalid input. Please enter a valid integer.\n";
            clearInputBuffer();
        }
    }

    // Helper: Prompts for a valid double
    double getValidDouble(const std::string& prompt) const {
        double value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value) {
                clearInputBuffer();
                return value;
            }
            std::cout << "[-] Invalid input. Please enter a valid number.\n";
            clearInputBuffer();
        }
    }

    // Core File I/O: Load data from disk
    void loadDatabase() {
        std::ifstream file(dbFilename);
        if (!file.is_open()) return;

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) continue;
            
            std::stringstream ss(line);
            std::string token;
            std::vector<std::string> tokens;
            
            while (std::getline(ss, token, ',')) {
                tokens.push_back(token);
            }

            if (tokens.size() == 4) {
                try {
                    int id = std::stoi(tokens[0]);
                    std::string name = tokens[1];
                    int age = std::stoi(tokens[2]);
                    double gpa = std::stod(tokens[3]);
                    database.emplace_back(id, name, age, gpa);
                } catch (const std::exception&) {
                    // Skip corrupted lines
                    continue;
                }
            }
        }
    }

    // Core File I/O: Save data to disk
    void saveDatabase() const {
        std::ofstream file(dbFilename, std::ios::trunc);
        if (!file.is_open()) {
            std::cerr << "[-] Critical Error: Could not write to database file.\n";
            return;
        }

        for (const auto& student : database) {
            file << student.toCSV() << "\n";
        }
    }

    // Helper: Find index of student by ID
    [[nodiscard]] int findStudentIndex(int id) const {
        auto it = std::find_if(database.begin(), database.end(), 
            [id](const Student& s) { return s.getId() == id; });
        
        if (it != database.end()) {
            return std::distance(database.begin(), it);
        }
        return -1;
    }

public:
    StudentManager() {
        loadDatabase();
    }

    ~StudentManager() {
        saveDatabase(); // Ensure data is flushed on exit
    }

    void addStudent() {
        std::cout << "\n--- Add New Student ---\n";
        int id = getValidInt("Enter Student ID: ");
        
        if (findStudentIndex(id) != -1) {
            std::cout << "[-] Error: A student with ID " << id << " already exists.\n";
            return;
        }

        std::cout << "Enter Full Name: ";
        std::string name;
        std::getline(std::cin, name);

        int age = getValidInt("Enter Age: ");
        double gpa = getValidDouble("Enter GPA (0.0 - 4.0): ");

        database.emplace_back(id, name, age, gpa);
        saveDatabase();
        std::cout << "[+] Student '" << name << "' added successfully.\n";
    }

    void displayAllStudents() const {
        if (database.empty()) {
            std::cout << "\n[-] The database is currently empty.\n";
            return;
        }

        std::cout << "\n=========================================================\n";
        std::cout << std::left << std::setw(10) << "ID" 
                  << std::setw(25) << "Full Name" 
                  << std::setw(10) << "Age" 
                  << std::setw(10) << "GPA" << "\n";
        std::cout << "=========================================================\n";
        
        for (const auto& s : database) {
            std::cout << std::left << std::setw(10) << s.getId() 
                      << std::setw(25) << s.getName() 
                      << std::setw(10) << s.getAge() 
                      << std::setw(10) << std::fixed << std::setprecision(2) << s.getGpa() << "\n";
        }
        std::cout << "=========================================================\n";
    }

    void updateStudent() {
        if (database.empty()) {
            std::cout << "\n[-] The database is currently empty.\n";
            return;
        }

        int id = getValidInt("\nEnter Student ID to update: ");
        int index = findStudentIndex(id);

        if (index == -1) {
            std::cout << "[-] Error: Student ID not found.\n";
            return;
        }

        std::cout << "Updating record for: " << database[index].getName() << "\n";
        
        std::cout << "Enter New Full Name (or press Enter to keep current): ";
        std::string newName;
        std::getline(std::cin, newName);
        if (!newName.empty()) {
            database[index].setName(newName);
        }

        int newAge = getValidInt("Enter New Age: ");
        database[index].setAge(newAge);

        double newGpa = getValidDouble("Enter New GPA: ");
        database[index].setGpa(newGpa);

        saveDatabase();
        std::cout << "[+] Student record updated successfully.\n";
    }

    void deleteStudent() {
        if (database.empty()) {
            std::cout << "\n[-] The database is currently empty.\n";
            return;
        }

        int id = getValidInt("\nEnter Student ID to delete: ");
        int index = findStudentIndex(id);

        if (index == -1) {
            std::cout << "[-] Error: Student ID not found.\n";
            return;
        }

        std::cout << "[+] Student '" << database[index].getName() << "' has been deleted.\n";
        database.erase(database.begin() + index);
        saveDatabase();
    }
};

int main() {
    StudentManager manager;
    int choice = 0;

    while (true) {
        std::cout << "\n=====================================\n";
        std::cout << "  SYSTEM ADMINISTRATION DASHBOARD    \n";
        std::cout << "=====================================\n";
        std::cout << "1. Add New Student Record\n";
        std::cout << "2. View All Student Records\n";
        std::cout << "3. Update Existing Record\n";
        std::cout << "4. Delete Student Record\n";
        std::cout << "5. Terminate Session\n";
        std::cout << "=====================================\n";
        
        std::cout << "Select Operation (1-5): ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "[-] Invalid input type. Numbers only.\n";
            continue;
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear buffer for next inputs

        switch (choice) {
            case 1: manager.addStudent(); break;
            case 2: manager.displayAllStudents(); break;
            case 3: manager.updateStudent(); break;
            case 4: manager.deleteStudent(); break;
            case 5: 
                std::cout << "\n[+] Data saved. Terminating session...\n"; 
                return 0;
            default: 
                std::cout << "[-] Unrecognized command. Select 1-5.\n";
        }
    }
}