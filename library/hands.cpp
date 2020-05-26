#include "hands.h"
#include "playing.h"

#define card pair<int,char>
#define hand pair<card,card>

vector<card> cardList(52);
hand handList[10];
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