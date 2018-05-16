#pragma once
#include <string>
#include <cstdlib>
#include <vector>
#include <cmath>
#include "constants.h"

const enum NodeType {
	OperationSum,
	OperationSub,
	OperationMult,
	OperationDiv,
	OperationSin,
	OperationCos,
	Constant,
	Variable
};

const int NUMBER_OF_OPERATIONS = 6;
const int NUMBER_OF_TYPES = 8;

const std::string OPERATION_SUM = "+";
const std::string OPERATION_SUB = "-";
const std::string OPERATION_MULT = "*";
const std::string OPERATION_DIV = "/";
const std::string OPERATION_SIN = "sin";
const std::string OPERATION_COS = "cos";

const int STOPPING_PROBABILITY = 60;

const int DISTRIB_OPERATION = 40;
const int DISTRIB_CONSTANT = 70;
const int DISTRIB_VARIABLE = MAX_PROBABILITY;

const std::vector<std::string> VAR_NAMES = { "x", "y" };

class Node {
public:
	friend class Tree;
	friend class CPGAlg;
	Node();
	Node(Node &otherNode);
	~Node();
	static Node* randomizeNode();

	std::string getONPFormula();
	void cutDown(int currentDepth, int depth);
	double computeValue(double &xValue, double &yValue, bool &evaluationSuccess);
private:
	Node** getRandomNodeFromTree(Node **lastPtr);
	void changeStructure(Node **pointerToChange);
	static int randomType();
	static int randomOperation();
	static int randomConstant();
	static std::string randomVariable();
	int nodeType;
	std::vector<Node*> children;
	std::string variableName;
	int constantValue;
};