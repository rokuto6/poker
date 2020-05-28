#ifndef LIBRARY_HANDS_H
#define LIBRARY_HANDS_H

#include <iostream>
#include <algorithm>
#include <random>
#include <vector>

using namespace std;

#define card pair<int,char>
#define hand pair<card,card>

extern vector<card> cardList;
extern vector<card> board;

extern void initializecardList();
extern void shuffleCardList();
extern void dealHands();
extern void displayCard(card);
extern void displayHand(hand);
extern void displayAllHand();
extern void displayBoard(char);

#endif //LIBRARY_HANDS_H