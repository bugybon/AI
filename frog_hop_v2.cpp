#include <iostream>
#include <cstring>

char* buffer;

bool dfs(char board[], const int& size, const int& free_position);
bool dfsNodeTreverse(char board[], const int& size, const int& free_position, const int& checked_position);

void initBoard(char* board, const int& size){
    for(int i = 0; i < size/2; i++){
        board[i]='>';
        board[size-i-1]='<';
    }
    board[size/2]='_';
}

bool isGoalState(const char board[], const int& size){
    for(int i = 0; i < size/2; i++){
        if(board[i]!='<' || board[size-i-1]!='>')
            return false;
    }
    return true;
}

char* reverseFrogs(const char board[], const int& size){
    for(int i = 0; i < size; i++){
        if(board[i] == '<'){
            buffer[i] = '>';
        }else if(board[i] == '>'){
            buffer[i] = '<';
        }else{
            buffer[i] = '_';
        }
    }
    return buffer;
}

bool dfs(char board[], const int& size, const int& free_position){
    if(isGoalState(board, size))
        return true;

    for(int i = -2; i <= 2; i++){
        if(i==0) continue;
        else if(i < 0 && free_position + i >=0){
            if(board[free_position + i]=='>'){
                dfsNodeTreverse(board, size, free_position, i);
            }
        }else if(i > 0 && free_position + i < size){
            if(board[free_position + i]=='<'){
                dfsNodeTreverse(board, size, free_position, i);
            }
        }
    }

    return false;
}

bool dfsNodeTreverse(char board[], const int& size, const int& free_position, const int& i){

                std::swap(board[free_position],board[free_position+i]);
                //std::cout << board << " " << free_position + i << std::endl;
                if(dfs(board, size, free_position + i)){
                    std::cout << reverseFrogs(board, size) <<  std::endl;
                    std::swap(board[free_position],board[free_position+i]);
                    return true;
                }
                std::swap(board[free_position],board[free_position+i]);
    //return false;
}

int main(){
    int n;
    std::cin >> n;  
    
    int size = n*2 + 1;
    char *board = new char[size];
    buffer = new char[size];
    initBoard(board, size);
    dfs(board, size, n);
    std::cout << reverseFrogs(board, size) << std::endl;
    return 0;
}