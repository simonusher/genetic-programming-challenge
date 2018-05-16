#pragma once
#include <windows.h>
#include <stdlib.h>
#include <iostream>
#include <atlstr.h>
#include <ctime>
#include <sstream>
#include <algorithm>
#include "Tree.h"

using namespace std;

const int DEFAULT_SIZE_OF_POPULATION = 100;
const int SECOND_SIZE_OF_POPULATION = 200;
const int DEFAULT_HYBRIDIZATION_PROBABILITY = 70;
const int DEFAULT_MUTATION_PROBABILITY = 10;

const int ITERATIONS_WITHOUT_IMPROVEMENT_CUTTING_THRESHOLD = 50;

const int MUTATION_MULTIPLIER = 3;
const int POPULATION_MULTIPLIER = 2;

const int DEFAULT_TREE_CUTTING_DEPTH = 7;

const double POPULATION_PURGED_PERCENTAGE = 0.5;

const int FIRST_THRESHOLD = 50;
const int SECOND_THRESHOLD = 400;

const double ELITE_PERCENTAGE = 0.1;

class  CPGAlg
{
public:
	CPGAlg();
	~CPGAlg();
	bool bInitialize(CString  sTest);
	void vRunIteration();
	CString sGetCurrentBestTree();

private:
	void reset();
	void initializePopulation();
	void printPopulation();
	void readFunctionDataFromFile(CString filename);
	void select();
	void hybridize();
	void mutate();
	void evaluate();
	void sortPopulationByFitness();
	void chooseElitePercent();
	Tree* getBestTreePtr();
	void deleteOldPopulation();
	void deleteParentPopulation();
	Tree* getRandomTreeFromPopulation();
	void generateNewTrees();
	void addNewTreesToPopulation(int numberOfTrees);
	void replaceFourWorstTrees();

	void selectNewBestTree();
	void chooseStrategy();

	int sizeOfPopulation;
	int hybridizationProbability;
	int mutationProbability;
	std::vector<Tree*> population;
	std::vector<Tree*> parentPopulation;
	std::vector<std::vector<double>> functionArgumentsAndValues;
	int iterationsWithoutImprovement;
	Tree *lastBestTree;
};//class  CPGAlg