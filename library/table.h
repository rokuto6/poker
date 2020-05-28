#ifndef LIBRARY_TABLE_H
#define LIBRARY_TABLE_H

#include <iostream>
#include <deque>
#include <algorithm>

using namespace std;
#define card pair<int,char>
#define hand pair<card,card>

// endGame: 勝利プレーヤーにpotを渡す処理(そもそも勝利処理が未実装)
// bettingRound: 全員fold時にpotを渡す処理

class Table{
public:
    int pot;
    int valBet;
    int minRaise;

    Table();
    void calcPot();
    void changeRound();
    void endGame();
    private:
};

extern int numPlayer;
extern Table table;
extern void decideNumPlayer();
extern void makePlayers();
extern bool bettingRound();
extern void performRound(bool*);

#endif //LIBRARY_TABLE_H