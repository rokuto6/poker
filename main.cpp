#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include "library/computer.h"
#include "library/playing.h"
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

void displayAllHand(const bool *active, int *position){
    // 卓の人のカードを全部表示。将来なくなるorショーダウンで使うかな？
    int i;
    for(i = 0; i < numPlayer; i++){
        if(position[i] == 0 && active[i]) {
            cout << "your hand is ";
            displayHand(handList[i]);
        }
        else if(active[i]) {
            cout << "com" << position[i] << " hand is ";
            displayHand(handList[i]);
        }
    }
}

void humanPlay(int *valBet, int *sunkCost, int *stack, bool *active){
    // 人間のプレイ方法。キーボードから入力受け取り。
    int playerBet;
    char action;
    while(true) {
        //このwhileは指定文字以外が入力されたときに再入力させるため。
        //なので全アクション完了時はreturnになってる。
        cout << "アクションを決めてください" << endl;
        if(*valBet == *sunkCost) {
            cout << "x : チェック,  b,r : ベット" << endl;
            cin >> action;
            if (action == 'x') {
                cout << "you check" << endl;
                return;
            }
        }
        else{
            cout << "f : フォールド, c : コール, b,r : レイズ" << endl;
            cin >> action;
            if (action == 'f') {
                cout << "you fold" << endl;
                *active = false;
                return;
            }
            else if (action == 'c') {
                cout << "you call" << endl;
                *sunkCost = *valBet;
                *stack -= *valBet - *sunkCost;
                return;
            }
        }
        if (action == 'b' || action == 'r') {
            while(true) {
                cout << "額を記入してください(半角英数字)" << endl;
                cin >> playerBet;
                if (cin.fail()) {
                    cout << "半角英数字を入れてね！" << endl;
                    cin.clear();
                    cin.ignore(1024, '\n');
                    continue;
                }
                else if(playerBet < *valBet){ //　最小ベット額まだやってない
                    cout << "少なすぎるよ！" << endl;
                    continue;
                }else if(playerBet > *stack){
                    cout << "持ってるチップより多いじゃん！" << endl;
                    continue;
                }
                break;
            }
            *stack -= *valBet - *sunkCost;
            *valBet = playerBet;
            *sunkCost = playerBet;
            cout << "you raise to " << *valBet << endl;
            return;
        }
        else{
            cout << "指定された文字で打ってね！" << endl;
        }
    }
}

void computerPlay(int *valBet, int *sunkCost, int *stack, bool *active, int comNum){
    // comのプレイ方法。将来はここをいじる。現在クソザコマシーン。23oでもプッシュします。
    std::random_device rnd;     // 非決定的な乱数生成器
    std::mt19937 mt(rnd());
    long percent;
    if(*valBet == *sunkCost){
        percent = mt() % 2;
        if (percent == 0) {
            cout << "com" << comNum << " check" << endl;
        }else{
            *valBet = max(5, *valBet * 5 / 2); // valBet == 0 の時に額が0になったので暫定処置
            *stack -= *valBet - *sunkCost;
            *sunkCost = *valBet;
            cout << "com" << comNum << " raise to " << *valBet << endl;
        }
    }
    else {
        if(*sunkCost + *stack > *valBet) {
            percent = mt() % 3;
            if (percent == 0) {
                *active = false;
                cout << "com" << comNum << " fold" << endl;
            } else if (percent == 1) {
                *stack -= *valBet - *sunkCost;
                *sunkCost = *valBet;
                cout << "com" << comNum << " call to " << *valBet << endl;
            } else {
                *valBet = min(*sunkCost + *stack, *valBet * 5 / 2);
                *stack -= *valBet - *sunkCost;
                *sunkCost = *valBet;
                if(*valBet != *sunkCost + *stack){
                    cout << "com" << comNum << " raise to " << *valBet << endl;
                }else{
                    cout << "com" << comNum << " allin " << *valBet << endl;
                }
            }
        }else{
            percent = mt() % 2;
            if (percent == 0) {
                *active = false;
                cout << "com" << comNum << " fold" << endl;
            }else{
                *sunkCost += *stack;
                *stack = 0;
                cout << "com" << comNum << " allin " << *valBet << endl;
            }
        }
    }
}


void bettingRound(int *valBet, int *sunkCost, int *stack, bool *active, int *position, bool *end){
    int i, j;
    int numActive = 0;
    displayAllHand(active, position);
    bool allPlayed = false, around = false, whileJump = false;
    while(true) {
        for (i = 0; i < numPlayer; i++) {
            if (position[i] == 0 && active[i]) {
                humanPlay(valBet, sunkCost+i, stack+i, active+i);
            }else if(active[i]){
                computerPlay(valBet, sunkCost+i, stack+i, active+i, position[i]);
            }
            for(j = 0; j < numPlayer; j++){ // 全員fの時の対応
                if(active[j]) numActive++;
            }
            if(numActive == 1){
                cout << "fold end!" << endl;
                *end = true;
                whileJump = true;
                break;
            }
            for (j = 0; j < numPlayer; j++) {
                if(active[j] && *valBet != sunkCost[j]) break;
                if(j == numPlayer - 1) around = true;
            }
            if(i == numPlayer - 1) allPlayed = true;
            for (j = i+1; j < numPlayer; j++) {
                if(active[j]) break;
                if(j == numPlayer - 1) allPlayed = true;
            }
            if(around && allPlayed){
                whileJump = true;
                break;
            }
            around = false; // aroundがtrueのままだと一周したら自動で終わっちゃうので
            numActive = 0;
        }
        if(whileJump) break;
    }
}

void slide(int *stack, bool *active, int *position){
    int i;
    int temsta[numPlayer], tempos[numPlayer];
    bool temact[numPlayer]; // 一時的に置く配列
    hand temhand[numPlayer];
    for(i = 0; i < numPlayer; i++) {
        int index = (i - 2 + numPlayer) % numPlayer;
        temsta[i] = stack[index]; // ポスフロはSBからのアクションなのでずらしている
        temact[i] = active[index];
        tempos[i] = position[index];
        temhand[i] = handList[index];
    }
    for(i = 0; i < numPlayer; i++){
        stack[i] = temsta[i];
        active[i] = temact[i];
        position[i] = tempos[i];
        handList[i] = temhand[i];
    }
}

void displayBoard(char occ){ // ボード表示
    int i;
    if(occ == 'f') {
        cout << endl;
        cout << "flop" << endl;
        cout << "board: ";
        for(i = 0; i < 3; i++) {
            displayCard(board[i]);
            cout << " ";
        }
        cout << endl;
    }
    if(occ == 't') {
        cout << endl;
        cout << "turn" << endl;
        cout << "turn card is ";
        displayCard(board[3]);
        cout << endl;
        cout << "board: ";
        for(i = 0; i < 4; i++) {
            displayCard(board[i]);
            cout << " ";
        }
        cout << endl;
    }
    if(occ == 'r') {
        cout << endl;
        cout << "river" << endl;
        cout << "river card is ";
        displayCard(board[4]);
        cout << endl;
        cout << "board: ";
        for(i = 0; i < 5; i++) {
            displayCard(board[i]);
            cout << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void performRound(int *valBet, int *sunkCost, char occ, int *stack, bool *active, int *position, bool *end){
    // ラウンド変更の際に呼び出し。3回あるので流石に何回も書くのはアホらしい。
    int i;
    *valBet = 0;
    for(i = 0; i < numPlayer; i++){
        *(sunkCost+i) = 0;
    }
    displayBoard(occ);
    bettingRound(valBet, sunkCost, stack, active, position, end);
    if(occ == 'r' && !*end){
        cout << "showdown!" << endl;
        *end = true;
    }
}

int main() {
    int i, valBet = 2;
    decideNumPlayer();
    numPlayer++;
    int stack[numPlayer], sunkCost[numPlayer];
    int position[numPlayer];
    bool active[numPlayer];
    for(i = 0; i < numPlayer; i++){
        stack[i] = 200;
        if(i == numPlayer - 2) sunkCost[i] = 1;
        else if(i == numPlayer - 1) sunkCost[i] = 2;
        else sunkCost[i] = 0;
        active[i] = true;
        position[i] = i;
    }
    initializecardList();
    //for(i = 0; i < 1; i++) {
        bool end = false;
        shuffleCardList();
        dealHands();
        bettingRound(&valBet, sunkCost, stack, active, position, &end);
        for(i = 0; i < 3; i++){
            board[i] = cardList[numPlayer*2+i];
        }
        board[3] = cardList[numPlayer*2+5];
        board[4] = cardList[numPlayer*2+7];
        slide(stack, active, position);
        if(!end) {
            performRound(&valBet, sunkCost, 'f', stack, active, position, &end);
        }
        if(!end) {
            performRound(&valBet, sunkCost, 't', stack, active, position, &end);
        }
        if(!end) {
            performRound(&valBet, sunkCost, 'r', stack, active, position, &end);
        }
    //}
}