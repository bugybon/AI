#include <iostream>
#include <cstdlib>
#include <ctime>
//#include <chrono>

#define ARRAY_SIZE 10020
#define CONST 3

enum Increment {
    Plus = 1,
    Minus = -1
};

bool printable = false;

int nQueens[ARRAY_SIZE], queensPerRow[ARRAY_SIZE], queensPerDiag1[2 * ARRAY_SIZE - 1], queensPerDiag2[2 * ARRAY_SIZE - 1], candidates[ARRAY_SIZE];

int sumConflicts(const int& n, const int& col, const int& row) {
    return queensPerRow[row] + queensPerDiag1[n - 1 + col - row] + queensPerDiag2[col + row];
}

int getColWithQeueenWithMaxConf(const int& n){
    candidates[0] = 0;
    int maxConflicts = sumConflicts(n, candidates[0], nQueens[0]), countCandidates = 1;
    for (int i = 1; i < n; i++) {
        if (maxConflicts < sumConflicts(n, i, nQueens[i])) {
            maxConflicts = sumConflicts(n, i, nQueens[i]);
            candidates[0] = i;
            countCandidates = 1;
        }
        else if (maxConflicts == sumConflicts(n, i, nQueens[i])) {
            candidates[countCandidates] = i;
            countCandidates++;
        }

    }
    return candidates[rand() % countCandidates];
}

int getRowWithMinConflict(const int& n,const int& col){
    candidates[0] = 0;
    int minConflicts = sumConflicts(n, col, candidates[0]), countCandidates = 1;
    for (int i = 1; i < n; i++) {
        if (minConflicts > sumConflicts(n, col, i)) {
            minConflicts = sumConflicts(n, col, i);
            candidates[0] = i;
            countCandidates = 1;
        }
        else if (minConflicts == sumConflicts(n,col, i)) {
            candidates[countCandidates] = i;
            countCandidates++;
        }

    }
    return candidates[rand() % countCandidates];
}

void updateConflicts(const int& n,const int& col, const int& row, const Increment& incr) {
    queensPerRow[row] += incr;
    queensPerDiag1[n - 1 + col - row] += incr;
    queensPerDiag2[col + row] += incr;
}

void neutralize(const int& n) {
    for (int i = 0; i < n; i++) {
        queensPerRow[i] = 0;
        queensPerDiag1[i] = 0;
        queensPerDiag2[i] = 0;
    }

    for (int i = n; i < 2 * n - 1; i++) {
        queensPerDiag1[i] = 0;
        queensPerDiag2[i] = 0;
    }
}

void initRandom(const int& n){
    neutralize(n);
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

    for(int iter = 0; iter <= CONST*n && hasConflicts(n); iter++){
        col = getColWithQeueenWithMaxConf(n);
        row = getRowWithMinConflict(n, col);
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
    srand(time(0));
    int n;
    std::cin >> n;
    //std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    solve(n);
    //std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    //std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " [ms]" << std::endl;
    if(printable)printBoard(n);
    printSimple(n);
}