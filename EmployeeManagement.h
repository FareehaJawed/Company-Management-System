#ifndef EMPLOYEEMANAGEMENT_H
#define EMPLOYEEMANAGEMENT_H

#include <string>
#include <unordered_map>

struct Employee {
    std::string id;
    std::string name;
    std::string role;
    std::string managerId;
};

class Management {
public:
    void showmenu();
private:
    void saveToCSV(const std::unordered_map<std::string, Employee>& empMap);
    void loadFromCSV(std::unordered_map<std::string, Employee>& empMap);
    void insertKey(std::unordered_map<std::string, Employee>& empMap);
    void searchKey(const std::unordered_map<std::string, Employee>& empMap);
    void deleteKey(std::unordered_map<std::string, Employee>& empMap);
    void printKey(const std::unordered_map<std::string, Employee>& empMap);
};

#endif // EMPLOYEEMANAGEMENT_H

