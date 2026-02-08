#ifndef ANNOUNCEMENT_H
#define ANNOUNCEMENT_H

#include <string>

struct Stack {
    std::string data;
    Stack* next;
};

class Announce {
public:
    void showmenu();

private:
	Stack* top = nullptr;
    void loadFromFile();
    void saveToFile();
    void clear();
    void print();
    void push();
};

#endif // ANNOUNCEMENT_H

