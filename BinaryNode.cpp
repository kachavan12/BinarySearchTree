
#include "BinaryNode.h"
#include <cstddef>

template<class ItemType>
BinaryNode<ItemType>::BinaryNode(): item(nullptr), leftChildPtr(nullptr), rightChildPtr(nullptr)
{ }

template<class ItemType>
BinaryNode<ItemType>::BinaryNode(const ItemType& anItem): item(anItem), leftChildPtr(nullptr), rightChildPtr(nullptr)
{ }

template<class ItemType>
BinaryNode<ItemType>::BinaryNode(const ItemType& anItem,shared_ptr<BinaryNode<ItemType>> leftPtr,shared_ptr<BinaryNode<ItemType>> rightPtr) : item(anItem), leftChildPtr(leftPtr), rightChildPtr(rightPtr)
{ }

template<class ItemType>
void BinaryNode<ItemType>::setItem(const ItemType& anItem)
{
   item = anItem;
}

template<class ItemType>
ItemType BinaryNode<ItemType>::getItem() const 
{
   return item;
}

template<class ItemType>
bool BinaryNode<ItemType>::isLeaf() const
{
   return ((leftChildPtr == nullptr) && (rightChildPtr == nullptr));
}

template<class ItemType>
void BinaryNode<ItemType>::setLeftChildPtr( shared_ptr<BinaryNode<ItemType>> leftPtr)
{
   leftChildPtr = leftPtr;
}

template<class ItemType>
void BinaryNode<ItemType>::setRightChildPtr( shared_ptr<BinaryNode<ItemType>> rightPtr)
{
   rightChildPtr = rightPtr;
}

template<class ItemType>
shared_ptr<BinaryNode<ItemType>> BinaryNode<ItemType>::getLeftChildPtr() const
{
   return leftChildPtr;
}

template<class ItemType>
shared_ptr<BinaryNode<ItemType>> BinaryNode<ItemType>::getRightChildPtr() const
{
   return rightChildPtr;
}

