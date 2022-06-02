//created by Karthik Chavan @ 7/26/2020
#include <iostream>
#include <iomanip>
#include <string>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "BinarySearchTree.h"

using namespace std;

vector<int> theTreeSorted;

//display funct
void display(int& anItem)
{
	cout << anItem << " ";
	theTreeSorted.push_back(anItem);
}

int main() {
	
   //random num genorator
	srand((unsigned)time(0));
	
	BinarySearchTree<int> theTree;
	
	int num = 0;
   cout << "Generate random numbers:" << endl;
	for (int i = 0; i < 22; i++)
	{
		num = rand() % 100 + 1;
      cout << num << " " << endl;;
		theTree.add(num);
	}
	cout << "Added the numbers to the tree" << endl;

   //remove the first number
	theTree.remove(num);
	cout << "Removed " << num << " from the tree" << endl;
	
   //display "new" BST
	cout << "The Binary Search Tree contains:" <<  endl;
	theTree.inorderTraverse(display);

	return 0;
}
