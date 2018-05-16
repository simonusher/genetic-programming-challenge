#include "Node.h"
Node::Node() {
}

Node::Node(Node &otherNode) {
	this->nodeType = otherNode.nodeType;
	if (this->nodeType == NodeType::Constant) {
		this->constantValue = otherNode.constantValue;
	}
	else if (this->nodeType == NodeType::Variable) {
		this->variableName = otherNode.variableName;
	}
	for (int i = 0; i < otherNode.children.size(); i++) {
		this->children.push_back(new Node(*(otherNode.children.at(i))));
	}
}


Node::~Node() {
	for each (Node* childPtr in children) {
		delete childPtr;
	}
}

void Node::cutDown(int currentDepth, int depth) {
	if (currentDepth == depth - 1) {
		for each (Node *child in children) {
			delete child;
		}
		children.clear();
		int typeDeterminant = std::rand() % 2;
		if (typeDeterminant == 0) {
			this->nodeType = Variable;
			this->variableName = randomVariable();
		}
		else {
			this->nodeType = Constant;
			this->constantValue = randomConstant();
		}
	}
	else {
		for each (Node* child in children) {
			child->cutDown(currentDepth + 1, depth);
		}
	}
}

std::string Node::getONPFormula() {
	std::string result;
	switch (nodeType) {
	case Variable:
		result = variableName;
		break;
	case Constant:
		result = std::to_string(this->constantValue);
		break;
	case OperationCos:
		result = OPERATION_COS + SPACE + children.at(0)->getONPFormula();
		break;
	case OperationSin:
		result = OPERATION_SIN + SPACE + children.at(0)->getONPFormula();
		break;
	case OperationSum:
		result = OPERATION_SUM + SPACE + children.at(0)->getONPFormula() + SPACE + children.at(1)->getONPFormula();
		break;
	case OperationSub:
		result = OPERATION_SUB + SPACE + children.at(0)->getONPFormula() + SPACE + children.at(1)->getONPFormula();
		break;
	case OperationMult:
		result = OPERATION_MULT + SPACE + children.at(0)->getONPFormula() + SPACE + children.at(1)->getONPFormula();
		break;
	case OperationDiv:
		result = OPERATION_DIV + SPACE + children.at(0)->getONPFormula() + SPACE + children.at(1)->getONPFormula();
		break;
	}
	return result;
}

double Node::computeValue(double &xValue, double &yValue, bool &evaluationSuccess) {
	double leftChildValue = 0;
	double rightChildValue = 1;
	switch (this->nodeType) {
	case OperationSum:
		return this->children.at(0)->computeValue(xValue, yValue, evaluationSuccess) + this->children.at(1)->computeValue(xValue, yValue, evaluationSuccess);
		break;
	case OperationSub:
		return this->children.at(0)->computeValue(xValue, yValue, evaluationSuccess) - this->children.at(1)->computeValue(xValue, yValue, evaluationSuccess);
		break;
	case OperationMult:
		return this->children.at(0)->computeValue(xValue, yValue, evaluationSuccess) * this->children.at(1)->computeValue(xValue, yValue, evaluationSuccess);
		break;
	case OperationDiv:
		leftChildValue = this->children.at(0)->computeValue(xValue, yValue, evaluationSuccess);
		rightChildValue = this->children.at(1)->computeValue(xValue, yValue, evaluationSuccess);
		if (rightChildValue == 0) {
			evaluationSuccess = false;
		}
		return leftChildValue / rightChildValue;
		break;
	case OperationSin:
		return std::sin(this->children.at(0)->computeValue(xValue, yValue, evaluationSuccess));
		break;
	case OperationCos:
		return std::cos(this->children.at(0)->computeValue(xValue, yValue, evaluationSuccess));
		break;
	case Constant:
		return this->constantValue;
		break;
	case Variable:
		if (this->variableName == VAR_NAMES.at(0)) {
			return xValue;
		}
		else if (this->variableName == VAR_NAMES.at(1)) {
			return yValue;
		}
		else {
			return 0;
		}
		break;
	}
	return 0;
}

Node **Node::getRandomNodeFromTree(Node **lastPtr) {
	if (this->children.size() == 0) {
		return lastPtr;
	}
	else {
		int stopOrGoDeeper = std::rand() % MAX_PROBABILITY;
		if (stopOrGoDeeper < STOPPING_PROBABILITY) {
			return lastPtr;
		}
		else {
			int childIndex = std::rand() % children.size();
			return children.at(childIndex)->getRandomNodeFromTree(&children.at(childIndex));
		}
	}
}

void Node::changeStructure(Node **pointerToChange) {
	int stopOrGoDeeper = std::rand() % MAX_PROBABILITY;
	if (stopOrGoDeeper < STOPPING_PROBABILITY || this->children.size() == 0) {
		delete *pointerToChange;
		*pointerToChange = randomizeNode();
	}
	else {
		int childIndex = std::rand() % children.size();
		changeStructure(&children.at(childIndex));
	}
}

Node* Node::randomizeNode() {
	Node *randomNode = new Node();
	int type = randomType();
	randomNode->nodeType = type;
	if (type == NodeType::Constant) {
		randomNode->constantValue = randomConstant();
	}
	else if (type == NodeType::Variable) {
		randomNode->variableName = randomVariable();
	}
	else {
		switch (randomNode->nodeType) {
		case OperationSin:
		case OperationCos:
			randomNode->children.push_back(randomizeNode());
			break;
		default:
			randomNode->children.push_back(randomizeNode());
			randomNode->children.push_back(randomizeNode());
		}

	}
	return randomNode;
}

int Node::randomType() {
	int typeDeterminer = std::rand() % MAX_PROBABILITY;
	if (typeDeterminer < DISTRIB_OPERATION) {
		return randomOperation();
	}
	else if (typeDeterminer < DISTRIB_CONSTANT) {
		return NodeType::Constant;
	}
	else {
		return NodeType::Variable;
	}
}

int Node::randomOperation() {
	return std::rand() % NUMBER_OF_OPERATIONS;
}

int Node::randomConstant() {
	return std::rand();
}

std::string Node::randomVariable() {
	int index = std::rand() % VAR_NAMES.size();
	return VAR_NAMES.at(index);
}