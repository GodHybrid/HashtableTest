#pragma once

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <string>
#include <algorithm>
#include <vector>

using namespace std;

class someObject
{
public:
	string line;
	int number;

	someObject()
	{
		line = "0";
		number = 0;
	}

	someObject(string newLine, int newNumber)
	{
		line = newLine;
		number = newNumber;
	}

	~someObject()
	{
	}
};

//Без шаблонов, не работает, чисто ради образца кода
//ЗАБРОШЕНО
class HashTable
{
	vector<someObject>* table[100];
	int hashSize;

public:
	HashTable()
	{
		hashSize = 100;
		*table = new vector<someObject>[hashSize];
	}

	HashTable(int newSize)
	{
		hashSize = newSize;
		*table = new vector<someObject>[hashSize];
	}

	~HashTable()
	{
		delete[] table;
	}

	int get_size()
	{
		return hashSize;
	}

	void set_size(int size)
	{
		hashSize = size;
	}

	vector<someObject>* get_element(int index)
	{
		return table[index];
	}

	int hash(string line)
	{
		int hashsum, i;
		for (hashsum = i = 0; line[i]; i++)	hashsum = (hashsum * 47) ^ line[i];
		return abs(hashsum) % hashSize;
	}

	void push(string newLine, int newNum)
	{
		int hashNumber = hash(newLine);
		someObject* newElem = new someObject(newLine, newNum);
		vector<someObject>* place = table[hashNumber];
		(*table)[hashNumber].push_back(*newElem);
	}

	void pop(string lineToGo)
	{
		int hashNumber = hash(lineToGo);
		vector<someObject>* check = table[hashNumber];
		if (!check)
		{
			cout << "Line " << lineToGo << " doesn't exist." << endl;
			return;
		}
		for (unsigned int i = 0; i < check->size(); i++)
		{
			if ((*check)[i].line == lineToGo)
			{
				cout << "Removed " << lineToGo << " -- " << (*check)[i].number << endl;
				check->erase(check->begin() + i);
				return;
			}
		}
		cout << "Line " << lineToGo << " doesn't exist." << endl;
	}

	someObject* find(string searchLine)
	{
		int hashNumber = hash(searchLine);
		vector<someObject>* result = table[hashNumber];
		if (result->size() == 0)
		{
			cout << "Line " << searchLine << " doesn't exist." << endl;
			return NULL;
		}
		for (unsigned int i = 0; i < result->size(); i++)
		{
			if ((*result)[i].line == searchLine)
			{
				return &(result->at(i));
			}
		}
		return NULL;
	}
};

//Шаблонный, работает
template <typename InValue>
class TNode
{
public:
	bool init = false;
	int key = 0;
	int index = -1;
	std::vector<InValue> values;
	TNode* next;
	TNode(int idx)
	{
		index = idx;
	}

	void InitKey(int inKey)
	{
		if (!init)
		{
			key = inKey;
			init = !init;
		}
	}

	void push(InValue val)
	{
		values.push_back(val);
	}

	friend bool operator==(TNode& const a, TNode& const b)
	{
		if(a.key != b.key) return false;
		if (a.values.size() != b.values.size()) return false;
		for (int i = 0; i < a.values.size(); i++)
		{
			if (a.values[i] != b.values[i])
			{
				return false;
			}
		}
		return true;
	}
};

template <typename InKey, typename InValue>
class OwnHashList
{
	typedef void(*HashFuncPtr)(InValue key, int& res);

	HashFuncPtr funcPtr;
	TNode<InValue>* nodes = NULL;
	TNode<InValue>* lastNode = NULL;
	const int ReAllocSize = 10;

private:
	void AddNewNodes(long numOfNodes)
	{
		TNode<InValue>* prevNode = lastNode;
		while (numOfNodes-- > 0)
		{
			TNode<InValue>* nextNode = new TNode<InValue>(lastNode->index + 1);
			lastNode = nextNode;
			prevNode->next = nextNode;
			prevNode = prevNode->next;
		}
	}

	void DeAlloc()
	{
		TNode<InValue>* prevNode = nodes;
		TNode<InValue>* nextNode = nodes->next;
		delete prevNode;
		while (nextNode->next != NULL)
		{
			prevNode = nextNode;
			nextNode = nextNode->next;
			delete prevNode;
		}

		delete nextNode;
	}

public:
	OwnHashList(long initialSize, HashFuncPtr hashFunc)
	{
		if (initialSize <= 0)
		{
			throw std::exception("Param initialSize have to be > 0");
		}

		if (nodes != NULL)
		{
			DeAlloc();
		}

		initialSize = initialSize - 1;
		TNode<InValue>* start = new TNode<InValue>(1);
		lastNode = start;
		AddNewNodes(initialSize);
		nodes = start;

		funcPtr = hashFunc;
	}

	~OwnHashList()
	{
		DeAlloc();
	}

	bool push(InKey inKey, InValue inVal)
	{
		int hash = 0;
		funcPtr(inKey, hash);

		TNode<InValue>* nextNode = nodes;
		TNode<InValue>* lastUnused = NULL;
		while (nextNode->next != NULL)
		{
			if ((!nextNode->init) && (lastUnused == NULL))
			{
				lastUnused = nextNode;
			}

			if (nextNode->key == hash)
			{
				nextNode->push(inVal);
				return true;
			}

			nextNode = nextNode->next;
		}

		if (lastUnused == NULL)
		{
			lastNode->next = new TNode<InValue>(lastNode->index + 1);
			if (!lastNode->init)
			{
				lastNode->InitKey(hash);
				lastNode->push(inVal);
				lastNode = lastNode->next;
			}
			else
			{
				lastNode = lastNode->next;
				lastNode->InitKey(hash);
				lastNode->push(inVal);
			}
			AddNewNodes(ReAllocSize);
		}
		else
		{
			lastUnused->InitKey(hash);
			lastUnused->push(inVal);
		}
		return true;
	}

	bool pop(InKey inKey, InValue inVal)
	{
		int hash = 0;
		funcPtr(inKey, hash);

		TNode<InValue>* currNode = nodes;
		TNode<InValue>* prevNode = NULL;
		while (currNode->next != NULL)
		{
			if (currNode->key == hash)
			{
				for (int i = 0; i < currNode->values.size(); i++)
				{
					if (currNode->values[i] == inVal)
					{
						//if (prevNode != NULL) prevNode->next = currNode->next;
						currNode->values.erase(currNode->values.begin() + i);
						if (currNode->values.size() == 0)
						{
							currNode->init = false;
							delete currNode;
						}
						return true;
					}
				}
				return false;
			}
			prevNode = currNode;
			currNode = currNode->next;
		}
		return false;
	}

	TNode<InValue> find(InKey key)
	{
		int hash = 0;
		funcPtr(key, hash);

		TNode<InValue>* currNode = nodes;
		while (currNode->next != NULL)
		{
			if (currNode->key == hash)
			{
				return currNode;
			}
			currNode = currNode->next;
		}
		return NULL;
	}

	/*TNode<InValue> find(InValue value)
	{
		TNode<InValue>* currNode = nodes;
		while (currNode->next != NULL)
		{
			for (int i = 0; i < currNode->values->size(); i++)
			{
				if (currNode->values[i] == value)
				{
					return currNode;
				}
			}
			currNode = currNode->next;
		}
		return NULL;
	}*/
};