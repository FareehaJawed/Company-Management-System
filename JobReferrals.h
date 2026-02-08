#ifndef JOBREFERRALS_H
#define JOBREFERRALS_H

#include <string>

struct Referral {
    std::string EmpName;
    std::string EmpID;
    std::string CanName;
    std::string expertise;
    std::string referralDate;
    Referral* next;
};

class JobReferral {
    Referral* front;
    Referral* rear;

public:
    JobReferral();              // Constructor to init queue
    void loadFromFile();        // Load referrals from file
    void writeToFile();         // Save referrals to file
    void EnQueue();             // Add new referral
    bool DeQueue();             // Remove oldest referral
    void read();                // Process front referral
    void print();               // Display all referrals
    void showmenu();            // Show interactive menu
    void clearQueue();
};

#endif // JOBREFERRALS_H

