#include <iostream>

int const MAX_DIMENSION = 10;

class Node{
    private:
        int positionMissingPiece;
        //int distance = 0;
        Node* parent;
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
};

Node::Node(const int& size, const int& positionMissingPiece, Node* parent){
    this->size = size;
    matrix = new int*[size];
    for(int i = 0; i < size; i++){
        matrix[i] = new int[size];
    }
    this->positionMissingPiece = (positionMissingPiece == -1) ? 9 : positionMissingPiece;
    this->parent = parent;
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
    int move;
    int** currentMatrix;
    for(int i = 0; i < 4; i++){
        move = currentMissingPiece + positions[i];
        if(move <= 0 || move >= this->size * this->size ||
            (move%this->size != currentMissingPiece%this->size && move/this->size != currentMissingPiece/this->size)) continue;

        neighbours[countNeighbours] = new Node(this);
        neighbours[countNeighbours]->parent = this;
        neighbours[countNeighbours]->currentMissingPiece = move;
        std::swap(neighbours[countNeighbours]->matrix[move/this->size][move%this->size], neighbours[countNeighbours]->matrix[currentMissingPiece/this->size][currentMissingPiece%this->size]);
        
        countNeighbours++;
    }
    return neighbours;
}

bool Node::isSolvable(){
    int inversions = 0;
    for(int row = 0; row < this->size; row++){
        for(int col = 0; col < this->size; col++){
            if(this->matrix[row][col] == 0)continue;

            for(int next = col + 1; next < this->size; next++ ){
                if(this->matrix[row][next] == 0)continue;

                inversions += (this->matrix[row][col] > this->matrix[row][next]);
            }
        }
    }
    return inversions % 2 == 0;
    /*return (this->size%2 == 1 && inversions % 2 == 0) || 
        (this->size%2 == 0 && this->positionMissingPiece%2==(inversions+this->positionMissingPiece)%2);
        The given algorithm checks if a puzzle is solvable only for bottom right. For all final possitions the puzzle is solvable only when the inversions are even.
        */
}

bool isInPath(Node* previous, Node* target){
    while(previous != nullptr){
        if(target == previous) return true;
        previous = previous->getParent();
    }
    return false;
}

int search(Node* currentNode, const int& currentCost, const int& bound){
    int f = currentCost + currentNode->totalDistance();
    if(f > bound) return f;
    if(currentNode->isGoal()) return -1;
    int min = INT16_MAX;
    int res;

    Node** neighbour = currentNode->neighbours();
    for(int i = 0; neighbour[i] != nullptr; i++){
        for(int j = i+1; neighbour[j] != nullptr; j++){
            if(neighbour[j-1]->totalDistance() < neighbour[j]->totalDistance())
                std::swap(neighbour[j],neighbour[j-1]);
        }
    }
    Node* previous = currentNode;
    for(int i = 0; neighbour[i] != nullptr; i++){
        if(isInPath(previous, neighbour[i])) continue;

        neighbour[i]->setParent(currentNode);
        res = search(neighbour[i], currentCost+1, bound);

        if(res == -1){
            currentNode->print();
            std::cout << "====" << std::endl;
            return -1;
        }else if(res < min) min = res;

        delete neighbour[i];
    }
    return min;
}

void ida_star(Node* root){
    int bound = root->totalDistance(), res;
    while(true){
        res = search(root, 0, bound);
        if(res == -1){
            std::cout << bound;
            // ...
            return;
        }else if(res == -2){
            std::cout << -1;
            return;
        }
        bound = res;
    }
}

int main(){
    Node test(3, -1);
    test.inputMatrix();

    //test.print();
    // Node** children = test.neighbours();
    // for(int i = 0; children[i] != nullptr; i++){
    //     std::cout << "=======" << std::endl;
    //     children[i]->print();
    // }
    std::cout << std::boolalpha << test.isSolvable() << std::endl;
    if(test.isSolvable()){
        ida_star(&test);
    }
    return 0;
}