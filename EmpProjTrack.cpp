#include "EmpProjTrack.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdexcept>

using namespace std;

static const int PROJECT_COUNT = 5;
static const ProjectDef projectDefs[PROJECT_COUNT] = {
    {"AI Model", 10},
    {"UI Redesign", 8},
    {"Bug Fixing", 5},
    {"Data Cleanup", 6},
    {"API Integration", 7}
};

int Tracking::projectCount(Employ* emp) {
    int count = 0;
    Project* p = emp->projHead;
    while (p) {
        count++;
        p = p->pnext;
    }
    return count;
}

int Tracking::getScoreFromName(const string& pname) {
    for (int i = 0; i < PROJECT_COUNT; ++i) {
        if (projectDefs[i].pname == pname)
            return projectDefs[i].score;
    }
    return 5; // Default
}

Employ* Tracking::loadEmployees(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "Error: Could not open " << filename << endl;
        return nullptr;
    }

    Employ* head = nullptr;
    Employ* tail = nullptr;

    string line;
    getline(file, line);

    while (getline(file, line)) {
        stringstream ss(line);
        string empId, name, dummy;

        getline(ss, empId, ',');
        getline(ss, name, ',');
        getline(ss, dummy, ',');
        getline(ss, dummy, ',');

        Employ* newEmp = new Employ{empId, name, nullptr, nullptr};
        if (!head) {
            head = newEmp;
            tail = newEmp;
        } else {
            tail->next = newEmp;
            tail = newEmp;
        }
    }
    file.close();
    return head;
}

void Tracking::loadProjects(const string& filename, Employ* head) {
    ifstream file(filename);
    if (!file) return;

    string line;
    getline(file, line);

    while (getline(file, line)) {
        string empId, name, projectsField;
        int pos = 0;

        size_t comma1 = line.find(',', pos);
        if (comma1 == string::npos) continue;
        empId = line.substr(pos, comma1 - pos);
        pos = comma1 + 1;

        size_t comma2 = line.find(',', pos);
        if (comma2 == string::npos) continue;
        name = line.substr(pos, comma2 - pos);
        pos = comma2 + 1;

        bool quoted = line[pos] == '"';
        size_t endQuote;
        if (quoted) {
            pos++;
            endQuote = line.find('"', pos);
            if (endQuote == string::npos) continue;
            projectsField = line.substr(pos, endQuote - pos);
        } else {
            size_t comma3 = line.find(',', pos);
            projectsField = line.substr(pos, comma3 - pos);
        }

        Employ* emp = findEmployeeById(head, empId);
        if (!emp) continue;

        stringstream projStream(projectsField);
        string projEntry;
        while (getline(projStream, projEntry, ',')) {
            size_t colon = projEntry.find(':');
            if (colon == string::npos) continue;

            string pname = projEntry.substr(0, colon);
            int score = stoi(projEntry.substr(colon + 1));

            Project* temp = emp->projHead;
            bool exists = false;
            while (temp) {
                if (temp->pname == pname) {
                    exists = true;
                    break;
                }
                temp = temp->pnext;
            }
            if (exists) continue;

            Project* newProj = new Project{pname, score, nullptr};
            if (!emp->projHead) emp->projHead = newProj;
            else {
                Project* last = emp->projHead;
                while (last->pnext) last = last->pnext;
                last->pnext = newProj;
            }
        }
    }
    file.close();
}

void Tracking::assignProjectToEmployee(Employ* head) {
    string empId;
    cout << "Enter Employee ID: ";
    cin >> empId;
    cin.ignore();

    Employ* emp = findEmployeeById(head, empId);
    if (!emp) {
        cout << "Employee not found.\n";
        return;
    }

    cout << "Available Projects:\n";
    for (int i = 0; i < PROJECT_COUNT; ++i) {
        cout << " " << (i + 1) << ". " << projectDefs[i].pname
             << " (Score: " << projectDefs[i].score << ")\n";
    }

    int choice;
    cout << "Enter project number to assign: ";
    cin >> choice;
    cin.ignore();

    if (choice < 1 || choice > PROJECT_COUNT) {
        cout << "Invalid project choice.\n";
        return;
    }

    string pname = projectDefs[choice - 1].pname;
    int score = projectDefs[choice - 1].score;

    Project* temp = emp->projHead;
    while (temp) {
        if (temp->pname == pname) {
            cout << "Project already assigned.\n";
            return;
        }
        temp = temp->pnext;
    }

    Project* newProj = new Project{pname, score, nullptr};
    if (!emp->projHead) emp->projHead = newProj;
    else {
        Project* last = emp->projHead;
        while (last->pnext) last = last->pnext;
        last->pnext = newProj;
    }

    cout << "Project assigned successfully!\n";
    updateProjectsCSV(head, "projects.csv");
}

void Tracking::updateProjectsCSV(Employ* head, const string& filename) {
    ofstream file(filename, ios::trunc);
    file << "EmpID,EmpName,Projects,AvgScore,ProjectCount,PerformanceScore,FinalScore\n";

    Employ* curr = head;
    while (curr) {
        file << curr->empId << "," << curr->name << ",";

        Project* p = curr->projHead;
        string projectList;
        int total = 0, count = 0;

        while (p) {
            projectList += p->pname + ":" + to_string(p->score);
            total += p->score;
            count++;
            if (p->pnext) projectList += ",";
            p = p->pnext;
        }

        file << "\"" << projectList << "\",";
        double avg = (count == 0) ? 0.0 : static_cast<double>(total) / count;
        double perf = avg * count;
        int finalScore = static_cast<int>(perf + 0.5);

        file << avg << "," << count << "," << perf << "," << finalScore << "\n";
        curr = curr->next;
    }

    file.close();
}

double Tracking::calculateAverageScore(Employ* emp) {
    int total = 0, count = 0;
    Project* p = emp->projHead;
    while (p) {
        total += p->score;
        count++;
        p = p->pnext;
    }
    return (count == 0) ? 0.0 : (double)total / count;
}

void Tracking::viewAllProjects(Employ* head) {
    Employ* curr = head;
    while (curr) {
        cout << curr->empId << " (" << curr->name << "): ";
        if (!curr->projHead) cout << "No project assigned\n";
        else {
            Project* p = curr->projHead;
            while (p) {
                cout << p->pname << " [Score: " << p->score << "]";
                if (p->pnext) cout << ", ";
                p = p->pnext;
            }
            double avg = calculateAverageScore(curr);
            int count = projectCount(curr);
            double perf = avg * count;
            int finalScore = static_cast<int>(perf + 0.5);

            cout << " | Avg Score: " << avg
                 << " | Projects: " << count
                 << " | Performance: " << perf
                 << " | Final Score: " << finalScore;
        }
        cout << "\n";
        curr = curr->next;
    }
}

void Tracking::searchEmployee(Employ* head) {
    string id;
    cout << "Enter Employee ID to search: ";
    cin >> id;

    Employ* emp = findEmployeeById(head, id);
    if (!emp) {
        cout << "Employee not found.\n";
        return;
    }

    cout << "Employee ID: " << emp->empId << "\n";
    cout << "Name      : " << emp->name << "\n";
    cout << "Projects  : ";
    if (!emp->projHead) cout << "No projects assigned.\n";
    else {
        Project* p = emp->projHead;
        while (p) {
            cout << p->pname;
            if (p->pnext) cout << ", ";
            p = p->pnext;
        }
        cout << "\n";
    }
}

void Tracking::deleteProjectFromEmployee(Employ* head) {
    string empId, projName;
    cout << "Enter Employee ID: ";
    cin >> empId;
    cin.ignore();
    cout << "Enter Project Name to Delete: ";
    getline(cin, projName);

    Employ* emp = findEmployeeById(head, empId);
    if (!emp) {
        cout << "Employee not found.\n";
        return;
    }

    Project* curr = emp->projHead;
    Project* prev = nullptr;

    while (curr) {
        if (curr->pname == projName) {
            if (!prev) emp->projHead = curr->pnext;
            else prev->pnext = curr->pnext;
            delete curr;
            cout << "Project deleted successfully.\n";
            updateProjectsCSV(head, "projects.csv");
            return;
        }
        prev = curr;
        curr = curr->pnext;
    }
    cout << "Project not found.\n";
}

Employ* Tracking::findEmployeeById(Employ* head, const string& empId) {
    while (head) {
        if (head->empId == empId) return head;
        head = head->next;
    }
    return nullptr;
}

void Tracking::freeMemory(Employ* head) {
    while (head) {
        Project* p = head->projHead;
        while (p) {
            Project* del = p;
            p = p->pnext;
            delete del;
        }
        Employ* delEmp = head;
        head = head->next;
        delete delEmp;
    }
}

void Tracking::showmenu() {
    Employ* head = loadEmployees("employees.csv");
    loadProjects("projects.csv", head);

    int choice;
    do {
        cout << "\n--- Project Tracking Menu ---\n";
        cout << "1. Assign Project to Employee\n";
        cout << "2. View All Projects\n";
        cout << "3. Search Employee and View Projects\n";
        cout << "4. Delete Project from Employee\n";
        cout << "0. Back\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1: assignProjectToEmployee(head); break;
            case 2: viewAllProjects(head); break;
            case 3: searchEmployee(head); break;
            case 4: deleteProjectFromEmployee(head); break;
            case 0: updateProjectsCSV(head, "projects.csv"); 
                    cout << "Returning to Main Menu...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    freeMemory(head);
}

