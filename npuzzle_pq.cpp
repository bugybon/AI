#include <iostream>
#include <cmath>
#include <vector>
#include <queue>

template<typename T>
class Compare
{
public:
    bool operator()(T left, T right){return left->getDistance() < right->getDistance();};
};

class Node{
    private:
        int positionMissingPiece;
        int distance;
        Node* parent;
        std::string direction;
        int size;
        int** matrix;
        int currentMissingPiece;
        
    public:
        Node(const int& size = 1, const int& positionMissingPiece = 1, Node* parent = nullptr);
        Node(Node* other);

        Node* getParent(){return this->parent;};
        void setParent(Node* other){this->parent = other;};
        int getDistance(){return this->distance;};
        void setDistance(const int& dist){this->distance = dist;};
        void inputMatrix();
        std::priority_queue<Node*,std::vector<Node*>,Compare<Node*>> neighbours();
        int distanceTile(int col, int row);
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
    this->currentMissingPiece = other->currentMissingPiece;
    this->parent = other->parent;
    this->direction = other->direction;
}

int Node::distanceTile(int col, int row){
    int current = matrix[row][col];
    if(current == 0) return 0;
    
    if(current < positionMissingPiece) current -= 1;

    return std::abs(col-current%this->size) + std::abs(row-current/this->size);
}

int Node::totalDistance(){
    int sum = 0;
    for(int row = 0; row < this->size; row++){
        for(int col = 0; col < this->size; col++){
            sum += this->distanceTile(col, row);
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
    this->distance = this->totalDistance();
}

std::priority_queue<Node*,std::vector<Node*>,Compare<Node*>> Node::neighbours(){
    std::priority_queue<Node*,std::vector<Node*>,Compare<Node*>> neighbours;
    
    int positions[] = {-this->size, -1, 1, this->size};
    std::string directions[] = {"down", "right", "left", "up"};
    int move;
    Node* temp;
    for(int i = 0; i < 4; i++){
        move = currentMissingPiece + positions[i];
        if(move < 0 || move >= this->size * this->size ||
            ((move%this->size) != (currentMissingPiece%this->size) && (move/this->size) != (currentMissingPiece/this->size))) continue;
            
            temp = new Node(this);
            temp->currentMissingPiece = move;
            temp->direction = directions[i];
            temp->setDistance(temp->getDistance() - this->distanceTile(currentMissingPiece%this->size,currentMissingPiece/this->size) +temp->distanceTile(move%this->size,move/this->size));
            std::swap(temp->matrix[move/this->size][move%this->size], temp->matrix[currentMissingPiece/this->size][currentMissingPiece%this->size]);
            neighbours.push(temp);
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
    return (this->size%2 == 1 && inversions % 2 == 0) || 
        (this->size%2 == 0 && (inversions+this->currentMissingPiece/this->size)%2 == 1);
        //The given algorithm checks if a puzzle is solvable only for bottom right. For all final possitions the puzzle is solvable only when the inversions are even.
        
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
    std::priority_queue<Node*,std::vector<Node*>,Compare<Node*>> neighbours = currentNode->neighbours();

    Node* previous = new Node(currentNode), *neighbour;
    while(!neighbours.empty()){
        neighbour = neighbours.top();
        neighbours.pop();

        if(isInPath(previous, neighbour)){
            // delete neighbour;    
            // neighbour = nullptr;
            continue;
        }
        //neighbour[i]->setParent(currentNode);
        res = search(neighbour, currentCost+1, bound);

        if(res == -1){
            delete neighbour;
            neighbour = nullptr;
            return -1;
        }else if(res < min) 
            min = res;

        // delete neighbour;
        // neighbour = nullptr;
    }
    delete previous;
    //delete neighbours;
    delete neighbour;
    return min;
}

void ida_star(Node* root){
    int bound = root->getDistance(), res;
    while(true){
        res = search(root, 0, bound);
        std::cout << bound << std::endl;
        if(res == -1){            
            return;
        }else if(res == INT16_MAX){
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