#include "CompanyHierarchy.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

// Function to read employees from CSV and build map
unordered_map<string, Emp> Hierarchy::readEmployeesFromCSV(const string& filename) {
    unordered_map<string, Emp> empMap;
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open " << filename << endl;
        return empMap;
    }

    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, role, managerId;
        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, role, ',');
        getline(ss, managerId, ',');

        Emp emp = {id, name, role, managerId};
        empMap[id] = emp;
    }

    file.close();
    return empMap;
}

// Build N-ary Tree from employee map
TreeNode* Hierarchy::buildHierarchyTree(const unordered_map<string, Emp>& empMap) {
    unordered_map<string, TreeNode*> localNodeMap;
    TreeNode* localRoot = nullptr;

    for (const auto& pair : empMap) {
        localNodeMap[pair.first] = new TreeNode{pair.second};
    }

    for (const auto& pair : empMap) {
        const string& empId = pair.first;
        const string& managerId = pair.second.managerId;

        if (managerId == "NULL") {
            localRoot = localNodeMap[empId];
        } else {
            localNodeMap[managerId]->subordinates.push_back(localNodeMap[empId]);
        }
    }

    return localRoot;
}

// Print hierarchy recursively
void Hierarchy::printHierarchy(TreeNode* root, int level) {
    if (!root) return;

    cout << string(level * 4, ' ') << root->emp.name << " (" << root->emp.role << ")\n";

    for (TreeNode* child : root->subordinates) {
        printHierarchy(child, level + 1);
    }
}

// Free memory
void Hierarchy::deleteTree(TreeNode* root) {
    if (!root) return;
    for (TreeNode* child : root->subordinates) {
        deleteTree(child);
    }
    delete root;
}

// Menu
void Hierarchy::showmenu() {
    int opt = -1;
    while (opt != 0) {
        cout << "\n--- Company Hierarchy Menu ---\n";
        cout << "1. Show Hierarchy\n";
        cout << "0. Back to Main Menu\n";
        cout << "Enter choice: ";
        cin >> opt;

        if (opt == 1) {
            unordered_map<string, Emp> empMap = readEmployeesFromCSV("employees.csv");

            if (empMap.empty()) {
                cout << "No employee data found.\n";
                continue;
            }

            TreeNode* hierarchyRoot = buildHierarchyTree(empMap);
            cout << "\n--- Company Hierarchy ---\n";
            printHierarchy(hierarchyRoot);

            deleteTree(hierarchyRoot); // Clean up
        } else if (opt == 0) {
            cout << "Returning to Main Menu...\n";
        } else {
            cout << "Invalid option!\n";
        }
    }
}

