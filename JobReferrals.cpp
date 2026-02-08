#include "JobReferrals.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string> 

using namespace std;

JobReferral::JobReferral() {	//constructor
    front = nullptr;
    rear = nullptr;
}

void JobReferral::clearQueue() {
    while (front != nullptr) {
        Referral* temp = front;
        front = front->next;
        delete temp;
    }
    rear = nullptr;
}

// Load referrals from file
void JobReferral::loadFromFile() {
	clearQueue();  // Clear existing referrals before loading new ones
	
    ifstream fin("referrals.csv");   //opens file for reading.
    if (!fin.is_open()) {
        cout << "Could not open referrals.csv.\n";
        return;
    }

    string line;
    getline(fin, line); // Skip header

    while (getline(fin, line)) {
        Referral* ref = new Referral;
        stringstream ss(line);     // breaks line into fields

        getline(ss, ref->EmpID, ',');		// extracts CSV values
        getline(ss, ref->EmpName, ',');
        getline(ss, ref->CanName, ',');
        getline(ss, ref->expertise, ',');
        getline(ss, ref->referralDate, '\n');
        ref->next = nullptr;

        if (rear == nullptr) {
            front = rear = ref;
        } else {
            rear->next = ref;
            rear = ref;
        }
    }
    fin.close();
}

// Save current referrals to file
void JobReferral::writeToFile() {
    ofstream fout("referrals.csv");
    fout << "Referrer ID,Referrer Name,Candidate Name,Expertise,Referral Date\n";

    Referral* temp = front;
    while (temp != nullptr) {
        fout << temp->EmpID << "," << temp->EmpName << "," << temp->CanName << ","
             << temp->expertise << "," << temp->referralDate << "\n";
        temp = temp->next;
    }
    fout.close();
}

// Add a new referral
void JobReferral::EnQueue() {
    Referral* ref = new Referral;

    cout << "EMPLOYEE DETAILS:\nEnter Employee ID: ";
    cin >> ref->EmpID;
    cin.ignore();
    cout << "Enter Employee Name: ";
    getline(cin, ref->EmpName);

    cout << "CANDIDATE DETAILS:\nEnter Candidate Name: ";
    getline(cin, ref->CanName);

    cout << "Enter Candidate Expertise: ";
    getline(cin, ref->expertise);

    cout << "Enter Referral Date: ";
    getline(cin, ref->referralDate);

    ref->next = nullptr;

    if (rear == nullptr) {
        front = rear = ref;
    } else {
        rear->next = ref;
        rear = ref;
    }

    cout << "Referral added!\n";
    writeToFile(); // Save immediately
    print();
}

// Remove the front referral
bool JobReferral::DeQueue() {
    if (front == nullptr) {
        cout << "Referral underflow!\n";
        return false;
    }

    Referral* temp = front;
    front = front->next;
    delete temp;

    if (front == nullptr) rear = nullptr;

    writeToFile();
    return true;
}

// Read and process front referral
void JobReferral::read() {
    if (front == nullptr) {
        cout << "No Referrals!\n";
        return;
    }

    cout << "\n EMPLOYEE REFERRAL FORM:\n\n";
    cout << "EMPLOYEE DETAILS:\n";
    cout << "ID: " << front->EmpID << endl;
    cout << "Name: " << front->EmpName << endl;

    cout << "\nCANDIDATE DETAILS:\n";
    cout << "NAME: " << front->CanName << endl;
    cout << "EXPERTISE: " << front->expertise << endl;

    cout << "\nREFERRAL DATE: " << front->referralDate << endl;

    DeQueue(); // Automatically remove after processing
}

// Display all referrals
void JobReferral::print() {
    if (front == nullptr) {
        cout << "Referral queue is empty!\n";
        return;
    }

    Referral* temp = front;
    cout << "\n JOB REFERRALS:\n\n";
    while (temp != nullptr) {
        cout << "EMPLOYEE DETAILS:\n";
        cout << "ID: " << temp->EmpID << "\nName: " << temp->EmpName << "\n";

        cout << "\nCANDIDATE DETAILS:\n";
        cout << "NAME: " << temp->CanName << "\nEXPERTISE: " << temp->expertise << "\n";

        cout << "\nREFERRAL DATE: " << temp->referralDate << "\n";
        cout << "--------------------------------------\n";
        temp = temp->next;
    }
}

// Show menu
void JobReferral::showmenu() {
    loadFromFile();
    int opt = -1;
    while (opt != 0) {
        cout << "\n--- Job Referral System ---\n";
        cout << "1. Add New Referral (EnQueue)\n";
        cout << "2. Read and Process Front Referral\n";
        cout << "3. View All Referrals\n";
        cout << "0. Back \nEnter choice: ";
        cin >> opt;

        switch (opt) {
            case 1: EnQueue(); break;
            //case 2: DeQueue(); break;
            case 2: read(); break;
            case 3: print(); break;
            case 0: cout << "Returning to Main Menu...\n"; break;
            default: cout << "Invalid option!\n";
        }
    }
}

