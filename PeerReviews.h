#ifndef PEERREVIEWS_H
#define PEERREVIEWS_H

#include <string>

struct Adj; // Forward declaration

struct Vertex {
    std::string id;
    std::string name;
    Vertex* next;
    Adj* startAdj;
};

struct Adj {
    Vertex* adjvert;
    Adj* anext;
    int weight;
};

class Reviews {
public:
    void showmenu();

private:
    void loadEmployees();
    void loadLinks();
    void saveLinks();
    void writeReviewsToCSV();
    Vertex* findVertex(std::string data);
    void giveReviews();
    void displayReviews();
    void clearData();
};

#endif // PEERREVIEWS_H

