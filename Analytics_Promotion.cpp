#include "Analytics_Promotion.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

vector<Employe> Promo::loadEmployees() {
    vector<Employe> employees;
    ifstream file("employees.csv");
    string line, id, name, role, managerId;

    getline(file, line); // skip header
    while (getline(file, line)) {
        stringstream ss(line);
        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, role, ',');
        getline(ss, managerId, ',');

        employees.push_back(Employe(id, name, role, managerId));
    }
    file.close();
    return employees;
}

void Promo::loadProjectScores(vector<Employe>& employees) {
    ifstream file("projects.csv");
    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, projects, avgScore, projectCount, performanceScore, finalScore;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, projects, '"'); // Skip opening quote
        getline(ss, projects, '"'); // Actual projects string
        ss.ignore(); // skip the comma after closing quote

        getline(ss, avgScore, ',');
        getline(ss, projectCount, ',');
        getline(ss, performanceScore, ',');
        getline(ss, finalScore, ',');

        for (auto& emp : employees) {
            if (emp.id == id) {
                emp.finalScore = stoi(finalScore);
                break;
            }
        }
    }
    file.close();
}

void Promo::loadReviewScores(vector<Employe>& employees) {
    ifstream file("reviews.csv");
    string line;
    getline(file, line); // Skip header

    while (getline(file, line)) {
        stringstream ss(line);
        string id, name, given, received, avg;

        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, given, ',');
        getline(ss, received, ',');
        getline(ss, avg, ',');

        for (auto& emp : employees) {
            if (emp.id == id) {
                emp.avgReview = stoi(avg);
                break;
            }
        }
    }
    file.close();
}

void Promo::saveLeaderboardToCSV(const vector<Employe>& employees) {
    ofstream fout("analytics.csv");
    fout << "EmpID,Name,Role,FinalScore,AvgReview,TotalScore\n";

    for (const auto& emp : employees) {
        fout << emp.id << "," << emp.name << "," << emp.role << ","
             << emp.finalScore << "," << emp.avgReview << "," << emp.totalScore << "\n";
    }

    fout.close();
    cout << "\nLeaderboard saved to analytics.csv \n";
}

void Promo::calculateTotalScores(vector<Employe>& employees) {
    for (auto& emp : employees) {
        emp.totalScore = emp.finalScore + emp.avgReview;
        cout << "Employee: " << emp.name
             << ", Final Score: " << emp.finalScore
             << ", Avg Review: " << emp.avgReview
             << ", Total: " << emp.totalScore << endl;
    }
}

void Promo::showLeaderboard(vector<Employe> employees) {
    sort(employees.begin(), employees.end(), [](const Employe& a, const Employe& b) {
        return a.totalScore > b.totalScore;
    });

    cout << "\n--- Employee Leaderboard ---\n";
    cout << "ID\tName\t\tScore\tRole\n";
    for (const auto& emp : employees) {
        cout << emp.id << "\t" << emp.name << "\t\t" << emp.totalScore << "\t" << emp.role << "\n";
    }

    saveLeaderboardToCSV(employees);
}

void Promo::promoteEmployees(vector<Employe>& employees) {
    cout << "\n--- Promoting Top Performers ---\n";
    for (auto& emp : employees) {
        if (emp.totalScore >= 20) {
            cout << emp.name << " (" << emp.id << ") promoted from " << emp.role;
            if (emp.role == "Junior Engineer") emp.role = "Senior Engineer";
            else if (emp.role == "Senior Engineer") emp.role = "Lead Engineer";
            else if (emp.role == "Lead Engineer") emp.role = "Manager";
            else if (emp.role == "Manager") emp.role = "Senior Manager";
            else if (emp.role == "Head") emp.role = "Head";
            else emp.role = "Executive";
            cout << " to " << emp.role << " | Bonus: " << emp.totalScore * 100 << "\n";
        }
    }

    ofstream fout("employees.csv");
    fout << "EmpID,Name,Role,ManagerID\n";
    for (auto& emp : employees) {
        fout << emp.id << "," << emp.name << "," << emp.role << "," << emp.managerId << "\n";
    }
    fout.close();
    cout << "\nEmployee roles updated in employees.csv\n";
}

void Promo::showmenu() {
    vector<Employe> employees = loadEmployees();
    loadProjectScores(employees);
    loadReviewScores(employees);
    calculateTotalScores(employees);

    int choice;
    do {
        cout << "\n--- Analytics & Promotion System ---\n";
        cout << "1. View Employee Leaderboard\n";
        cout << "2. View Promotion Criteria\n";
        cout << "3. Do Promotion\n";
        cout << "0. Back\nEnter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: showLeaderboard(employees); break;
            case 2: cout << "\nEmployees with Total Score >= 20 are eligible for promotion.\n"; break;
            case 3: promoteEmployees(employees); break;
            case 0:
                cout << "Exiting...\nReturning to Main Menu...\n";
                break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);
}

