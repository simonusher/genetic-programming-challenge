#pragma once
#include <fstream>
#include "Node.h"

class Tree {
public:
	const int NODE_SWAP_PROBABILITY = 50;

	friend class CPGAlg;
	Tree();
	Tree(Tree &otherTree);
	~Tree();
	std::string getONPFormula();

	void cutTreeDown(int depth);
	Tree& mutate();
	static Tree* randomizeTree();
	std::vector<Tree*> hybridizeWith(Tree &otherTree);
	double evaluate(std::vector<std::vector<double>> &environments);
	static bool comparePtrToTree(Tree *first, Tree *second);
	void swapRandomNodes();
	void changeStructure();
	void generateDataFile(std::vector<std::vector<double>> &environments, std::string filename);
private:
	double square(const double &value);
	Node *root;
	double fitness;
	bool isEvaluated;
	bool evaluationSuccess;
};