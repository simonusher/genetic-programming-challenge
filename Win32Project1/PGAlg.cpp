#include  "PGAlg.h"

CPGAlg::CPGAlg() {
	this->sizeOfPopulation = DEFAULT_SIZE_OF_POPULATION;
	this->hybridizationProbability = DEFAULT_HYBRIDIZATION_PROBABILITY;
	this->mutationProbability = DEFAULT_MUTATION_PROBABILITY;
	this->iterationsWithoutImprovement = 0;
	this->lastBestTree = nullptr;
}

CPGAlg::~CPGAlg() {
	deleteOldPopulation();
	deleteParentPopulation();
	if (lastBestTree != nullptr) {
		delete lastBestTree;
	}
}

bool CPGAlg::bInitialize(CString  sTest) {
	reset();
	std::srand(std::time(0));
	readFunctionDataFromFile(sTest);
	initializePopulation();
	evaluate();
	lastBestTree = new Tree(*getBestTreePtr());
	return(true);
}

void CPGAlg::vRunIteration() {
	select();
	deleteOldPopulation();
	hybridize();
	deleteParentPopulation();
	mutate();
	evaluate();

	selectNewBestTree();
	chooseStrategy();
}//void CPGAlg::vRunIteration()


CString CPGAlg::sGetCurrentBestTree() {
	return(lastBestTree->getONPFormula().c_str());
}


void CPGAlg::reset() {
	this->sizeOfPopulation = DEFAULT_SIZE_OF_POPULATION;
	this->hybridizationProbability = DEFAULT_HYBRIDIZATION_PROBABILITY;
	this->mutationProbability = DEFAULT_MUTATION_PROBABILITY;
	this->iterationsWithoutImprovement = 0;
	functionArgumentsAndValues.clear();
	deleteOldPopulation();
	deleteParentPopulation();
	if (lastBestTree != nullptr) {
		delete lastBestTree;
		lastBestTree = nullptr;
	}
}

void CPGAlg::initializePopulation() {
	for (int i = 0; i < sizeOfPopulation; i++) {
		this->population.push_back(Tree::randomizeTree());
	}
}

void CPGAlg::printPopulation() {
	for (int i = 0; i < population.size(); i++) {
		std::cout << population.at(i)->getONPFormula() << " " << population.at(i)->fitness << std::endl;
	}
}

void CPGAlg::readFunctionDataFromFile(CString filename) {
	std::ifstream file(filename);
	std::string line;
	std::string item;
	if (file.is_open()) {
		while (std::getline(file, line)) {
			std::vector<double> dataVec;
			std::string token;
			std::istringstream tokenStream(line);
			while (std::getline(tokenStream, token, ';')) {
				dataVec.push_back(std::stod(token));
			}
			if(dataVec.size() == 3){
				functionArgumentsAndValues.push_back(dataVec);
			}
		}
	}
}

void CPGAlg::select() {
	sortPopulationByFitness();
	chooseElitePercent();
	Tree* firstTree;
	Tree* secondTree;
	while (parentPopulation.size() != sizeOfPopulation) {
		firstTree = getRandomTreeFromPopulation();
		secondTree = getRandomTreeFromPopulation();
		while (secondTree == firstTree) {
			secondTree = getRandomTreeFromPopulation();
		}
		if (firstTree->fitness < secondTree->fitness) {
			parentPopulation.push_back(new Tree(*firstTree));
		}
		else {
			parentPopulation.push_back(new Tree(*secondTree));
		}
	}
}

void CPGAlg::hybridize() {
	for (int i = 0; i < parentPopulation.size(); i += 2) {
		Tree* firstTree = parentPopulation.at(i);
		Tree* secondTree = parentPopulation.at(i + 1);
		int willHybridize = std::rand() % MAX_PROBABILITY;
		if (willHybridize < hybridizationProbability) {
			std::vector<Tree*> newTrees = firstTree->hybridizeWith(*secondTree);
			for (int i = 0; i < newTrees.size(); i++) {
				population.push_back(newTrees.at(i));
			}
		}
		else {
			population.push_back(new Tree(*firstTree));
			population.push_back(new Tree(*secondTree));
		}
	}
}

void CPGAlg::mutate() {
	for (int i = 0; i < population.size(); i++) {
		int willMutate = std::rand() % MAX_PROBABILITY;
		if (willMutate < mutationProbability) {
			population.at(i)->mutate();
		}
	}
}

void CPGAlg::evaluate() {
	Tree *tree;
	for (int i = 0; i < population.size(); i++) {
		tree = population.at(i);
		if (!tree->isEvaluated) {
			tree->evaluate(functionArgumentsAndValues);
			while (tree->evaluationSuccess == false) {
				delete tree;
				population.at(i) = Tree::randomizeTree();
				tree = population.at(i);
				tree->evaluate(functionArgumentsAndValues);
			}
		}
	}
}

void CPGAlg::sortPopulationByFitness() {
	std::sort(population.begin(), population.end(), Tree::comparePtrToTree);
}
void CPGAlg::chooseElitePercent() {
	int numberOfElite = ELITE_PERCENTAGE * sizeOfPopulation;
	for (int i = 0; i < numberOfElite; i++) {
		parentPopulation.push_back(new Tree(*population.at(i)));
	}
}

Tree *CPGAlg::getBestTreePtr() {
	Tree *lastBestTree = population.at(0);
	double lastBestFitness = lastBestTree->fitness;
	for (int i = 1; i < population.size(); i++) {
		if (population.at(i)->fitness < lastBestFitness) {
			lastBestTree = population.at(i);
			lastBestFitness = lastBestTree->fitness;
		}
	}
	return lastBestTree;
}

void CPGAlg::deleteOldPopulation() {
	for(int i = 0; i < population.size(); i++){
		delete population.at(i);
	}
	population.clear();
}
void CPGAlg::deleteParentPopulation() {
	for (int i = 0; i < parentPopulation.size(); i++) {
		delete parentPopulation.at(i);
	}
	parentPopulation.clear();
}
Tree * CPGAlg::getRandomTreeFromPopulation() {
	int index = std::rand() % population.size();
	return population.at(index);
}
void CPGAlg::generateNewTrees() {
	while (population.size() != sizeOfPopulation) {
		population.push_back(Tree::randomizeTree());
	}
}

void CPGAlg::addNewTreesToPopulation(int numberOfTrees) {
	for (int i = 0; i < numberOfTrees; i++) {
		if (sizeOfPopulation > population.size()) {
			Tree* newTree = Tree::randomizeTree();
			newTree->evaluate(functionArgumentsAndValues);
			while (!newTree->evaluationSuccess) {
				delete newTree;
				newTree = Tree::randomizeTree();
				newTree->evaluate(functionArgumentsAndValues);
			}
			population.push_back(newTree);
		}
	}
}

void CPGAlg::replaceFourWorstTrees() {
	sortPopulationByFitness();
	for (int i = 0, j = population.size() - 1; i < 4; i++, j--) {
		delete population.at(j);
	}
	population.resize(population.size() - 4);
	for (int i = 0, j = population.size() - 1; i < 4; i++, j--) {
		Tree* newTree = Tree::randomizeTree();
		newTree->evaluate(functionArgumentsAndValues);
		while (!newTree->evaluationSuccess) {
			delete newTree;
			newTree = Tree::randomizeTree();
			newTree->evaluate(functionArgumentsAndValues);
		}
		population.push_back(newTree);
	}
}

void CPGAlg::selectNewBestTree() {
	Tree *bestTree = getBestTreePtr();
	if (bestTree->fitness < lastBestTree->fitness) {
		delete lastBestTree;
		lastBestTree = new Tree(*bestTree);
		iterationsWithoutImprovement = 0;
	}
	else {
		iterationsWithoutImprovement++;
	}
}

void CPGAlg::chooseStrategy() {
	if (iterationsWithoutImprovement == 0) {
		sizeOfPopulation = DEFAULT_SIZE_OF_POPULATION;
		mutationProbability = DEFAULT_MUTATION_PROBABILITY;
		if (population.size() > sizeOfPopulation) {
			sortPopulationByFitness();
			for (int i = population.size() - 1; i >= sizeOfPopulation; i--) {
				delete population.at(i);
			}
			population.resize(sizeOfPopulation);
		}
	}

	else if (sizeOfPopulation < SECOND_SIZE_OF_POPULATION) {
		sizeOfPopulation += 2;
		addNewTreesToPopulation(2);
	}

	if (iterationsWithoutImprovement == FIRST_THRESHOLD) {
		mutationProbability = DEFAULT_MUTATION_PROBABILITY * MUTATION_MULTIPLIER;
	}

	else if (iterationsWithoutImprovement > 0 && iterationsWithoutImprovement % SECOND_THRESHOLD == 0) {
		sortPopulationByFitness();
		for (int i = population.size() - 1; i >= population.size() - POPULATION_PURGED_PERCENTAGE * population.size(); i--) {
			delete population.at(i);
			population.at(i) = Tree::randomizeTree();
		}
		evaluate();
	}

	if (iterationsWithoutImprovement % ITERATIONS_WITHOUT_IMPROVEMENT_CUTTING_THRESHOLD == 0) {
		for (int i = 0; i < population.size(); i++) {
			population.at(i)->cutTreeDown(DEFAULT_TREE_CUTTING_DEPTH);
		}
		evaluate();
	}

	if (iterationsWithoutImprovement > SECOND_THRESHOLD) {
		replaceFourWorstTrees();
	}
}
