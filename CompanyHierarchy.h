#ifndef COMPANYHIERARCHY_H
#define COMPANYHIERARCHY_H

#include <string>
#include <unordered_map>
#include <vector>

struct Emp {
    std::string id;
    std::string name;
    std::string role;
    std::string managerId;
};

struct TreeNode {
    Emp emp;
    std::vector<TreeNode*> subordinates;
};

class Hierarchy {
public:
    void showmenu();

private:
    std::unordered_map<std::string, Emp> readEmployeesFromCSV(const std::string& filename);
    TreeNode* buildHierarchyTree(const std::unordered_map<std::string, Emp>& empMap);
    void printHierarchy(TreeNode* root, int level = 0);
    void deleteTree(TreeNode* root);
};

#endif // COMPANYHIERARCHY_H

