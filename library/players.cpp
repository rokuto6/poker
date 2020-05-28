#include "players.h"
#include "table.h"

#define card pair<int,char>
#define hand pair<card,card>

Table table;

Player::Player(int sta, int blind, int pos, bool human){
    stack = sta - blind;
    sunkCost = blind;
    active = true;
    isHuman = human;
    position = pos;
}

void Player::displayPos(){
    if(position == numPlayer - 1){
        cout << "BB ";
    }else if(position == numPlayer - 2){
        cout << "SB ";
    }else if(position == numPlayer - 3){
        cout << "BU ";
    }else if(position == numPlayer - 4){
        cout << "CO ";
    }else if(position == numPlayer - 5){
        cout << "HJ ";
    }else if(position == numPlayer - 6){
        cout << "LJ ";
    }else if(position == numPlayer - 7){
        cout << "7UTG ";
    }else if(position == numPlayer - 8){
        cout << "8UTG ";
    }else if(position == numPlayer - 9){
        cout << "9UTG ";
    }else if(position == numPlayer - 10){
        cout << "10UTG ";
    }
}

void Player::humanPlay(){
    // 人間のプレイ方法。キーボードから入力受け取り。
    int playerBet;
    char action;
    while(true) {
        //このwhileは指定文字以外が入力されたときに再入力させるため。
        //なので全アクション完了時はreturnになってる。
        cout << "アクションを決めてください" << endl;
        if(table.valBet == sunkCost) {
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
                active = false;
                return;
            }
            else if (action == 'c') {
                cout << "you call" << endl;
                sunkCost = table.valBet;
                stack -= table.valBet - sunkCost;
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
                else if(playerBet < table.minRaise){
                    if(table.minRaise < stack+sunkCost){
                        cout << "オールインしてね！" << endl;
                    }
                    else cout << "レイズは" << table.minRaise << "からだよ！" << endl;
                    continue;
                }else if(playerBet > stack){
                    cout << "持ってるチップより多いじゃん！" << endl;
                    continue;
                }
                break;
            }
            stack -= playerBet - sunkCost;
            sunkCost = playerBet;
            table.minRaise = 2 * sunkCost - table.valBet;
            table.valBet = playerBet;
            cout << "you raise to " << sunkCost << endl;
            return;
        }
        else{
            cout << "指定された文字で打ってね！" << endl;
        }
    }
}

void Player::computerPlay(){
    // comのプレイ方法。将来はここをいじる。現在クソザコマシーン。23oでもプッシュします。
    random_device rnd;     // 非決定的な乱数生成器
    mt19937 mt(rnd());
    long percent;
    if(table.valBet == sunkCost){
        percent = mt() % 2;
        if (percent == 0) {
            displayPos();
            cout << "check" << endl;
        }else{
            int tmpbet = max(5, table.valBet * 5 / 2); // valBet == 0 の時に額が0になったので暫定処置
            stack -= tmpbet - sunkCost;
            sunkCost = tmpbet;
            table.minRaise = 2 * sunkCost - table.valBet;
            table.valBet = tmpbet;
            displayPos();
            cout << "raise to " << sunkCost << endl;
        }
    }
    else {
        if(sunkCost + stack > table.valBet) {
            percent = mt() % 3;
            if (percent == 0) {
                active = false;
                displayPos();
                cout << "fold" << endl;
            } else if (percent == 1) {
                stack -= table.valBet - sunkCost;
                sunkCost = table.valBet;
                displayPos();
                cout << "call to " << table.valBet << endl;
            } else {
                int tmpbet = min(sunkCost + stack, table.valBet * 5 / 2);
                stack -= tmpbet - sunkCost;
                sunkCost = tmpbet;
                table.minRaise = 2 * sunkCost - table.valBet;
                table.valBet = tmpbet;
                if(stack == 0){
                    active = false;
                    displayPos();
                    cout << "allin " << table.valBet << endl;
                }
                else{
                    displayPos();
                    cout << "raise to " << table.valBet << endl;
                }
            }
        }else{
            percent = mt() % 2;
            active = false;
            if (percent == 0) {
                displayPos();
                cout << "fold" << endl;
            }else{
                sunkCost += stack;
                stack = 0;
                displayPos();
                cout << "allin " << table.valBet << endl;
            }
        }
    }
}

void makePlayers(){
    for(int i = 0; i < numPlayer; i++){
        if(i==0){
            Player p(200, 0, i,true);
            playerList.push_back(p);
            copyList.push_back(p);
        }
        else if(i==numPlayer-1){
            Player p(200, 2, i, false);
            playerList.push_back(p);
            copyList.push_back(p);
        }else if(i==numPlayer-2){
            Player p(200, 1, i, false);
            playerList.push_back(p);
            copyList.push_back(p);
        }
        else {
            Player p(200, 0, i,false);
            playerList.push_back(p);
            copyList.push_back(p);
        }
    }
}

void postflopPlayers(){ //リストをアクション順に入れ替える.
    int prePos = -1;
    Player back = copyList.back();
    for(int i = 0; i < copyList.size(); i++){
        Player p = copyList.front();
        if(back.position == numPlayer - 2){
            copyList.pop_back();
            copyList.push_front(back);
            return;
        }
        else if(p.position == numPlayer - 1 || p.position == numPlayer - 2 || p.position < prePos) return;
        else{
            copyList.pop_front();
            copyList.push_back(p);
            prePos = p.position;
        }
    }
}