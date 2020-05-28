#ifndef LIBRARY_PLAYERS_H
#define LIBRARY_PLAYERS_H

#include <iostream>
#include <deque>
#include <algorithm>
#include <random>

using namespace std;

#define card pair<int,char>
#define hand pair<card,card>

//humanPlay: アクションミスった時の戻れる挙動を作りたい

class Player{
public:
    int stack;
    int sunkCost;
    bool active;
    bool isHuman;
    int position;
    hand myHand;

    Player(int, int, int, bool);
    void displayPos();
    void humanPlay();
    void computerPlay();
private:
};

extern deque<Player> playerList;
extern deque<Player> copyList;

extern void makePlayers();
extern void postflopPlayers();

#endif //LIBRARY_PLAYERS_H
