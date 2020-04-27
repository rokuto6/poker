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
 *
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

void humanPlay(int *valBet, int *sunkCost, int *stack, bool *active, int posPlayer){
    // 人間のプレイ方法。キーボードから入力受け取り。
    int playerBet;
    char action;
    while(true) {
        //このwhileは指定文字以外が入力されたときに再入力させるため。
        //なので全アクション完了時はreturnになってる。
        cout << "アクションを決めてください" << endl;
        if(*valBet == sunkCost[posPlayer]) {
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
                active[posPlayer] = false;
                return;
            }
            else if (action == 'c') {
                cout << "you call" << endl;
                sunkCost[posPlayer] = *valBet;
                stack[posPlayer] -= *valBet - sunkCost[posPlayer];
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
                }else if(playerBet > stack[posPlayer]){
                    cout << "持ってるチップより多いじゃん！" << endl;
                    continue;
                }
                break;
            }
            stack[posPlayer] -= *valBet - sunkCost[posPlayer];
            *valBet = playerBet;
            sunkCost[posPlayer] = playerBet;
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

void computerBetting(int *valBet, int *sunkCost, int *stack, bool *active, int i, bool *around, bool *allPlayed){
    // bettingRoundで使う用。何回も書きたくないので小分け。
    computerPlay(valBet, sunkCost+i, stack+i, active+i, i);
    for (int j = 0; j < numPlayer; j++) {
        if(active[j] && *valBet != sunkCost[j]) break;
        if(j == numPlayer - 1) *around = true;
    }
    for (int j = i+1; j < numPlayer; j++) {
        if(active[j]) break;
        if(j == numPlayer - 1) *allPlayed = true;
    }
}

void bettingRound(int *valBet, int *sunkCost, int *stack, bool *active, int posPlayer, int *position){
    int i;
    bool allPlayed = false; //判定方法的に全員一回プレイしたかを見る必要があるため。
    displayAllHand(active, position);
    while(true) {
        bool around = false, whileJump = false;
        for (i = 0; i < posPlayer; i++) {
            if(active[i]) {
                computerBetting(valBet, sunkCost, stack, active, i, &around, &allPlayed);
                if(around && allPlayed){
                    whileJump = true;
                    break;
                }
                around = false; // aroundがtrueのままだと一周したら自動で終わっちゃうので
            }
        }
        if(whileJump) break;
        if (active[posPlayer]) {
            humanPlay(valBet, sunkCost, stack, active, posPlayer);
            for (i = 0; i < numPlayer; i++) {
                if(active[i]) {
                    if (*valBet != sunkCost[i]) break;
                }
                if(i == numPlayer - 1) around = true;
            }
            for (int j = posPlayer+1; j < numPlayer; j++) {
                if(active[j]) break;
                if(j == numPlayer - 1) allPlayed = true;
            }
            if(around && allPlayed) break;
            around = false;
        }
        for (i = posPlayer+1; i < numPlayer; i++) {
            if(active[i]) {
                computerBetting(valBet, sunkCost, stack, active, i, &around, &allPlayed);
                if(i == numPlayer-1) allPlayed = true;
                if(around && allPlayed){
                    whileJump = true;
                    break;
                }
                around = false;
            }
        }
        if(whileJump) break;
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

void changeRound(int *valBet, int *sunkCost, char occ, int *stack, bool *active, int posPlayer, int *position){
    // ラウンド変更の際に呼び出し。3回あるので流石に何回も書くのはアホらしい。
    *valBet = 0;
    for(int i = 0; i < numPlayer; i++){
        *(sunkCost+i) = 0;
    }
    displayBoard(occ);
    bettingRound(valBet, sunkCost, stack, active, posPlayer, position);
}

int main() {
    srand((unsigned int)time(NULL)); //　乱数初期化はmainでやらないとダメそうdamesou
    int i, j, valBet = 2, posPlayer = 0;
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
        shuffleCardList();
        dealHands();
        bettingRound(&valBet, sunkCost, stack, active, posPlayer, position);
        for(i = 0; i < 3; i++){
            board[i] = cardList[numPlayer*2+i];
        }
        board[3] = cardList[numPlayer*2+5];
        board[4] = cardList[numPlayer*2+7];
        posPlayer += 2;
        for(i = 0; i < numPlayer; i++){
            cout << position[i] << endl;
        }
        if(posPlayer >= numPlayer) posPlayer -= numPlayer;
        for(i = 0; i < numPlayer; i++){
            int index = (i - 2 + numPlayer) % numPlayer;
            int temsta[numPlayer], tempos[numPlayer];
            bool temact[numPlayer]; // 一時的に置く配列
            temsta[i] = stack[index]; // ポスフロはSBからのアクションなのでずらしている
            temact[i] = active[index];
            tempos[i] = position[index];
            if(i == numPlayer-1){
                for(j = 0; j < numPlayer; j++){
                    stack[j] = temsta[j];
                    active[j] = temact[j];
                    position[j] = tempos[j];
                }
            }
        }
        for(i = 0; i < numPlayer; i++){
            cout << position[i] << endl;
        }
        changeRound(&valBet, sunkCost, 'f', stack, active, posPlayer, position);
        changeRound(&valBet, sunkCost, 't', stack, active, posPlayer, position);
        changeRound(&valBet, sunkCost, 'r', stack, active, posPlayer, position);
        cout << "showdown!" << endl;
    //}
}

