#include "playing.h"

using namespace std;
#define card pair<int,char>
#define hand pair<card,card>

int numPlayer;

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
}