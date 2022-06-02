//  Created by Frank M. Carrano and Timothy M. Henry.
//  Copyright (c) 2017 Pearson Education, Hoboken, New Jersey.

/** ADT binary tree: Link-based implementation.
 Listing 16-3.
 @file BinaryNodeTree.h */
 
#ifndef BINARY_NODE_TREE_
#define BINARY_NODE_TREE_

#include <memory>
#include "BinaryTreeInterface.h"
#include "BinaryNode.h"
#include "PrecondViolatedExcep.h"
#include "NotFoundException.h"
using namespace std;
template<class ItemType>
class BinaryNodeTree : public BinaryTreeInterface<ItemType>
{
protected:
   shared_ptr<BinaryNode<ItemType>> rootPtr;
   
protected:
   //------------------------------------------------------------
   // Protected Utility Methods Section:
   // Recursive helper methods for the public methods.
   //------------------------------------------------------------
   
   int getHeightHelper( shared_ptr<BinaryNode<ItemType>> subTreePtr) const;
   int getNumberOfNodesHelper(shared_ptr<BinaryNode<ItemType>> subTreePtr) const;
   
   // Recursively adds a new node to the tree in a left/right fashion to
   // keep the tree balanced.
   shared_ptr<BinaryNode<ItemType>> balancedAdd( shared_ptr<BinaryNode<ItemType>> subTreePtr,shared_ptr<BinaryNode<ItemType>> newNodePtr);
   
   // Copies values up the tree to overwrite value in current node until
   // a leaf is reached; the leaf is then removed, since its value is
   // stored in the parent.
   shared_ptr<BinaryNode<ItemType>> moveValuesUpTree(shared_ptr<BinaryNode<ItemType>> subTreePtr);
   
   // Removes the target value from the tree by calling moveValuesUpTree
   // to overwrite value with value from child.
   virtual shared_ptr<BinaryNode<ItemType>>
   removeValue(shared_ptr<BinaryNode<ItemType>> subTreePtr, const ItemType target, bool& success);
   
   // Recursively searches for target value in the tree by using a
   // preorder traversal.
   shared_ptr<BinaryNode<ItemType>> findNode(shared_ptr<BinaryNode<ItemType>> treePtr,const ItemType& target,bool& success) const;
   
   // Copies the tree rooted at treePtr and returns a pointer to
   // the copy.
   shared_ptr<BinaryNode<ItemType>> copyTree(const shared_ptr<BinaryNode<ItemType>> oldTreeRootPtr) const;
   
   // Recursively deletes all nodes from the tree.
   void destroyTree(shared_ptr<BinaryNode<ItemType>> subTreePtr);
   
   // Recursive traversal helper methods:
   void preorder(void visit(ItemType&), shared_ptr<BinaryNode<ItemType>> treePtr) const;
   void inorder(void visit(ItemType&), shared_ptr<BinaryNode<ItemType>> treePtr) const;
   void postorder(void visit(ItemType&), shared_ptr<BinaryNode<ItemType>> treePtr) const;
   
public:
   //------------------------------------------------------------
   // Constructor and Destructor Section.
   //------------------------------------------------------------
   BinaryNodeTree();
   BinaryNodeTree(const ItemType& rootItem);
   BinaryNodeTree(const ItemType& rootItem,
                   const  shared_ptr<BinaryNodeTree<ItemType>> leftTreePtr,
                   const  shared_ptr<BinaryNodeTree<ItemType>> rightTreePtr);
   BinaryNodeTree(const BinaryNodeTree<ItemType>& tree);
   virtual ~BinaryNodeTree();
   
   //------------------------------------------------------------
   // Public BinaryTreeInterface Methods Section.
   //------------------------------------------------------------
   bool isEmpty() const;
   int getHeight() const;
   int getNumberOfNodes() const;
   ItemType getRootData() const;
   void setRootData(const ItemType& newData);
   bool add(const ItemType& newData); // Adds a node
   bool remove(const ItemType& data); // Removes a node
   void clear();
   ItemType getEntry(const ItemType& anEntry) const;
   bool contains(const ItemType& anEntry) const;
   
   //------------------------------------------------------------
   // Public Traversals Section.
   //------------------------------------------------------------
   void preorderTraverse(void visit(ItemType&)) const;
   void inorderTraverse(void visit(ItemType&)) const;
   void postorderTraverse(void visit(ItemType&)) const;
   
   //------------------------------------------------------------
   // Overloaded Operator Section.
   //------------------------------------------------------------
   BinaryNodeTree& operator=(const BinaryNodeTree& rightHandSide);
}; // end BinaryNodeTree


/** @file BinaryNodeTree.cpp */

//////////////////////////////////////////////////////////////
//      Protected Utility Methods Section
//////////////////////////////////////////////////////////////

template<class ItemType>
int BinaryNodeTree<ItemType>::getHeightHelper( shared_ptr<BinaryNode<ItemType>> subTreePtr) const
{
    if (subTreePtr == nullptr)
        return 0;
    else
        return 1 +  max(getHeightHelper(subTreePtr->getLeftChildPtr()),
                            getHeightHelper(subTreePtr->getRightChildPtr()));
}  // end getHeightHelper

template<class ItemType>
int BinaryNodeTree<ItemType>::getNumberOfNodesHelper( shared_ptr<BinaryNode<ItemType>> subTreePtr) const
{
    if (subTreePtr == nullptr)
        return 0;
    else
        return 1 + getNumberOfNodesHelper(subTreePtr->getLeftChildPtr())
        + getNumberOfNodesHelper(subTreePtr->getRightChildPtr());
}  // end getNumberOfNodesHelper

template<class ItemType>
 shared_ptr<BinaryNode<ItemType>> BinaryNodeTree<ItemType>::balancedAdd( shared_ptr<BinaryNode<ItemType>> subTreePtr,
                                                                             shared_ptr<BinaryNode<ItemType>> newNodePtr)
{
    if (subTreePtr == nullptr)
        return newNodePtr;
    else
    {
        auto leftPtr = subTreePtr->getLeftChildPtr();
        auto rightPtr = subTreePtr->getRightChildPtr();
        
        if (getHeightHelper(leftPtr) > getHeightHelper(rightPtr))
        {
            rightPtr = balancedAdd(rightPtr , newNodePtr);
            subTreePtr->setRightChildPtr(rightPtr );
        }
        else
        {
            leftPtr = balancedAdd(leftPtr, newNodePtr);
            subTreePtr->setLeftChildPtr(leftPtr);
        }  // end if
        
        return subTreePtr;
    }  // end if
}  // end balancedAdd

template<class ItemType>
 shared_ptr<BinaryNode<ItemType>> BinaryNodeTree<ItemType>::moveValuesUpTree( shared_ptr<BinaryNode<ItemType>> subTreePtr)
{
    auto  leftPtr = subTreePtr->getLeftChildPtr();
    auto  rightPtr = subTreePtr->getRightChildPtr();
    int leftHeight = getHeightHelper(leftPtr);
    int rightHeight = getHeightHelper(rightPtr);
    if (leftHeight > rightHeight)
    {
        subTreePtr->setItem(leftPtr->getItem());
        leftPtr = moveValuesUpTree(leftPtr);
        subTreePtr->setLeftChildPtr(leftPtr);
        return subTreePtr;
    }
    else
    {
        if (rightPtr != nullptr)
        {
            subTreePtr->setItem(rightPtr->getItem());
            rightPtr =moveValuesUpTree(rightPtr);
            subTreePtr->setRightChildPtr(rightPtr);
            return subTreePtr;
        }
        else
        {
            //this was a leaf!
            // value not important
            return nullptr;
        }  // end if
    }  // end if
}  // end moveValuesUpTree

/** Depth-first search of tree for item.
 @param subTreePtr  tree to search.
 @param target  target item to find.
 @param success  communicate to client we found it.
 @returns  A pointer to node containing the item. */
template<class ItemType>
 shared_ptr<BinaryNode<ItemType>> BinaryNodeTree<ItemType>::removeValue( shared_ptr<BinaryNode<ItemType>> subTreePtr,
                                                                            const ItemType target,
                                                                            bool& success)
{
    if(subTreePtr == nullptr) // not found here
        return subTreePtr;
    
    if (subTreePtr->getItem() == target) // found it
    {
        subTreePtr = moveValuesUpTree(subTreePtr);
        success = true;
        return subTreePtr;
    }
    else
    {
        auto targetNodePtr = removeValue(subTreePtr->getLeftChildPtr(), target, success);
        subTreePtr->setLeftChildPtr(targetNodePtr);
        if (!success) // no need to search right subTree
        {
            targetNodePtr = removeValue(subTreePtr->getRightChildPtr(), target, success);
            subTreePtr->setRightChildPtr(targetNodePtr);
        }  // end if
        
        return subTreePtr;
    }  // end if
}  // end removeValue

template<class ItemType>
 shared_ptr<BinaryNode<ItemType>> BinaryNodeTree<ItemType>::findNode( shared_ptr<BinaryNode<ItemType>> treePtr,
                                                                         const ItemType& target,
                                                                         bool& success) const
{
    if (treePtr == nullptr) // not found here
        return treePtr;
    
    if (treePtr->getItem() == target) // found it
    {
        success = true;
        return treePtr;
    }
    else
    {
         shared_ptr<BinaryNode<ItemType>> targetNodePtr = findNode(treePtr->getLeftChildPtr(), target, success);
        if (!success) // no need to search right subTree
        {
            targetNodePtr = findNode(treePtr->getRightChildPtr(), target, success);
        }  // end if
        
        return targetNodePtr;
    }  // end if
}  // end findNode

template<class ItemType>
 shared_ptr<BinaryNode<ItemType>> BinaryNodeTree<ItemType>::copyTree(const  shared_ptr<BinaryNode<ItemType>> oldTreeRootPtr) const
{
     shared_ptr<BinaryNode<ItemType>> newTreePtr;
    
    // Copy tree nodes during a preorder traversal
    if (oldTreeRootPtr != nullptr)
    {
        // Copy node
        newTreePtr =  make_shared<BinaryNode<ItemType>>(oldTreeRootPtr->getItem(), nullptr, nullptr);
        newTreePtr->setLeftChildPtr(copyTree(oldTreeRootPtr->getLeftChildPtr()));
        newTreePtr->setRightChildPtr(copyTree(oldTreeRootPtr->getRightChildPtr()));
    }  // end if
    
    return newTreePtr;
}  // end copyTree

template<class ItemType>
void BinaryNodeTree<ItemType>::destroyTree( shared_ptr<BinaryNode<ItemType>> subTreePtr)
{
    if (subTreePtr != nullptr)
    {
        destroyTree(subTreePtr->getLeftChildPtr());
        destroyTree(subTreePtr->getRightChildPtr());
        subTreePtr.reset(); // decrement reference count to node
    }  // end if
}  // end destroyTree

//////////////////////////////////////////////////////////////
//      Protected Tree Traversal Sub-Section
//////////////////////////////////////////////////////////////

template<class ItemType>
void BinaryNodeTree<ItemType>::preorder(void visit(ItemType&),  shared_ptr<BinaryNode<ItemType>> treePtr) const
{
    if (treePtr != nullptr)
    {
        ItemType theItem = treePtr->getItem();
        visit(theItem);
        //      visit(treePtr->getItem());
        preorder(visit, treePtr->getLeftChildPtr());
        preorder(visit, treePtr->getRightChildPtr());
    }  // end if
}  // end preorder

template<class ItemType>
void BinaryNodeTree<ItemType>::inorder(void visit(ItemType&),  shared_ptr<BinaryNode<ItemType>> treePtr) const
{
    if (treePtr != nullptr)
    {
        inorder(visit, treePtr->getLeftChildPtr());
        ItemType theItem = treePtr->getItem();
        visit(theItem);
        inorder(visit, treePtr->getRightChildPtr());
    }  // end if
}  // end inorder

template<class ItemType>
void BinaryNodeTree<ItemType>::postorder(void visit(ItemType&),  shared_ptr<BinaryNode<ItemType>> treePtr) const
{
    if (treePtr != nullptr)
    {
        postorder(visit, treePtr->getLeftChildPtr());
        postorder(visit, treePtr->getRightChildPtr());
        ItemType theItem = treePtr->getItem();
        visit(theItem);
    } // end if
}  // end postorder

//////////////////////////////////////////////////////////////
//      PUBLIC METHODS BEGIN HERE
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
//      Constructor and Destructor Section
//////////////////////////////////////////////////////////////

template<class ItemType>
BinaryNodeTree<ItemType>::BinaryNodeTree()
{  }  // end default constructor

template<class ItemType>
BinaryNodeTree<ItemType>::BinaryNodeTree(const ItemType& rootItem)
:rootPtr( make_shared<BinaryNode<ItemType>>(rootItem, nullptr, nullptr))
{  }  // end constructor

template<class ItemType>
BinaryNodeTree<ItemType>::BinaryNodeTree(const ItemType& rootItem,
                                         const  shared_ptr<BinaryNodeTree<ItemType>> leftTreePtr,
                                         const  shared_ptr<BinaryNodeTree<ItemType>> rightTreePtr)
:rootPtr( make_shared<BinaryNode<ItemType>>(rootItem,
                                                copyTree(leftTreePtr->rootPtr),
                                                copyTree(rightTreePtr->rootPtr)))
{   }  // end constructor

template<class ItemType>
BinaryNodeTree<ItemType>::BinaryNodeTree(const BinaryNodeTree<ItemType>& treePtr)
{
    rootPtr = copyTree(treePtr.rootPtr);
}  // end copy constructor

template<class ItemType>
BinaryNodeTree<ItemType>::~BinaryNodeTree()
{
    destroyTree(rootPtr);
}  // end destructor

//////////////////////////////////////////////////////////////
//      Public BinaryTreeInterface Methods Section
//////////////////////////////////////////////////////////////


template<class ItemType>
bool BinaryNodeTree<ItemType>::isEmpty() const
{
    return rootPtr == nullptr;
}  // end isEmpty

template<class ItemType>
int BinaryNodeTree<ItemType>::getHeight() const
{
    return getHeightHelper(rootPtr);
}  // end getHeight

template<class ItemType>
int BinaryNodeTree<ItemType>::getNumberOfNodes() const
{
    return getNumberOfNodesHelper(rootPtr);
}  // end getNumberOfNodes

template<class ItemType>
void BinaryNodeTree<ItemType>::clear()
{
    destroyTree(rootPtr);
    rootPtr.reset();
}  // end clear

template<class ItemType>
ItemType BinaryNodeTree<ItemType>::getRootData() const
{
    if (isEmpty())
        throw PrecondViolatedExcep("getRootData() called with empty tree.");
        
        return rootPtr->getItem();
        }  // end getRootData

template<class ItemType>
void BinaryNodeTree<ItemType>::setRootData(const ItemType& newItem)
{
    if (isEmpty())
        rootPtr =  make_shared<BinaryNode<ItemType>>(newItem, nullptr, nullptr);
    else
        rootPtr->setItem(newItem);
}  // end setRootData

template<class ItemType>
bool BinaryNodeTree<ItemType>::add(const ItemType& newData)
{
    auto newNodePtr =  make_shared<BinaryNode<ItemType>>(newData);
    rootPtr = balancedAdd(rootPtr, newNodePtr);
    return true;
}  // end add

template<class ItemType>
bool BinaryNodeTree<ItemType>::remove(const ItemType& target)
{
    bool isSuccessful = false;
    rootPtr = removeValue(rootPtr, target, isSuccessful);
    return isSuccessful;
}  // end remove

template<class ItemType>
ItemType BinaryNodeTree<ItemType>::getEntry(const ItemType& anEntry) const
{
    bool isSuccessful = false;
    auto binaryNodePtr = findNode(rootPtr, anEntry, isSuccessful);
    
    if (isSuccessful)
        return binaryNodePtr->getItem();
        else
            throw NotFoundException("Entry not found in tree!");
            }  // end getEntry

template<class ItemType>
bool BinaryNodeTree<ItemType>:: contains(const ItemType& anEntry) const
{
    bool isSuccessful = false;
    findNode(rootPtr, anEntry, isSuccessful);
    return isSuccessful;
}  // end contains

//////////////////////////////////////////////////////////////
//      Public Traversals Section
//////////////////////////////////////////////////////////////

template<class ItemType>
void BinaryNodeTree<ItemType>::preorderTraverse(void visit(ItemType&)) const
{
    preorder(visit, rootPtr);
}  // end preorderTraverse

template<class ItemType>
void BinaryNodeTree<ItemType>::inorderTraverse(void visit(ItemType&)) const
{
    inorder(visit, rootPtr);
}  // end inorderTraverse

template<class ItemType>
void BinaryNodeTree<ItemType>::postorderTraverse(void visit(ItemType&)) const
{
    postorder(visit, rootPtr);
}  // end postorderTraverse

//////////////////////////////////////////////////////////////
//      Overloaded Operator
//////////////////////////////////////////////////////////////

template<class ItemType>
BinaryNodeTree<ItemType>& BinaryNodeTree<ItemType>::operator=(
                                                              const BinaryNodeTree<ItemType>& rightHandSide)
{
    if (!isEmpty())
        clear();
    this = copyTree(&rightHandSide);
    return *this;
}  // end operator=



#endif
