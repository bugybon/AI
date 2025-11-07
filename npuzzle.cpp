#include <iostream>

int const MAX_DIMENSION = 10;

class Node{
    private:
        int positionMissingPiece;
        //int distance = 0;
        Node* parent;
        int size;
        
    public:
        int** matrix;
        Node(const int& size = 0, const int& positionMissingPiece = 1, Node* parent = nullptr);

        int distance(int col, int row);
        int totalDistance();
        void print();
        bool isGoal();
};

Node::Node(const int& size, const int& positionMissingPiece, Node* parent){
    matrix = new int*[size];
    for(int i = 0; i < size; i++){
        matrix[i] = new int[size];
    }
    this->size = size;
    this->positionMissingPiece = (positionMissingPiece == -1) ? 9 : positionMissingPiece;
    this->parent = parent;
}

int Node::distance(int col, int row){
    int current = matrix[row][col];
    if(current == 0) return 0;
    
    if(current < positionMissingPiece) current -= 1;

    return std::abs(col-current%3) + std::abs(row-current/3);
}

int Node::totalDistance(){
    int sum = 0;
    for(int row = 0; row < this->size; row++){
        for(int col = 0; col < this->size; col++){
            sum += this->distance(col, row);
        }
    }
    return sum;
}

void Node::print(){
    for(int row = 0; row < this->size; row++){
        for(int col = 0; col < this->size; col++){
            std::cout << matrix[row][col];
        }
        std::cout << std::endl;
    }
}

bool Node::isGoal(){
    return this->totalDistance() == 0;
}

int main(){
    Node test(3, -1);
    for(int i = 0; i < 9; i++)
        test.matrix[i/3][i%3] = (i+1)%9;

    test.print();
    std::cout << test.totalDistance() << std::endl;
    return 0;
}