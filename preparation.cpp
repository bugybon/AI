#include <iostream>
#include <string>
#include <queue>
#include <random>

template <typename T>
struct Node {
	T value;
	int size;
	Node **children;
	int *costs;
};

//ChatGPT generated function to make varying graphs
Node<std::string>* makeGraph(const std::string& base, int depth, int maxDepth, int branching) {
	int children_count = rand() % branching;
	std::string name;

	if (depth == maxDepth || children_count == 0) {
		return new Node<std::string> { base, 0, nullptr, nullptr };
	}

	Node<std::string>** children = new Node<std::string>*[children_count];
	int* costs = new int[children_count];

	for (int i = 0; i < children_count; ++i) {
		name = base + "_" + std::to_string(i + 1);
		children[i] = makeGraph(name, depth + 1, maxDepth, branching);
		costs[i] = rand() % 100;
	}

	return new Node<std::string>{ base, children_count, children, costs };
}

void bfs(Node<std::string>* root){
	std::queue<Node<std::string>*> nodes;
	Node<std::string>* current;

	nodes.push(root);
	while (!nodes.empty()) {
		current = nodes.front();
		std::cout << current->value << std::endl;

		for (int i = 0; i < current->size; i++) {
			nodes.push(current->children[i]);
		}
		nodes.pop();
	}
}

void dfs(Node<std::string>* root) {
	if (root == nullptr)
		return;

	std::cout << root->value << std::endl;
	for (int i = 0; i < root->size; i++)
		dfs(root->children[i]);
}

void ucs(Node<char>* root, Node<char>* goal) {
	bool visited[26];
	std::string paths[26];
	int costToNode[26];
	std::priority_queue<Node<char>*> candidates;
	Node<char>* currentNode;
	short currentIndex, parentIndex;

	for (int i = 0; i < 26; i++) {
		costToNode[i] = INT8_MAX;
		visited[i] = false;
	}

	candidates.push(root);
	currentIndex= root->value - 'A';
	visited[currentIndex] = true;
	costToNode[currentIndex] = 0;
	paths[currentIndex] = root->value;

	while (!candidates.empty() || candidates.top() == goal) {
		currentNode = candidates.top();
		candidates.pop();

		parentIndex = currentIndex;
		for (int i = 0; i < currentNode->size; i++) {
			currentIndex = currentNode->children[i]->value - 'A';
			if (visited[currentIndex])
				continue;

			if (costToNode[currentIndex] > costToNode[parentIndex] + currentNode->costs[i]) {
				costToNode[currentIndex] = costToNode[parentIndex] + currentNode->costs[i];

				paths[currentIndex] = paths[parentIndex] + "->" + (char)(currentIndex + 'A');

			}
		}
	}
}

int main() {
	srand(time(0));
	bfs(makeGraph("1", 1, 5, 4));


	return 0;
}