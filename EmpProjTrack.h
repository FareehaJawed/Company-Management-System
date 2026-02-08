#ifndef EMPPROJTRACK_H
#define EMPPROJTRACK_H

#include <string>

struct Project {
    std::string pname;
    int score;
    Project* pnext;
};

struct Employ {
    std::string empId;
    std::string name;
    Employ* next;
    Project* projHead;
};

struct ProjectDef {
    std::string pname;
    int score;
};

class Tracking {
public:
    void showmenu();

private:
    Employ* loadEmployees(const std::string& filename);
    void loadProjects(const std::string& filename, Employ* head);
    void assignProjectToEmployee(Employ* head);
    void viewAllProjects(Employ* head);
    void searchEmployee(Employ* head);
    void deleteProjectFromEmployee(Employ* head);
    void updateProjectsCSV(Employ* head, const std::string& filename);
    Employ* findEmployeeById(Employ* head, const std::string& empId);
    int getScoreFromName(const std::string& pname);
    int projectCount(Employ* emp);
    double calculateAverageScore(Employ* emp);
    void freeMemory(Employ* head);
};

#endif // EMPPROJTRACK_H

