#include <iostream>

#include "EmployeeManagement.h"
#include "CompanyHierarchy.h"
#include "EmpProjTrack.h"
#include "PeerReviews.h"
#include "Analytics_Promotion.h"
#include "Announcement.h"
#include "JobReferrals.h"

using namespace std;

int main() {
    Management em;
    Hierarchy ch;
    Tracking ept;
    Reviews pr;
    Promo p;
    Announce ann;
	JobReferral jr;
	
    int choice;
    cout << "\n--- Company Management System ---\n\n";
    do {
    	cout << endl;
        cout << "1. Employee Management\n";
        cout << "2. Hierarchy\n";
        cout << "3. Project Tracking\n";
        cout << "4. Peer Reviews\n";
        cout << "5. Analytics and Promotions\n";
        cout << "6. Announcements\n";
        cout << "7. Job Referrals\n";
        cout << "8. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
		cin.ignore(); // Important
		
        switch (choice) {
            case 1: em.showmenu(); break;
            case 2: ch.showmenu(); break;
            case 3: ept.showmenu(); break;
            case 4: pr.showmenu(); break;
            case 5: p.showmenu(); break;
            case 6: ann.showmenu(); break;
            case 7: jr.showmenu(); break;
            case 8: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 8);

    return 0;
}

