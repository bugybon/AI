#include <iostream>

#define ARRAY_SIZE 10020
#define CONST 3

int nQueens[ARRAY_SIZE], queensPerRow[ARRAY_SIZE], queensPerDiag1[ARRAY_SIZE], queensPerDiag2[ARRAY_SIZE];

int getColWithQeueenWithMaxConf(){

}

int getRowWithMinConflict(const int& col){

}

void initRandom(const int& n){

}

bool hasConflicts(){

}

void solve(const int& n){
    initRandom(n);
    int col, row;

    for(int iter = 0; iter <= CONST*n; iter++){
        col = getColWithQeueenWithMaxConf();
        row = getRowWithMinConflict(col);
        nQueens[col]=row;
    }

    if(hasConflicts()){
        solve(n);
    }
}

void print(const int& n){
    
}

int main(){
    int n;
    std::cin >> n;
    solve(n);
    print(n);
}