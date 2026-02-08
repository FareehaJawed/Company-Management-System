#include "EmployeeManagement.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>

using namespace std;

void Management::saveToCSV(const unordered_map<string, Employee>& empMap) {
    ofstream file("employees.csv");
    if (!file.is_open()) {
        cout << "Failed to open file for writing.\n";
        return;
    }

    file << "ID,Name,Role,ManagerID\n";

    for (auto it = empMap.begin(); it != empMap.end(); ++it) {
        const Employee& emp = it->second;
        file << emp.id << "," 
             << emp.name << "," 
             << emp.role << "," 
             << emp.managerId << "\n";
    }

    file.close();
    cout << "Employee data saved to employees.csv\n";
}

void Management::loadFromCSV(unordered_map<string, Employee>& empMap) {
    ifstream file("employees.csv");
    if (!file.is_open()) {
        cout << "?? No existing employees.csv file found. Starting fresh.\n";
        return;
    }
    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, role, managerId;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, role, ',');
        getline(ss, managerId);

        empMap[id] = {id, name, role, managerId};
    }
    file.close();
    cout << "Employee data loaded from employees.csv\n";
}

void Management::insertKey(unordered_map<string, Employee>& empMap) {
    string i, n, r, m;

    cout << "Enter Employee ID    : ";
    cin >> i;
    cin.ignore();  			//Clear leftover newline

    cout << "Enter Employee Name  : ";
    getline(cin, n);  		//Allows full name with spaces

    cout << "Enter Employee Role  : ";
    getline(cin, r);  

    cout << "Enter Manager ID (or 'NULL'): ";
    cin >> m;
    
    if (empMap.find(i) != empMap.end()) {
        cout << "Employee ID already exists!\n";
        return;
    }

    if (m != "NULL" && empMap.find(m) == empMap.end()) {
        cout << "Invalid Manager ID: " << m << "\n";
        return;
    }

    Employee emp = {i, n, r, m};
    empMap[i] = emp;
    cout << "Employee added successfully.\n";
}

void Management::searchKey(const unordered_map<string, Employee>& empMap) {
    string i;
    cout << "Enter Employee ID : "; cin >> i;

    auto it = empMap.find(i);

    if (it != empMap.end()) {
        cout << "Employee exists!\n";
        const Employee& emp = it->second;
        cout << "ID        : " << emp.id << "\n"
             << "Name      : " << emp.name << "\n"
             << "Role      : " << emp.role << "\n"
             << "Manager ID: " << emp.managerId << "\n";
    } else {
        cout << "Employee does not exist!\n";
    }
}

void Management::deleteKey(unordered_map<string, Employee>& empMap) {
    string i;
    cout << "Enter Employee ID : "; cin >> i;

    auto it = empMap.find(i);

    if (it != empMap.end()) {
        empMap.erase(it);
        cout << "Employee Deleted\n";
    } else {
        cout << "Employee does not exist!\n";
    }
}

void Management::printKey(const unordered_map<string, Employee>& empMap) {
    if (empMap.empty()) {
        cout << "No employees found.\n";
        return;
    }

    for (const auto& pair : empMap) {
        const Employee& emp = pair.second;
        cout << "ID        : " << emp.id << "\n"
             << "Name      : " << emp.name << "\n"
             << "Role      : " << emp.role << "\n"
             << "Manager ID: " << emp.managerId << "\n"
             << "-------------------------\n";
    }
}

void Management::showmenu() {
    unordered_map<string, Employee> empDB;
    loadFromCSV(empDB);

    int opt = -1;
    while (opt != 0) {
        cout << "\n1. Insert\n2. Search\n3. Delete\n4. Print\n0. Back To Main\nEnter choice: ";
        cin >> opt;
        switch (opt) {
            case 1: insertKey(empDB); break;
            case 2: searchKey(empDB); break;
            case 3: deleteKey(empDB); break;
            case 4: printKey(empDB); break;
            case 0:
                saveToCSV(empDB);
                cout << "Returning to Main Menu...\n";
                break;
            default: cout << "Invalid option!\n";
        }
    }
}

