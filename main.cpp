#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <map>
#include <cstdlib>
#include <ctime>

using namespace std;

/* メモ的な。
 * これはポーカーをプレイするゲームです。
 * 今後はコンピュータを強くしてって最終的に人工知能の学習とかに持っていきたい。
 *
 * 今の所：コンピュータは全てのアクションを等確率で選択します。とても雑魚です。
 */

int numPlayer;
using card = pair<int, char>;
using hand = pair<card, card>;
vector<card> cardList(52);
hand handList[10];
vector<card> board(5);

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
};

void initializecardList(){ //カード配列を生成
    int i;
    for(i = 0; i < 52; i++){
        if(i / 13 == 0){
            cardList[i] = make_pair(i + 1, 's');
        }else if(i / 13 == 1){
            cardList[i] = make_pair(i - 12, 'h');
        }else if(i / 13 == 2){
            cardList[i] = make_pair(i - 25, 'd');
        }else{
            cardList[i] = make_pair(i - 38, 'c');
        }
    }
}

void shuffleCardList(){ //カードをシャッフル
    random_device rnd;
    mt19937 mt(rnd());
    shuffle(cardList.begin(), cardList.end(), mt);
}

void dealHands(){ //カードをくばる
    int i;
    for(i = 0; i < numPlayer*2; i++){
        if(i < numPlayer){
            handList[i].first = cardList[i];
        }else{
            handList[i%numPlayer].second = cardList[i];
        }
    }
}

void displayCard(card card1){ //カードの表示。1はAじゃないとね。
    if(card1.first == 1) cout << 'A' << card1.second;
    else if(card1.first == 13) cout << 'K' << card1.second;
    else if(card1.first == 12) cout << 'Q' << card1.second;
    else if(card1.first == 11) cout << 'J' << card1.second;
    else if(card1.first == 10) cout << 'T' << card1.second;
    else cout << card1.first << card1.second;
}

void displayHand(hand hand1){ //ハンドの表示。カードはpairで作ってるので表示がめんどい。
    displayCard(hand1.first);
    displayCard(hand1.second);
    cout << endl;
}

void displayAllHand(bool *active, int *position){
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
    int percent;
    if(*valBet == *sunkCost){
        percent = rand() % 2;
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
            percent = rand() % 3;
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
            percent = rand() % 2;
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
    srand((unsigned int)time(NULL)); //　乱数初期化はmainでやらないとダメそう
    int i, j, valBet = 2;
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