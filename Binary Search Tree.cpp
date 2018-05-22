#pragma once
#include <iostream>
#include "SafeArray.h"
#include "SafeArrayOutOfBoundsExceptions.h"
#include <cmath>
#include "TreeExceptions.h"

template <class T>
class BalancedTree
{
public:

	BalancedTree();
	~BalancedTree();

	void insert(const T& elem);
	bool search(const T& elem);

	bool remove(const T& elem);

	int length();

	void printInOrder();
	void printPreorder();
	void printPostOrder();

	T findMax();
	T findMin();

	int heightOfTree();

	double percentLeaf();
	double percentInterior();

	void printInTreeForm();

private:
	struct Node
	{
		T data;
		Node* p_left;
		Node* p_right;
		int heightL;
		int heightR;
		int height;
	};

	//SafeArray<Node*> tArray;

	int numElements;

	int numLeaves;
	double percentLeaves;

	double percentInteriorNodes;

	Node* p_root;

	void destructorHelper(Node* p_node);

	void insertHelper(const T& elem, Node*& p_node, bool& increaseHeight);

	bool searchHelper(const T& elem, Node* p_node);

	void printInOrderHelper(Node* p_node);
	void printPreorderHelper(Node* p_node);
	void printPostOrderHelper(Node* p_node);

	bool removeHelper(const T& elem, Node*& p_node, bool& decreaseHeight);
	bool removeLeaf(const T& elem, Node*& p_node, bool& decreaseHeight);
	bool removeWithChild(const T& elem, Node*& p_node, bool& decreaseHeight);
	bool removeWithChildren(const T& elem, Node*& p_node, bool& decreaseHeight);

	T findMaxHelper(Node* p_node);
	T findMinHelper(Node* p_node);

	int setHeights(Node* p_node);
	int heightOfTreeHelper(Node* p_node);

	double BalancedTree<T>::percentLeafHelper(Node* p_node);
	double BalancedTree<T>::percentInteriorHelper(Node* p_node);

	void balanceRight(Node*& p_node);
	void balanceLeft(Node*& p_node);

	void BalancedTree<T>::printHelper(int index, Node* p_node, SafeArray<Node*>& tree);
};
template <class T>
BalancedTree<T>::BalancedTree()
{
	numElements = 0;
	numLeaves = 0;
	percentLeaves = 0;
	p_root = 0;

}
//--
template <class T>
BalancedTree<T>::~BalancedTree()
{
	destructorHelper(p_root);
}
//--
template <class T>
void BalancedTree<T>::destructorHelper(Node* p_node)
{
	if (p_node != 0)
	{
		destructorHelper(p_node->p_left);
		destructorHelper(p_node->p_right);
		delete p_node;
		p_node = 0;
	}
}
//--
template <class T>
void BalancedTree<T>::insert(const T& elem)
{
	bool increaseHeight = true;
	insertHelper(elem, p_root, increaseHeight);
	//setHeights(p_root);

}
//--
template <class T>
void BalancedTree<T>::insertHelper(const T& elem, Node*& p_node, bool& increaseHeight)
{
	if (p_node == 0)
	{

		p_node = new Node;
		p_node->data = elem;

		p_node->p_left = 0;
		p_node->p_right = 0;

		p_node->heightL = 0;
		p_node->heightR = 0;

		increaseHeight = true;

		numElements++;
	}
	else if (elem < p_node->data)
	{

		insertHelper(elem, p_node->p_left, increaseHeight);
		if (increaseHeight == true)
		{
			p_node->heightL++;
			if (p_node->heightL > p_node->heightR)
			{
				increaseHeight = true;
			}
			else
			{
				increaseHeight = false;
			}
		}
		if (!(p_node->heightL - p_node->heightR <= 1 && p_node->heightL - p_node->heightR >= -1))
		{
			balanceLeft(p_node);
			if (!(p_node->heightL - p_node->heightR <= 1 && p_node->heightL - p_node->heightR >= -1))
			{
				balanceLeft(p_node->p_right);

				balanceRight(p_node);
			}
		}
	}
	else if (elem > p_node->data)
	{
		insertHelper(elem, p_node->p_right, increaseHeight);

		if (increaseHeight == true)
		{
			p_node->heightR++;
			if (p_node->heightR > p_node->heightL)
			{
				increaseHeight = true;
			}
			else
			{
				increaseHeight = false;
			}
		}

		//balancing
		if (!(p_node->heightL - p_node->heightR <= 1 && p_node->heightL - p_node->heightR >= -1))
		{
			balanceRight(p_node);
			if (!(p_node->heightL - p_node->heightR <= 1 && p_node->heightL - p_node->heightR >= -1))
			{
				balanceRight(p_node->p_left);

				balanceLeft(p_node);
			}
		}
	}
	else
	{
		TreeExceptions ex;
		throw ex;
	}
}
//--
template <class T>
bool BalancedTree<T>::search(const T& elem)
{
	return searchHelper(elem, p_root);
}
//--
template <class T>
bool BalancedTree<T>::searchHelper(const T& elem, Node* p_node)
{
	bool retVal = false;

	if (p_node != 0)
	{
		if (elem == p_node->data)
		{
			//cout << elem << endl;
			retVal = true;
		}
		else if (elem <= p_node->data)
		{
			searchHelper(elem, p_node->p_left);
		}
		else
		{
			searchHelper(elem, p_node->p_right);
		}
	}

	return retVal;
}
//-
template <class T>
bool BalancedTree<T>::remove(const T& elem)
{
	bool decreaseHeight = true;
	if (removeHelper(elem, p_root, decreaseHeight))
	{
		numElements--;
		//setHeights(p_root);
		return true;
	}
	else
	{
		return false;
	}
}
//--
template <class T>
bool BalancedTree<T>::removeHelper(const T& elem, Node*& p_node, bool& decreaseHeight)
{
	bool retVal = false;
	if (p_node != 0)
	{
		if (elem == p_node->data)
		{
			if (p_node->p_left == 0 && p_node->p_right == 0)
			{
				retVal = removeLeaf(elem, p_node, decreaseHeight);
			}
			else if (p_node->p_left == 0 || p_node->p_right == 0)
			{
				retVal = removeWithChild(elem, p_node, decreaseHeight);
			}
			else if (p_node->p_left != 0 && p_node->p_right != 0)
			{
				retVal = removeWithChildren(elem, p_node, decreaseHeight);

				if (decreaseHeight == true)
				{
					p_node->heightL--;
					if (p_node->heightL > p_node->heightR)
					{
						decreaseHeight = true;
					}
					else
					{
						decreaseHeight = false;
					}
				}
			}
			//cout << "\n\n";
			//printInTreeForm();
			//decreaseHeight = true;
		}
		else if (elem < p_node->data)
		{
			if (p_node->p_left == 0)
			{
				TreeExceptions ex;
				throw ex;
			}
			removeHelper(elem, p_node->p_left, decreaseHeight);
			if (decreaseHeight == true)
			{
				p_node->heightL--;
				if (p_node->heightL > p_node->heightR)
				{
					decreaseHeight = true;
				}
				else
				{
					decreaseHeight = false;
				}
			}
			//cout << "\n\n";
			//	printInTreeForm();
			if (!(p_node->heightL - p_node->heightR <= 1 && p_node->heightL - p_node->heightR >= -1))
			{
				balanceRight(p_node);
				if (!(p_node->heightL - p_node->heightR <= 1 && p_node->heightL - p_node->heightR >= -1))
				{
					balanceRight(p_node->p_left);

					balanceLeft(p_node);
				}
			}


		}
		else if (elem > p_node->data)
		{
			if (p_node->p_right == 0)
			{
				TreeExceptions ex;
				throw ex;
			}
			removeHelper(elem, p_node->p_right, decreaseHeight);
			if (decreaseHeight == true)
			{
				p_node->heightR--;
				if (p_node->heightR > p_node->heightL)
				{
					decreaseHeight = true;
				}
				else
				{
					decreaseHeight = false;
				}
			}
			//cout << "\n\n";
			//printInTreeForm();
			if (!(p_node->heightL - p_node->heightR <= 1 && p_node->heightL - p_node->heightR >= -1))
			{
				balanceLeft(p_node);
				if (!(p_node->heightL - p_node->heightR <= 1 && p_node->heightL - p_node->heightR >= -1))
				{
					balanceLeft(p_node->p_right);

					balanceRight(p_node);
				}
			}


		}
		return retVal;
	}
}
//--
template <class T>
bool BalancedTree<T>::removeLeaf(const T& elem, Node*& p_node, bool& decreaseHeight)
{
	decreaseHeight = true;
	delete p_node;
	p_node = 0;

	return true;
}
//--
template <class T>
bool BalancedTree<T>::removeWithChild(const T& elem, Node*& p_node, bool& decreaseHeight)
{
	decreaseHeight = true;
	Node* p_temp = p_node;
	if (p_node->p_left == 0)
	{
		p_node = p_node->p_right;
		delete p_temp;
		p_temp = 0;
	}
	else if (p_node->p_right == 0)
	{
		p_node = p_node->p_left;
		delete p_temp;
		p_temp = 0;
	}
	return true;
}
//--
template <class T>
bool BalancedTree<T>::removeWithChildren(const T& elem, Node*& p_node, bool& decreaseHeight)
{

	Node* p_temp = p_node->p_left;

	while (p_temp->p_right != 0)
	{
		p_temp = p_temp->p_right;
	}
	p_node->data = p_temp->data;

	removeHelper(p_node->data, p_node->p_left, decreaseHeight);

	return true;
}
//--
template <class T>
int BalancedTree<T>::length()
{
	return numElements;
}
//--
template <class T>
void BalancedTree<T>::printInOrder()
{
	printInOrderHelper(p_root);
}
//--
template <class T>
void BalancedTree<T>::printInOrderHelper(Node* p_node)
{
	if (p_node != 0)
	{
		printInOrderHelper(p_node->p_left);
		cout << p_node->data << endl;
		printInOrderHelper(p_node->p_right);
	}
}
//--
template <class T>
void BalancedTree<T>::printPreorder()
{
	printPreorderHelper(p_root);
}
//--
template <class T>
void BalancedTree<T>::printPreorderHelper(Node* p_node)
{
	if (p_node != 0)
	{
		cout << p_node->data << endl;
		printPreorderHelper(p_node->p_left);
		printPreorderHelper(p_node->p_right);
	}
}
//--
template <class T>
void BalancedTree<T>::printPostOrder()
{
	printPostOrderHelper(p_root);
}
//--
template <class T>
void BalancedTree<T>::printPostOrderHelper(Node* p_node)
{
	if (p_node != 0)
	{
		printPostOrderHelper(p_node->p_left);
		printPostOrderHelper(p_node->p_right);
		cout << p_node->data << endl;
	}
}
//--
template <class T>
T BalancedTree<T>::findMax()
{
	return findMaxHelper(p_root);
}
//--
template <class T>
T BalancedTree<T>::findMaxHelper(Node* p_node)
{
	if (p_node->p_right == 0)
	{
		return p_node->data;
	}
	else
	{
		findMaxHelper(p_node->p_right);
	}
}
//--
template <class T>
T BalancedTree<T>::findMin()
{
	return findMinHelper(p_root);
}
//--
template <class T>
T BalancedTree<T>::findMinHelper(Node* p_node)
{
	if (p_node->p_left == 0)
	{
		return p_node->data;
	}
	else
	{
		findMinHelper(p_node->p_left);
	}
}
//--
template <class T>
int BalancedTree<T>::heightOfTree()
{
	return heightOfTreeHelper(p_root) + 1;
}
//--
template <class T>
int BalancedTree<T>::heightOfTreeHelper(Node* p_node)
{
	if (p_root->heightL >= p_root->heightR)
	{
		return p_root->heightL;
	}
	else
	{
		return p_root->heightR;
	}
}
//--
template <class T>
int BalancedTree<T>::setHeights(Node* p_node)
{
	if (p_node != 0)
	{
		int leftHeight;
		int rightHeight;

		leftHeight = setHeights(p_node->p_left);
		p_node->heightL = leftHeight + 1;

		rightHeight = setHeights(p_node->p_right);
		p_node->heightR = rightHeight + 1;

		if (p_node->heightL >= p_node->heightR)
		{
			return p_node->heightL;
		}
		else
		{
			return p_node->heightR;
		}
	}
	else
	{
		return -1;
	}
}
//--
template <class T>
double BalancedTree<T>::percentLeaf()
{
	numLeaves = 0;
	percentLeaves = 0;
	return percentLeafHelper(p_root);
}
//--
template <class T>
double BalancedTree<T>::percentLeafHelper(Node* p_node)
{
	if (p_node != 0)
	{
		percentLeafHelper(p_node->p_left);
		if (p_node->p_left == 0 && p_node->p_right == 0)
		{
			numLeaves++;
		}
		percentLeafHelper(p_node->p_right);
	}
	percentLeaves = numLeaves / double(numElements) * 100.0;
	return percentLeaves;
}
//--
template <class T>
double BalancedTree<T>::percentInterior()
{
	percentInteriorNodes = 0;
	return percentInteriorHelper(p_root);
}
//--
template <class T>
double BalancedTree<T>::percentInteriorHelper(Node* p_node)
{

	if (p_node != 0)
	{
		percentLeafHelper(p_node->p_left);
		if (p_node->p_left == 0 && p_node->p_right == 0)
		{
			numLeaves++;
		}
		percentInteriorHelper(p_node->p_right);
	}
	percentInteriorNodes = (numElements - numLeaves - 1) / double(numElements) * 100.0;
	return percentInteriorNodes;
}
//--
template <class T>
void BalancedTree<T>::balanceRight(Node*& p_node)
{
	Node* p_NewSubRoot = p_node->p_right;
	p_node->heightR = p_NewSubRoot->heightL;
	p_NewSubRoot->heightL++;

	Node* p_temp = p_NewSubRoot->p_left;
	p_NewSubRoot->p_left = p_node;
	p_node->p_right = p_temp;

	p_node = p_NewSubRoot;
}
//--
template <class T>
void BalancedTree<T>::balanceLeft(Node*& p_node)
{
	Node* p_NewSubRoot = p_node->p_left;
	p_node->heightL = p_NewSubRoot->heightR;
	p_NewSubRoot->heightR++;

	Node* p_temp = p_NewSubRoot->p_right;
	p_NewSubRoot->p_right = p_node;
	p_node->p_left = p_temp;

	p_node = p_NewSubRoot;
}
//--
template <class T>
void BalancedTree<T>::printInTreeForm()
{
	SafeArray<int> powers;
	for (int i = 0; i < heightOfTree(); i++)
	{
		powers.push_back(pow(2, i));
	}
	SafeArray<Node*> tree;
	for (int i = 0; i < pow(2.0, double(heightOfTree())); i++)
	{
		tree.push_back(NULL);
	}
	printHelper(0, p_root, tree);

	int level = 0;
	for (int i = 0; i < pow(2.0, double(heightOfTree())) - 1; i++)
	{

		if (tree[i] != 0)
		{
			for (int j = 0; j < powers.size(); j++)
			{
				//int num = powers[j];
				if (i == powers[j] - 1)
				{
					cout << endl;
					for (int k = 0; k < pow(2, (heightOfTree() - level)); k++)
					{
						cout << " ";

					}
					level++;
				}
			}
			cout << "(" << tree[i]->heightL << ")" << tree[i]->data << "(" << tree[i]->heightR << ")";
			for (int j = 0; j < pow(2, heightOfTree() - level + 2) - 2; j++)
			{
				cout << " ";
			}
		}
		else
		{
			for (int j = 0; j < powers.size(); j++)
			{
				//int num = powers[j];
				if (i == powers[j] - 1)
				{
					cout << endl;
					for (int k = 0; k < pow(2, (heightOfTree() - level)); k++)
					{
						cout << " ";

					}
					level++;
				}
			}
			cout << "--";
			for (int j = 0; j < pow(2, heightOfTree() - level + 1); j++)
			{
				cout << " ";
			}
		}
	}
}
//--
template <class T>
void BalancedTree<T>::printHelper(int index, Node* p_node, SafeArray<Node*>& tree)
{
	if (p_node != 0)
	{
		tree[index] = p_node;
		printHelper(index * 2 + 1, p_node->p_left, tree);
		printHelper(index * 2 + 2, p_node->p_right, tree);
	}
}
//--