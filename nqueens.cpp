#include <iostream>
#include <cstdlib>

#define ARRAY_SIZE 10020
#define CONST 3

enum Increment {
    Plus = 1,
    Minus = -1
};

bool printable = true;

int nQueens[ARRAY_SIZE], queensPerRow[ARRAY_SIZE], queensPerDiag1[2 * ARRAY_SIZE - 1], queensPerDiag2[2 * ARRAY_SIZE - 1];

int getColWithQeueenWithMaxConf(){

}

int getRowWithMinConflict(const int& col){

}

void updateConflicts(const int& n,const int& col, const int& row, const Increment& incr) {
    queensPerRow[row] += incr;
    queensPerDiag1[n - 1 + col - row] += incr;
    queensPerDiag2[col + row] += incr;
}

void initRandom(const int& n){
    for (int i = 0; i < n; i++) {
        nQueens[i] = rand() % n;
        updateConflicts(n, i, nQueens[i], Plus);
    }
}

bool hasConflicts(const int& n){
    for (int i = 0; i < n; i++) {
        if (queensPerRow[i] > 1)return true;
    }

    for (int i = 0; i < n * 2 - 1; i++) {
        if (queensPerDiag1[i] > 1 || queensPerDiag2[i] > 1)return true;
    }

    return false;
}

void solve(const int& n){
    initRandom(n);
    int col, row;

    for(int iter = 0; iter <= CONST*n; iter++){
        col = getColWithQeueenWithMaxConf();
        row = getRowWithMinConflict(col);
        updateConflicts(n, col, nQueens[col], Minus);
        nQueens[col]=row;
        updateConflicts(n, col, row, Plus);
    }

    if(hasConflicts(n)){
        solve(n);
    }
}

void printBoard(const int& n){
    for(int row = 0; row < n; row++){
        for(int col = 0; col < n; col++){
            std::cout << (nQueens[col] == row) ? '*' : '_';
        }
        std::cout << std::endl;
    }
}

void printSimple(const int& n) {
    std::cout << "[";
    for (int i = 0; i < n - 1; i++)std::cout << nQueens[i] << ", ";
    std::cout << nQueens[n - 1] << "]" << std::endl;
}

int main(){
    int n;
    std::cin >> n;
    solve(n);
    if(printable)printBoard(n);
    printSimple(n);
}