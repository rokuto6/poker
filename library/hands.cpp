#include "hands.h"
#include "players.h"
#include "table.h"

#define card pair<int,char>
#define hand pair<card,card>

vector<card> cardList(52);
vector<card> board(5);

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
    for(int i = 0; i < numPlayer; i++){
        Player p = playerList.front();
        playerList.pop();
        p.myHand = make_pair(cardList[i], cardList[i+numPlayer]);
        playerList.push(p);
    }
    for(int i = 0; i < 3; i++){
        board[i] = cardList[2*numPlayer+1+i];
    }
    board[3] = cardList[2*numPlayer+5];
    board[4] = cardList[2*numPlayer+7];
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

void displayAllHand(){
    // 卓の人のカードを全部表示。将来なくなるorショーダウンで使うかな？
    for(int i = 0; i < numPlayer; i++){
        Player p = playerList.front();
        playerList.pop();
        playerList.push(p);
        if(p.active) {
            if(p.isHuman){
                cout << "your hand is ";
                displayHand(p.myHand);
            }
            else{
                p.displayPos();
                cout << "hand is ";
                displayHand(p.myHand);
            }
        }
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