#include "Tree.h"

Tree::Tree() {
	this->root = nullptr;
	this->isEvaluated = false;
	this->evaluationSuccess = true;
}

Tree::~Tree() {
	if (this->root != nullptr) {
		delete this->root;
	}
}

Tree::Tree(Tree &otherTree) {
	if (otherTree.root != nullptr) {
		this->root = new Node(*otherTree.root);
		this->fitness = otherTree.fitness;
		this->isEvaluated = otherTree.isEvaluated;
		this->evaluationSuccess = otherTree.evaluationSuccess;
	}
	else {
		this->root = nullptr;
	}
}

std::string Tree::getONPFormula() {
	return this->root->getONPFormula();
}

void Tree::cutTreeDown(int depth) {
	root->cutDown(0, depth);
	this->isEvaluated = false;
}

Tree& Tree::mutate() {
	if (this->root->children.size() >= 2) {
		int mutationTypeDeterminer = std::rand() % MAX_PROBABILITY;
		if (mutationTypeDeterminer < NODE_SWAP_PROBABILITY) {
			swapRandomNodes();
		}
		else {
			changeStructure();
		}
	}
	else {
		changeStructure();
	}
	this->isEvaluated = false;
	return *this;
}

void Tree::generateDataFile(std::vector<std::vector<double>>& environments, std::string filename) {
	std::ofstream file;
	file.open(filename);
	std::vector<double> currentEnv;
	double x, y, h;
	for (int i = 0; i < environments.size(); i++) {
		currentEnv = environments.at(i);
		x = currentEnv.at(0);
		y = currentEnv.at(1);
		h = root->computeValue(x, y, this->evaluationSuccess);
		file << x << ";" << y << ";" << h << ";" << std::endl;
	}
}

void Tree::swapRandomNodes() {
	Node **firstNode = root->children.at(0)->getRandomNodeFromTree(&root->children.at(0));
	Node **secondNode = root->children.at(1)->getRandomNodeFromTree(&root->children.at(1));
	Node *temp = *firstNode;
	*firstNode = *secondNode;
	*secondNode = temp;
}

void Tree::changeStructure() {
	if (root->children.size() == 0) {
		delete root;
		root = Node::randomizeNode();
	}
	else {
		Node **randomNode = root->getRandomNodeFromTree(&root);
		delete *randomNode;
		*randomNode = Node::randomizeNode();
	}
}

double Tree::square(const double &value) {
	return value * value;
}

Tree* Tree::randomizeTree() {
	Tree *randomTree = new Tree();
	randomTree->root = Node::randomizeNode();
	return randomTree;
}

std::vector<Tree*> Tree::hybridizeWith(Tree &otherTree) {
	std::vector<Tree*> result;
	int firstTreeChildrenNumber = this->root->children.size();
	int secondTreeChildrenNumber = otherTree.root->children.size();
	Tree *firstResult = new Tree(*this);
	Tree *secondResult = new Tree(otherTree);
	if (firstTreeChildrenNumber == 0) {
		if (secondTreeChildrenNumber != 0) {
			int childIndex = std::rand() % secondResult->root->children.size();
			delete secondResult->root->children.at(childIndex);
			secondResult->root->children.at(childIndex) = new Node(*firstResult->root);
			secondResult->isEvaluated = false;
		}
	}
	else {
		if (secondTreeChildrenNumber == 0) {
			int childIndex = std::rand() % firstResult->root->children.size();
			delete firstResult->root->children.at(childIndex);
			firstResult->root->children.at(childIndex) = new Node(*secondResult->root);
			firstResult->isEvaluated = false;
		}
		else {
			int firstChildIndex = std::rand() % firstResult->root->children.size();
			int secondChildIndex = std::rand() % secondResult->root->children.size();
			Node *firstSubTree = new Node(*firstResult->root->children.at(firstChildIndex));
			Node *secondSubTree = new Node(*secondResult->root->children.at(secondChildIndex));
			delete firstResult->root->children.at(firstChildIndex);
			delete secondResult->root->children.at(secondChildIndex);
			firstResult->root->children.at(firstChildIndex) = secondSubTree;
			secondResult->root->children.at(secondChildIndex) = firstSubTree;
			firstResult->isEvaluated = false;
			secondResult->isEvaluated = false;
		}
	}
	result.push_back(firstResult);
	result.push_back(secondResult);
	return result;
}

double Tree::evaluate(std::vector<std::vector<double>>& environments) {
	double result = 0.0;
	std::vector<double> currentEnv;
	double currentEnvResult = 0.0;
	double x, y, f;
	for(int i = 0; i < environments.size() && this->evaluationSuccess == true; i++) {
		currentEnv = environments.at(i);
		x = currentEnv.at(0);
		y = currentEnv.at(1);
		f = currentEnv.at(2);
		currentEnvResult = root->computeValue(x, y, this->evaluationSuccess) - f;
		result += square(currentEnvResult);
	}
	this->fitness = result;
	this->isEvaluated = true;
	return result;
}

bool Tree::comparePtrToTree(Tree *first, Tree *second) {
	return first->fitness < second->fitness;
}
