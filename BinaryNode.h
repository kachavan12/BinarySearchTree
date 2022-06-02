//  Created by Frank M. Carrano and Timothy M. Henry.
//  Copyright (c) 2017 Pearson Education, Hoboken, New Jersey.

/** A class of nodes for a link-based binary tree.
 Listing 16-2.
 @file BinaryNode.h */
 
#ifndef BINARY_NODE_
#define BINARY_NODE_

#include <memory>
using namespace std;
template<class ItemType>
class BinaryNode
{   
private:
   ItemType item;           // Data portion
   shared_ptr<BinaryNode<ItemType>> leftChildPtr;   // Pointer to left child
   shared_ptr<BinaryNode<ItemType>> rightChildPtr;  // Pointer to right child

public:
   BinaryNode();
   BinaryNode(const ItemType& anItem);
   BinaryNode(const ItemType& anItem,
   shared_ptr<BinaryNode<ItemType>> leftPtr,
   shared_ptr<BinaryNode<ItemType>> rightPtr);

   void setItem(const ItemType& anItem);
   ItemType getItem() const;
   
   bool isLeaf() const;

   shared_ptr<BinaryNode<ItemType>> getLeftChildPtr() const;
   shared_ptr<BinaryNode<ItemType>> getRightChildPtr() const;
   
   void setLeftChildPtr(shared_ptr<BinaryNode<ItemType>> leftPtr);
   void setRightChildPtr(shared_ptr<BinaryNode<ItemType>> rightPtr);
}; // end BinaryNode

/** @file BinaryNode.cpp */


template<class ItemType>
BinaryNode<ItemType>::BinaryNode():item(nullptr), leftChildPtr(nullptr), rightChildPtr(nullptr)
{ }  // end default constructor

template<class ItemType>
BinaryNode<ItemType>::BinaryNode(const ItemType& anItem):item(anItem), leftChildPtr(nullptr), rightChildPtr(nullptr)
{ }  // end constructor

template<class ItemType>
BinaryNode<ItemType>::BinaryNode(const ItemType& anItem, shared_ptr<BinaryNode<ItemType>> leftPtr, shared_ptr<BinaryNode<ItemType>> rightPtr)
:item(anItem), leftChildPtr(leftPtr), rightChildPtr(rightPtr)
{ }  // end constructor

template<class ItemType>
void BinaryNode<ItemType>::setItem(const ItemType& anItem)
{
    item = anItem;
}  // end setItem

template<class ItemType>
ItemType BinaryNode<ItemType>::getItem() const
{
    return item;
}  // end getItem

template<class ItemType>
bool BinaryNode<ItemType>::isLeaf() const
{
    return ((leftChildPtr == nullptr) && (rightChildPtr == nullptr));
}

template<class ItemType>
void BinaryNode<ItemType>::setLeftChildPtr(shared_ptr<BinaryNode<ItemType>> leftPtr)
{
    leftChildPtr = leftPtr;
}  // end setLeftChildPtr

template<class ItemType>
void BinaryNode<ItemType>::setRightChildPtr(shared_ptr<BinaryNode<ItemType>> rightPtr)
{
    rightChildPtr = rightPtr;
}  // end setRightChildPtr

template<class ItemType>
 shared_ptr<BinaryNode<ItemType>> BinaryNode<ItemType>::getLeftChildPtr() const
{
    return leftChildPtr;
}  // end getLeftChildPtr

template<class ItemType>
shared_ptr<BinaryNode<ItemType>> BinaryNode<ItemType>::getRightChildPtr() const
{
    return rightChildPtr;
}  // end getRightChildPtr



#endif 
