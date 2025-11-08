#include <iostream>
#include <cmath>

int const MAX_DIMENSION = 10;

class Node{
    private:
        int positionMissingPiece;
        //int distance = 0;
        Node* parent;
        std::string direction;
        int size;
        int** matrix;
        int currentMissingPiece = 0;
        
    public:
        Node(const int& size = 1, const int& positionMissingPiece = 1, Node* parent = nullptr);
        Node(Node* other);

        Node* getParent(){return this->parent;};
        void setParent(Node* other){this->parent = other;};
        void inputMatrix();
        Node** neighbours();
        int distance(int col, int row);
        int totalDistance();
        void print();
        bool isGoal();
        bool isSolvable();
        bool isEqual(Node* other);
        void printPath();
};

Node::Node(const int& size, const int& positionMissingPiece, Node* parent){
    this->size = size;
    matrix = new int*[size];
    for(int i = 0; i < size; i++){
        matrix[i] = new int[size];
    }
    this->positionMissingPiece = (positionMissingPiece == -1) ? size*size : (positionMissingPiece + 1); // position counting starts from 0 which leads this small adjustments
    this->parent = parent;
    this->direction = "\0";
}

Node::Node(Node* other){
    this->size = other->size;
    matrix = new int*[other->size];

    for(int i = 0; i < this->size; i++){

        matrix[i] = new int[this->size];
        for(int col = 0; col < this->size; col++)
            matrix[i][col] = other->matrix[i][col];
    }

    this->positionMissingPiece = other->positionMissingPiece;
    this->parent = other->parent;
    this->direction = other->direction;
}

int Node::distance(int col, int row){
    int current = matrix[row][col];
    if(current == 0) return 0;
    
    if(current < positionMissingPiece) current -= 1;

    return std::abs(col-current%this->size) + std::abs(row-current/this->size);
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

void Node::inputMatrix(){
    for(int row = 0; row < this->size; row++){
        for(int col = 0; col < this->size; col++){
            std::cin >> this->matrix[row][col];
            if(this->matrix[row][col] == 0) currentMissingPiece = row*this->size + col;
        }
    }
}

Node** Node::neighbours(){
    Node** neighbours = new Node*[5]();
    int countNeighbours = 0;

    int positions[] = {-this->size, -1, 1, this->size};
    std::string directions[] = {"down", "right", "left", "up"};
    int move;
    int** currentMatrix;
    for(int i = 0; i < 4; i++){
        move = currentMissingPiece + positions[i];
        if(move < 0 || move >= this->size * this->size ||
            ((move%this->size) != (currentMissingPiece%this->size) && (move/this->size) != (currentMissingPiece/this->size))) continue;

        neighbours[countNeighbours] = new Node(this);
        neighbours[countNeighbours]->parent = this;
        neighbours[countNeighbours]->currentMissingPiece = move;
        neighbours[countNeighbours]->direction = directions[i];
        std::swap(neighbours[countNeighbours]->matrix[move/this->size][move%this->size], neighbours[countNeighbours]->matrix[currentMissingPiece/this->size][currentMissingPiece%this->size]);
        
        countNeighbours++;
    }
    return neighbours;
}

bool Node::isSolvable(){
    int inversions = 0;
    for(int i = 0; i < this->size*this->size; i++){
        if(this->matrix[i/this->size][i%this->size] == 0)continue;

        for(int next = i + 1; next < this->size*this->size; next++ ){
            if(this->matrix[next/this->size][next%this->size] == 0)continue;

            inversions += (this->matrix[i/this->size][i%this->size] > this->matrix[next/this->size][next%this->size]);
        }
    }
    return inversions % 2 == 0;
    /*return (this->size%2 == 1 && inversions % 2 == 0) || 
        (this->size%2 == 0 && this->positionMissingPiece%2==(inversions+this->positionMissingPiece)%2);
        The given algorithm checks if a puzzle is solvable only for bottom right. For all final possitions the puzzle is solvable only when the inversions are even.
        */
}

bool Node::isEqual(Node* other){
    if(this->size != other->size) return false;
    for(int i = 0; i < this->size*this->size; i++){
        if(this->matrix[i/this->size][i%this->size] != other->matrix[i/this->size][i%this->size])return false;
    }
    return true;
}

bool isInPath(Node* previous, Node* target){
    while(previous != nullptr){
        if(target->isEqual(previous)) return true;
        previous = previous->getParent();
    }
    return false;
}

void Node::printPath(){
    if(this->direction == "\0") return;

    parent->printPath();
    std::cout << this->direction << std::endl;
}

int search(Node* currentNode, const int& currentCost, const int& bound){
    int f = currentCost + currentNode->totalDistance();
    if(f > bound) return f;
    if(currentNode->isGoal()){
        std::cout << bound << std::endl;
        currentNode->printPath();
        return -1;
    }
    int min = INT16_MAX;
    int res;

    Node** neighbour = currentNode->neighbours();
    for(int i = 0; neighbour[i] != nullptr; i++){
        for(int j = i+1; neighbour[j] != nullptr; j++){
            if(neighbour[j-1]->totalDistance() < neighbour[j]->totalDistance())
                std::swap(neighbour[j],neighbour[j-1]);
        }
    }

    Node* previous = new Node(currentNode);
    for(int i = 0; neighbour[i] != nullptr; i++){
        if(isInPath(previous, neighbour[i])){
            delete neighbour[i];    
            neighbour[i] = nullptr;
            continue;
        }

        //neighbour[i]->setParent(currentNode);
        res = search(neighbour[i], currentCost+1, bound);

        if(res == -1){
            return -1;
        }else if(res < min) 
            min = res;

        delete neighbour[i];
        neighbour[i] = nullptr;
    }
    delete neighbour;
    return min;
}

void ida_star(Node* root){
    int bound = root->totalDistance(), res;
    while(true){
        res = search(root, 0, bound);
        if(res == -1){            
            return;
        }else if(res == -2){
            std::cout << -1;
            return;
        }
        bound = res;
    }
}

int main(){
    int size, location;
    std::cin >> size >> location;
    Node node(std::sqrt(size+1), location); // working with size of a row
    node.inputMatrix();

    if(node.isSolvable()){
        ida_star(&node);
    }else{
        std::cout << -1 << std::endl;
    }
    return 0;
}