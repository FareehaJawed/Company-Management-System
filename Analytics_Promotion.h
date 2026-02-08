#ifndef ANALYTICS_PROMOTION_H
#define ANALYTICS_PROMOTION_H

#include <vector>
#include <string>

struct Employe {
    std::string id, name, role, managerId;
    int finalScore = 0;
    int avgReview = 0;
    int totalScore = 0;

    Employe(std::string i, std::string n, std::string r, std::string m)
        : id(i), name(n), role(r), managerId(m), finalScore(0), avgReview(0), totalScore(0) {}
};

class Promo {
public:
    void showmenu();

private:
    std::vector<Employe> loadEmployees();
    void loadProjectScores(std::vector<Employe>& employees);
    void loadReviewScores(std::vector<Employe>& employees);
    void calculateTotalScores(std::vector<Employe>& employees);
    void saveLeaderboardToCSV(const std::vector<Employe>& employees);
    void showLeaderboard(std::vector<Employe> employees);
    void promoteEmployees(std::vector<Employe>& employees);
};

#endif // ANALYTICS_PROMOTION_H

