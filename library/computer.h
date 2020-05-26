#ifndef LIBRARY_COMPUTER_H
#define LIBRARY_COMPUTER_H

#include <iostream>

using namespace std;

#define card pair<int,char>
#define hand pair<card,card>

class Computer{
public:
    int stack;
    int sunkCost;
    bool active;
    string position;
    hand comHand;

    Computer();
    Computer(string, hand);
private:
};

#endif //LIBRARY_COMPUTER_H
