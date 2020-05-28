#include "table.h"
#include "players.h"
#include "hands.h"

#define card pair<int,char>
#define hand pair<card,card>

int numPlayer;
deque<Player> playerList;
deque<Player> copyList;

Table::Table(){
    pot = 0;
    valBet = 2;
    minRaise = 4;
}

void Table::calcPot(){
    for(int i = 0; i < copyList.size(); i++){
        Player p = copyList.front();
        copyList.pop_front();
        pot += p.sunkCost;
        p.sunkCost = 0;
        copyList.push_back(p);
    }
}

void Table::changeRound(){
    calcPot();
    valBet = 0;
    minRaise = 0;
}

void Table::endGame(){ // 勝利プレーヤーにpotを渡す処理(そもそも勝利処理が未実装)
    calcPot();
    valBet = 2;
    minRaise = 4;
}

void decideNumPlayer(){ //　プレーヤー数を決める(10maxにした)
    while(true) {
        cout << "comは何人にしますか？(1~9人まで、半角英数字)" << endl;
        cin >> numPlayer;
        if (cin.fail()) {
            cout << "半角英数字を入れてね！" << endl;
            cin.clear();
            cin.ignore(1024, '\n');
            continue;
        }
        else if(numPlayer < 1){
            cout << "1人でポーカーする気かい？comは1人以上にしてね！" << endl;
            continue;
        }else if(numPlayer > 9){
            cout << "ちょっと多すぎるかなぁ…comは9人以下にしてね！" << endl;
            continue;
        }
        break;
    }
    numPlayer++;
}

bool bettingRound(){
    int numAround = 0, endPos = copyList.back().position, preBet = table.valBet;
    displayAllHand(); // 全ハンド表示しています
    bool allPlayed = false;
    while(numAround != copyList.size() || !allPlayed) {
        Player p = copyList.front();
        copyList.pop_front();
        if (p.isHuman) p.humanPlay();
        else p.computerPlay();
        cout << p.sunkCost << endl;
        if(p.active){
            // ポジション操作
            if(endPos == p.position) allPlayed = true;
            // 全員がコールしたか
            if(p.sunkCost == preBet) numAround++;
            else{
                numAround = 1;
                preBet = p.sunkCost;
            }
            copyList.push_back(p);
        }
        // 全員がフォールド
        if(copyList.size() == 1) return true; // potを渡す処理
    }
    return false;
}

void performRound(bool *end){
    // ラウンド変更の際に呼び出し。
    *end = bettingRound();
    postflopPlayers();
    table.changeRound();
}