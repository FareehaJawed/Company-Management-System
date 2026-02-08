#include "Announcement.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

// Function to print announcements
void Announce::print() {
    Stack* temp = top;
    cout << "\n ANNOUNCEMENTS\n\n";

    if (top == NULL) {
        cout << " No announcements today!\n";
        return;
    }

    while (temp != NULL) {
        cout << "-> " << temp->data << "\n\n";
        temp = temp->next;
    }
}

// Push function to add to stack and file
void Announce::push() {
    string value, line;
    cout << "Enter multi-line announcement (enter an empty line to finish):\n";

    while (true) {
        getline(cin, line);
        if (line.empty()) break;
        if (!value.empty()) value += "\n";
        value += line;
    }

    // Push to stack
    Stack* ptr = new Stack;
    ptr->data = value;
    ptr->next = top;
    top = ptr;

    //print();
    cout << "Announcement added successfully.\n";
}

// Clear memory
void Announce::clear() {
    while (top != NULL) {
        Stack* temp = top;
        top = top->next;
        delete temp;
    }
}

// Save announcements to file
void Announce::saveToFile() {
    ofstream fout("announcements.txt");
    Stack* temp = top;

    while (temp != NULL) {
        fout << temp->data << "\n\n";  
        temp = temp->next;
    }
    fout.close();
}

// Load announcements from file on startup
void Announce::loadFromFile() {
    ifstream fin("announcements.txt");
    if (!fin) return;

    string line, announcement;
    vector<string> announcements;

    while (getline(fin, line)) {
        if (line.empty()) {
            if (!announcement.empty()) {
                announcements.push_back(announcement);
                announcement.clear();
            }
        } else {
            if (!announcement.empty()) announcement += "\n";
            announcement += line;
        }
    }
    if (!announcement.empty()) {
        announcements.push_back(announcement);
    }

    for (int i = announcements.size() - 1; i >= 0; --i) {
        Stack* ptr = new Stack;
        ptr->data = announcements[i];
        ptr->next = top;
        top = ptr;
    }
    fin.close();
}

// Menu to show options
void Announce::showmenu() {
    clear();            // Clear any existing stack data
    loadFromFile();     // Load clean from file

    int opt = -1;
    while (opt != 0) {
        cout << "\n1. Make Announcement\n2. View All Announcements\n0. Back\n Enter choice: ";
        cin >> opt;
        cin.ignore();

        switch (opt) {
            case 1: push(); break;
            case 2: print(); break;
            case 0:
                cout << "Saving.\n Returning to Main Menu...\n ";
                saveToFile();
                break;
            default:
                cout << "Invalid option!\n";
        }
    }

    saveToFile();  // Optional double-save protection
    clear();       // Clean up memory when done
}


