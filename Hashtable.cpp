// Hashtable.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include "Hashtable.h"

void MyHashFunc(int key, int& res)
{
	res = key % 999;
}

void MyHashFunc2(std::string var, int& res)
{
	for (int i = 0; var[i]; i++)res = (res * 47) ^ var[i];
	res = abs(res) % 999;
}

void PrintTable(HashTable& hasher)
{
	for (int i = 0; i < hasher.get_size(); i++)
	{
		vector<someObject>* tmp = hasher.get_element(i);
		if (tmp->size() == 0)
		{
			cout << i << ": " << "Empty" << endl;
		}
		else
		{
			for (unsigned int j = 0; j < tmp->size(); j++)
			{
				cout << i << ": " << tmp->at(j).line << " -- " << tmp->at(j).number << endl;
			}
		}
	}
}

int main()
{
	//int size, x = 0;
	//string tmp;
	//int tmp2;
	//cout << "Enter table's size: ";
	//cin >> size;

	//HashTable* hash = new HashTable(size);

	//while (x != 5)
	//{
	//	//system("CLS");
	//	cout << "\n\n1. push new element\n2. Search...\n3. Remove an element\n4. Print out the table.\n5. Quit\n\n>>> ";
	//	cin >> x;
	//	switch (x)
	//	{
	//	case 1:
	//		cin >> tmp >> tmp2;
	//		hash->push(tmp, tmp2);
	//		cout << "Successfully added the new element.";
	//		break;
	//	case 2:
	//	{
	//		someObject* tmp = hash->find("xc");
	//		if (tmp != NULL)cout << "Found: " << tmp->line << " - " << tmp->number << endl;
	//		else cout << "Found nothing." << endl;
	//		break;
	//	}
	//	case 3:
	//		hash->pop("xc");
	//		break;
	//	case 4:
	//		cout << "Printing the table...\n//////////////////////////" << endl;
	//		PrintTable(*hash);
	//		break;
	//	case 5:
	//		cout << "Farewell";
	//		break;
	//	default:
	//		break;
	//	}
	//}

	OwnHashList<std::string, std::string> hash2(3, &MyHashFunc2);
	unsigned long x = 9999999;
	while (x-- > 0)
	{
		hash2.push(std::string("abc"), std::string("qqq"));
		hash2.push(std::string("abc"), std::string("eee2"));
		hash2.push(std::string("xyfghgzff"), std::string("kgsfghdh"));
		hash2.pop(std::string("abc"), std::string("qqq"));
		hash2.pop(std::string("abc"), std::string("eee2"));
		hash2.pop(std::string("xyfghgzff"), std::string("kgsfghdh"));
	}
	return 0;
}
