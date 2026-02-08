#include "PeerReviews.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
using namespace std;

// Define the global head pointer once
Vertex* head = nullptr;

void Reviews::loadEmployees() {
    ifstream fin("employees.csv");
    string id, name, role, managerId;
    string line;

    getline(fin, line); // skip header
    while (getline(fin, line)) {
        stringstream ss(line);
        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, role, ',');
        getline(ss, managerId, ',');

        Vertex* newEmp = new Vertex{id, name, nullptr, nullptr};
        if (!head) head = newEmp;
        else {
            Vertex* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newEmp;
        }
    }
    fin.close();
}

void Reviews::writeReviewsToCSV() {
    ofstream fout("reviews.csv");
    fout << "ID,Name,Reviews Given,Reviews Received,Average Received Score\n";

    Vertex* emp = head;
    while (emp) {
        // Reviews Given
        string reviewsGiven = "";
        Adj* given = emp->startAdj;
        while (given) {
            reviewsGiven += "To " + given->adjvert->id + ":" + to_string(given->weight) + " ";
            given = given->anext;
        }

        // Reviews Received
        string reviewsReceived = "";
        int total = 0, count = 0;

        Vertex* reviewer = head;
        while (reviewer) {
            Adj* adj = reviewer->startAdj;
            while (adj) {
                if (adj->adjvert == emp) {
                    reviewsReceived += "From " + reviewer->id + ":" + to_string(adj->weight) + " ";
                    total += adj->weight;
                    count++;
                }
                adj = adj->anext;
            }
            reviewer = reviewer->next;
        }

        int avg = (count == 0) ? 0 : total / count;

        fout << emp->id << "," << emp->name << ","
             << "\"" << reviewsGiven << "\","
             << "\"" << reviewsReceived << "\","
             << avg << "\n";

        emp = emp->next;
    }
    fout.close();
    cout << " Reviews saved to reviews.csv\n";
}

void Reviews::saveLinks() {
    ofstream fout("peerlinks.csv");
    Vertex* emp = head;
    while (emp) {
        Adj* adj = emp->startAdj;
        while (adj) {
            fout << emp->id << "," << adj->adjvert->id << "," << adj->weight << "\n";
            adj = adj->anext;
        }
        emp = emp->next;
    }
    fout.close();
}

void Reviews::loadLinks() {
    ifstream fin("peerlinks.csv");
    if (!fin.is_open()) return;

    string line;
    while (getline(fin, line)) {
        stringstream ss(line);
        string from, to, weightStr;

        getline(ss, from, ',');
        getline(ss, to, ',');
        getline(ss, weightStr); // instead of ss >> weight

        if (from.empty() || to.empty() || weightStr.empty())
            continue;

        int weight = stoi(weightStr);

        Vertex* source = findVertex(from);
        Vertex* dest = findVertex(to);
        if (!source || !dest) continue;

        // Check for duplicate review before inserting
        Adj* check = source->startAdj;
        bool exists = false;
        while (check) {
            if (check->adjvert == dest) {
                exists = true;
                break;
            }
            check = check->anext;
        }
        if (exists) continue;  // Skip if already reviewed

        Adj* newAdj = new Adj{dest, nullptr, weight};

        if (!source->startAdj) {
            source->startAdj = newAdj;
        } else {
            Adj* temp = source->startAdj;
            while (temp->anext) temp = temp->anext;
            temp->anext = newAdj;
        }
    }

    fin.close();
}

Vertex* Reviews::findVertex(string data) {
    Vertex* temp = head;
    while (temp != nullptr && temp->id != data)
        temp = temp->next;
    return temp;
}

void Reviews::giveReviews() {
    string s, d;
    int points;

    cout << "Enter Source Employee ID     : ";   cin >> s;
    Vertex* sourceEmp = findVertex(s);
    if(sourceEmp == nullptr){
        cout << "Source Employee not found\n";
        return;
    }

    cout << "Enter Destination Employee ID: ";   cin >> d;
    Vertex* destEmp = findVertex(d);
    if(destEmp == nullptr){
        cout<<"Destination Employee not found\n";
        return;
    }
    if (sourceEmp == destEmp) { // no self-reviews
        cout << "Self-reviews are not allowed.\n";
        return;
    }

    cout << "Select Rating Between 1 To 10: "; cin >> points;
    if (points < 0 || points > 10) {
        cout << "Invalid score. Must be between 0-10.\n";
        return;
    }

    Adj* ptr = new Adj{destEmp, nullptr, points};

    if(sourceEmp->startAdj == nullptr){
        sourceEmp->startAdj = ptr;
    }
    else{
        Adj* temp = sourceEmp->startAdj;
        while(temp->anext != nullptr){
            temp = temp->anext;
        }
        temp->anext = ptr;
    }
    cout<< "\n Reviews Submitted! \n";
}

void Reviews::displayReviews() {
    Vertex* emp = head;
    while (emp) {
        cout << "\nEmployee: " << emp->id << " - " << emp->name << "\n";

        // Reviews Given
        cout << "  Reviews Given:\n";
        if (!emp->startAdj) {
            cout << "    None\n";
        } else {
            Adj* given = emp->startAdj;
            while (given) {
                cout << "    To " << given->adjvert->id << " - " << given->adjvert->name
                     << ": " << given->weight << "\n";
                given = given->anext;
            }
        }

        // Reviews Received
        cout << "  Reviews Received:\n";
        int total = 0, count = 0;
        Vertex* reviewer = head;
        bool hasReviews = false;
        while (reviewer) {
            Adj* adj = reviewer->startAdj;
            while (adj) {
                if (adj->adjvert == emp) {
                    cout << "    From " << reviewer->id << " - " << reviewer->name
                         << ": " << adj->weight << "\n";
                    total += adj->weight;
                    count++;
                    hasReviews = true;
                }
                adj = adj->anext;
            }
            reviewer = reviewer->next;
        }

        if (!hasReviews) {
            cout << "    None\n";
        } else {
            int avg = (count == 0) ? 0 : total / count;
            cout << " Average Received Score: " << avg << "\n";
        }

        emp = emp->next;
    }
}

void Reviews::clearData() {
    Vertex* temp = head;
    while (temp) {
        Adj* adj = temp->startAdj;
        while (adj) {
            Adj* nextAdj = adj->anext;
            delete adj;
            adj = nextAdj;
        }
        Vertex* next = temp->next;
        delete temp;
        temp = next;
    }
    head = nullptr;
}

void Reviews::showmenu() {
	clearData();        // <-- Clear old data before loading new
    int choice = -1;
    loadEmployees();
    loadLinks();

    while (choice != 0){
        cout << "\n--- Peer Review System ---\n 1. Give Review\n 2. Display All Reviews\n";
        cout << "0. Back To Main\n Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                giveReviews();
                saveLinks();
                writeReviewsToCSV();
                break;
            case 2:
                displayReviews();
                break;
            case 0:
                writeReviewsToCSV();
                saveLinks();
                cout << "Reviews saved.\n";
                cout << "Returning to Main Menu...\n";
                break;
            default:
                cout << "Invalid choice!\n";
                break;
        }
    }
    clearData();  
}

