#include <iostream>
#include <string>

#define ITER_COUNT 10000
#define POOL_SIZE 20
#define MAX_CITY_COUNT 100
#define TOTAL_CHANCE 6
#define SUCCESS_CUTOFF 1

short size = 0;
struct City {
	std::string name;
	float x, y;
};

City cities[MAX_CITY_COUNT];
short generation[POOL_SIZE][MAX_CITY_COUNT], parents[POOL_SIZE][MAX_CITY_COUNT], solution[MAX_CITY_COUNT];
bool used[POOL_SIZE];
float distance[POOL_SIZE];
int fitness[POOL_SIZE];

float distanceBetween(const int& first_index, const int& second_index) {
	return std::sqrt((cities[first_index].x - cities[second_index].x)* (cities[first_index].x - cities[second_index].x)+
		(cities[first_index].y - cities[second_index].y)*(cities[first_index].y - cities[second_index].y));
};

void promoteToParent(const int& parent, const int& chosen) {
	for (short i = 0; i < size; i++)
		parents[parent][i] = generation[chosen][i];
}

void findWanted(const int& value) {
	for (int pool = 0; pool < POOL_SIZE; pool++) {
		if (value != distance[pool]) continue;

		for (int i = 0; i < size; i++) {
			solution[i] = generation[pool][i];
		}
		return;
	}
}

void crossover(const int& adopter, const int& giver, const short& start, const short& end) {
	for (int i = 0; i < size; i++)
		used[i] = false;

	for (int i = start; i < end; i++) {
		used[parents[adopter][i]] = true;
		generation[adopter][i] = parents[adopter][i];
	}

	int adopted = 0;
	for (int i = 0; i < size && adopted < size - (end - start); i++) {
		if (used[parents[giver][(end + i) % size]]) continue;

		generation[adopter][(end + adopted) % size] = parents[giver][(end + i) % size];
		used[parents[giver][(end + i) % size]] = true;
		adopted++;
	}
}

void init() {
	short sequence[MAX_CITY_COUNT];
	short current;
	for (short i = 0; i < size; i++) {
		sequence[i] = i;
	}

	for (short pool = 0; pool < POOL_SIZE; pool++) {
		for (short i = 0; i < size; i++) {
			current = rand() % (size - i);
			generation[pool][i] = sequence[current];
			std::swap(sequence[current], sequence[size - i - 1]);
		}
	}
};

int eval() {
	for (short pool = 0; pool < POOL_SIZE; pool++)
		distance[pool] = 0;

	for (short pool = 0; pool < POOL_SIZE; pool++) {
		for (short i = 0; i < size - 1; i++) {
			distance[pool] += distanceBetween(generation[pool][i], generation[pool][i + 1]);
		}
	}
	float max = distance[0];
	for(short pool = 1;pool < POOL_SIZE; pool++)
		max = std::max(distance[pool] + 1, max);

	fitness[0] = std::ceil(max - distance[0]);
	int res = distance[0];
	for (short pool = 1; pool < POOL_SIZE; pool++) {
		fitness[pool] = std::ceil(max - distance[pool]);
		if (max - res < fitness[pool]) {
			res = distance[pool];
		}
	}
	return res;
};

void selectParents() {
	int sum_fitness = 0, prefix_sum, current;
	short candidate;

	for (short pool = 0; pool < POOL_SIZE; pool++)
		sum_fitness += fitness[pool];

	for (short pool = 0; pool < POOL_SIZE;pool++) {
		current = rand() % sum_fitness;
		prefix_sum = 0;
		candidate = 0;

		for (; prefix_sum < current; candidate++) {
			prefix_sum += fitness[candidate];
		}

		candidate = std::max(0, candidate - 1);
		promoteToParent(pool, candidate);
	}
};

void reproduction() {
	short start, end;
	for (int i = 0; i < POOL_SIZE / 2; i++) {
		start = rand() % size;
		end = (start + 1) == size ? (start + 1) : (start + 1 + rand() % (size - start - 1));
		crossover(i, i + 1, start, end);
		crossover(i + 1, i, start, end);
	}
};

void mutate() {
	short attampt_mutation, first, second;
	for (int i = 0; i < POOL_SIZE; i++) {
		attampt_mutation = rand() % TOTAL_CHANCE;

		if (attampt_mutation > SUCCESS_CUTOFF) continue;

		first = rand() % size;
		second = rand() % size;
		std::swap(generation[i][first], generation[i][second]);
	}
};


int geneticAlgorithm(){
	int count = 0, temp;
	init();
	int min_distance = eval();
	findWanted(min_distance);

	for (int i = 0; i < ITER_COUNT; i++) {
		selectParents();
		reproduction();
		
		count++;
		mutate();
		temp = eval();
		if(min_distance > temp){
			min_distance = temp;
			findWanted(min_distance);
		}
		//min_distance = std::min(min_distance,eval());
		std::cout << min_distance << std::endl;
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
	//int i = 0;
	//for (; distance[i] != wanted_value; i++);

	std::cout << cities[solution[0]].name;
	for (int j = 1; j < size; j++) {
		std::cout << " -> " << cities[solution[j]].name;
	}
	std::cout << std::endl;
}

int main() {
	srand(time(0));
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
			cities[i].x = (float)rand() / (float)rand();
			cities[i].y = (float)rand() / (float)rand();
		}

		result = geneticAlgorithm();

		std::cout << std::endl << result << std::endl;
	}

	return 0;
}