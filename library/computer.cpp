#include "computer.h"

#define card pair<int,char>
#define hand pair<card,card>

Computer::Computer() {
    stack = 200;
    sunkCost = 0;
    active = true;
}
Computer::Computer(string pos, hand han){
    stack = 200;
    sunkCost = 0;
    active = true;
    position = pos;
    comHand = han;
}