#include <iostream>
#include <string>

#define ITER_COUNT 3
#define POOL_SIZE 10
#define MAX_CITY_COUNT 100

int size = 0;
struct City {
	std::string name;
	float x, y;
};

City cities[MAX_CITY_COUNT];
short generation[POOL_SIZE][MAX_CITY_COUNT], parents[POOL_SIZE][MAX_CITY_COUNT];
int distance[POOL_SIZE], fitness[POOL_SIZE];

void init(const int &iter) {};
int eval(const int &iter) {};
void selectParents(const int& iter) {};
void reproduction(short parents[POOL_SIZE][MAX_CITY_COUNT], short current[POOL_SIZE][MAX_CITY_COUNT]) {};
void mutate(const int& iter) {};

int geneticAlgorithm(){
	int count = 0;
	init(count);
	int min_distance = eval(count);

	for (int i = 0; i < ITER_COUNT; i++) {
		selectParents(count);
		reproduction(parents, generation);
		
		count++;
		mutate(count);
		min_distance = std::min(min_distance,eval(count));
		std::cout << min_distance;
	}

	return min_distance;
};

bool hasLetter( std::string& string) {
	if (string.size() > 3) return true;
	
	for (char &ch : string) {
		if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) return true;
	}

	return false;
}

void printCities(const float &wanted_value) {
	int i = 0;
	for (; distance[i] != wanted_value; i++);

	std::cout << cities[generation[i][0]].name;
	for (int j = 1; j < size; j++) {
		std::cout << " -> " << cities[generation[i][j]].name;
	}
	std::cout << std::endl;
}

int main() {
	std::string input;
	float result;
	std::getline(std::cin, input);
	if (hasLetter(input)) {
		std::cin >> size;
		for (int i = 0; i < size; i++) {
			std::cin >> cities[i].name >> cities[i].x >> cities[i].y;
		}

		result = geneticAlgorithm();

		std::cout << std::endl;
		printCities(result);
		std::cout << result << std::endl;
	} else {
		size = stoi(input);
		for (int i = 0; i < size; i++) {
			std::cin >> cities[i].name >> cities[i].x >> cities[i].y;
		}

		result = geneticAlgorithm();

		std::cout << std::endl << result << std::endl;
	}

	return 0;
}