#include <iostream>

int const MAX_DIMENSION = 10;

class Node{
    private:
        int positionMissingPiece;
        //int distance = 0;
        Node* parent;
        
    public:
        int** matrix;
        Node(const int& size = 0, const int& positionMissingPiece = 1, Node* parent = nullptr);

        int distance();
        void print();
};

Node::Node(const int& size, const int& positionMissingPiece, Node* parent){
    matrix = new int*[size];
    for(int i = 0; i < size; i++){
        matrix[i] = new int[size];
    }
    this->positionMissingPiece = (positionMissingPiece == -1) ? 9 : positionMissingPiece;
    this->parent = parent;
}

int Node::distance(){
    int sum = 0, current;
    for(int row = 0; row < 3; row++){
        for(int col = 0; col < 3; col++){
            current = matrix[row][col];
            if(current == 0) continue;
            
            if(current < positionMissingPiece) current -= 1;

            sum += std::abs(col-current%3) + std::abs(row-current/3);
        }
    }
    return sum;
}

void Node::print(){
    for(int row = 0; row < 3; row++){
        for(int col = 0; col < 3; col++){
            std::cout << matrix[row][col];
        }
        std::cout << std::endl;
    }
}

int main(){
    Node test(3, 2);
    for(int i = 0; i < 9; i++)
        test.matrix[i/3][i%3] = (i+1)%9;

    test.print();
    std::cout << test.distance() << std::endl;
    return 0;
}