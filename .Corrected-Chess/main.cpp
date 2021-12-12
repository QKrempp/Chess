#include "Board.hpp"
#include "Player.hpp"
#include "Arbitre.hpp"

#include <iostream>
using namespace std;

int main(int argc, char* argv[]){
    Board b;
    Arbitre a;
    a.initHash(&b);
    a.updateMenacesFull(&b);
    if(argc > 1){
        cout << "-----------------------" << endl;
        for(int i = 1; i < argc; i++){
            a.requestMove(&b, b.getPlayer(), argv[i]);
        }
        cout << "-----------------------" << endl;
    }
//     Human ab1(WHITE);
    Human ab1(WHITE);
    Human ab2(BLACK);
    while(!a.isGameOver(&b)){
        ab1.play(&b, &a);
        ab2.play(&b, &a);
        cout << b << endl;
        a.printMenaces(&b);
        cout << endl;
    }
    return 0;
}
