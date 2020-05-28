#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include "library/players.h"
#include "library/table.h"
#include "library/hands.h"

using namespace std;
#define card pair<int,char>
#define hand pair<card,card>

/*
 * メモ的な。
 * これはポーカーをプレイするゲームです。
 * 今後はコンピュータを強くしてって最終的に人工知能の学習とかに持っていきたい。
 *
 * 今の所：コンピュータは全てのアクションを等確率で選択します。とても雑魚です。
 */

int main(){
    char round[4] = {'p','f','t','r'};
    decideNumPlayer();
    makePlayers();
    initializecardList();
    //for(i = 0; i < 1; i++) {
        bool end = false;
        shuffleCardList();
        dealHands();
        for(int i = 0; i < 4; i++){
            if(i != 0) displayBoard(round[i]);
            performRound(&end);
            if(end) break;
        }
        cout << "showdown!" << endl;
    //}
}