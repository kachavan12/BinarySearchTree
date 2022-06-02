/** @file BinaryNodeTree.cpp */

#include "BinaryNodeTree.h" 
#include "BinaryNode.h" 
#include <iostream>

//Protected Utility Methods Section


template<class ItemType>
int BinaryNodeTree<ItemType>::getHeightHelper( shared_ptr<BinaryNode<ItemType>> subTreePtr) const
{
   if (subTreePtr == nullptr)
      return 0;
   else
      return 1 +  max(getHeightHelper(subTreePtr->getLeftChildPtr()),
                     getHeightHelper(subTreePtr->getRightChildPtr()));
}

template<class ItemType>
int BinaryNodeTree<ItemType>::getNumberOfNodesHelper( shared_ptr<BinaryNode<ItemType>> subTreePtr) const
{
   if (subTreePtr == nullptr)
      return 0;
   else
      return 1 + getNumberOfNodesHelper(subTreePtr->getLeftChildPtr()) 
               + getNumberOfNodesHelper(subTreePtr->getRightChildPtr());
}

template<class ItemType>
shared_ptr<BinaryNode<ItemType>> BinaryNodeTree<ItemType>::balancedAdd(shared_ptr<BinaryNode<ItemType>> subTreePtr,shared_ptr<BinaryNode<ItemType>> newNodePtr)
{
   if (subTreePtr == nullptr)
      return newNodePtr;
   else
   {
      auto leftPtr = subTreePtr->getLeftChildPtr();
      auto rightPtr = subTreePtr->getRightChildPtr();

      if (getHeightHelper(leftPtr) > getHeightHelper(rightPtr))
      {
         rightPtr = balancedAdd(rightPtr, newNodePtr);
         subTreePtr->setRightChildPtr(rightPtr);
      }
      else
      {
         leftPtr = balancedAdd(leftPtr, newNodePtr);
         subTreePtr->setLeftChildPtr(leftPtr);
      } 
      return subTreePtr;
   }  
} 

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
         return nullptr;
      }  
   }   
} 


template<class ItemType>
 shared_ptr<BinaryNode<ItemType>> BinaryNodeTree<ItemType>::removeValue( shared_ptr<BinaryNode<ItemType>> subTreePtr, const ItemType target, bool& success)
{
   if(subTreePtr == nullptr)
      return subTreePtr;

   if (subTreePtr->getItem() == target) 
   {
      subTreePtr = moveValuesUpTree(subTreePtr);
      success = true;
      return subTreePtr;
   }
   else 
   {
      auto targetNodePtr = removeValue(subTreePtr->getLeftChildPtr(), target, success);
      subTreePtr->setLeftChildPtr(targetNodePtr);
      if (!success) 
      {
         targetNodePtr = removeValue(subTreePtr->getRightChildPtr(), target, success);
         subTreePtr->setRightChildPtr(targetNodePtr);
      } 
      
      return subTreePtr;
   }  
}  

template<class ItemType>
shared_ptr<BinaryNode<ItemType>> BinaryNodeTree<ItemType>::findNode( shared_ptr<BinaryNode<ItemType>> treePtr, const ItemType& target, bool& success) const
{
   if (treePtr == nullptr) 
      return treePtr;
   
   if (treePtr->getItem() == target) \
   {
      success = true;
      return treePtr;
   }
   else 
   {
      shared_ptr<BinaryNode<ItemType>> targetNodePtr = findNode(treePtr->getLeftChildPtr(), target, success);
      if (!success)
      {
         targetNodePtr = findNode(treePtr->getRightChildPtr(), target, success);
      }  
      
      return targetNodePtr;
   }
}  

template<class ItemType>
 shared_ptr<BinaryNode<ItemType>> BinaryNodeTree<ItemType>::copyTree(const  shared_ptr<BinaryNode<ItemType>> oldTreeRootPtr) const
{
    shared_ptr<BinaryNode<ItemType>> newTreePtr;
   
   
   if (oldTreeRootPtr != nullptr)
   {
      newTreePtr =  make_shared<BinaryNode<ItemType>>(oldTreeRootPtr->getItem(), nullptr, nullptr);
	   newTreePtr->setLeftChildPtr(copyTree(oldTreeRootPtr->getLeftChildPtr()));
      newTreePtr->setRightChildPtr(copyTree(oldTreeRootPtr->getRightChildPtr()));
   }
   
   return newTreePtr;
}

template<class ItemType>
void BinaryNodeTree<ItemType>::destroyTree( shared_ptr<BinaryNode<ItemType>> subTreePtr)
{
   if (subTreePtr != nullptr)
   {
      destroyTree(subTreePtr->getLeftChildPtr());
      destroyTree(subTreePtr->getRightChildPtr());
      subTreePtr.reset();
   }  
}  


template<class ItemType> void BinaryNodeTree<ItemType>::preorder(void visit(ItemType&), shared_ptr<BinaryNode<ItemType>>treePtr) const
{
   if (treePtr != nullptr)
   {
      ItemType theItem = treePtr->getItem();
      visit(theItem);
      //visit(treePtr->getItem());
      preorder(visit, treePtr->getLeftChildPtr());
      preorder(visit, treePtr->getRightChildPtr());
   };
}

template<class ItemType>
void BinaryNodeTree<ItemType>::inorder(void visit(ItemType&),  shared_ptr<BinaryNode<ItemType>> treePtr) const
{
   if (treePtr != nullptr)
   {
      inorder(visit, treePtr->getLeftChildPtr());
      ItemType theItem = treePtr->getItem();
      visit(theItem);
      inorder(visit, treePtr->getRightChildPtr());
   }  
}  

template<class ItemType>
void BinaryNodeTree<ItemType>::postorder(void visit(ItemType&),  shared_ptr<BinaryNode<ItemType>> treePtr) const
{
   if (treePtr != nullptr)
   {
      postorder(visit, treePtr->getLeftChildPtr());
      postorder(visit, treePtr->getRightChildPtr());
      ItemType theItem = treePtr->getItem();
      visit(theItem);
   }
} 

//Constructor and Destructor Section

template<class ItemType>
BinaryNodeTree<ItemType>::BinaryNodeTree()
{  }

template<class ItemType>
BinaryNodeTree<ItemType>::BinaryNodeTree(const ItemType& rootItem)
   :rootPtr( make_shared<BinaryNode<ItemType>>(rootItem, nullptr, nullptr))
{  } 

template<class ItemType>
BinaryNodeTree<ItemType>::BinaryNodeTree(const ItemType& rootItem,const  shared_ptr<BinaryNodeTree<ItemType>> leftTreePtr, const  shared_ptr<BinaryNodeTree<ItemType>> rightTreePtr):rootPtr( make_shared<BinaryNode<ItemType>>(rootItem,copyTree(leftTreePtr->rootPtr),copyTree(rightTreePtr->rootPtr)))
{   }

template<class ItemType>
BinaryNodeTree<ItemType>::BinaryNodeTree(const BinaryNodeTree<ItemType>& treePtr)
{
   rootPtr = copyTree(treePtr.rootPtr);
}

template<class ItemType>
BinaryNodeTree<ItemType>::~BinaryNodeTree()
{
   destroyTree(rootPtr);
}


//Public BinaryTreeInterface Methods Section

template<class ItemType>
bool BinaryNodeTree<ItemType>::isEmpty() const
{
   return rootPtr == nullptr;
}  // end isEmpty

template<class ItemType>
int BinaryNodeTree<ItemType>::getHeight() const
{
   return getHeightHelper(rootPtr);
}

template<class ItemType>
int BinaryNodeTree<ItemType>::getNumberOfNodes() const
{
   return getNumberOfNodesHelper(rootPtr);
}

template<class ItemType>
void BinaryNodeTree<ItemType>::clear()
{
   destroyTree(rootPtr);
   rootPtr.reset();
}

template<class ItemType>
ItemType BinaryNodeTree<ItemType>::getRootData() const throw(PrecondViolatedExcep)
{
   if (isEmpty())
      throw PrecondViolatedExcep("getRootData() called with empty tree."); 
      
   return rootPtr->getItem();
}

template<class ItemType>
void BinaryNodeTree<ItemType>::setRootData(const ItemType& newItem)
{
   if (isEmpty())
      rootPtr =  make_shared<BinaryNode<ItemType>>(newItem, nullptr, nullptr);
   else
      rootPtr->setItem(newItem);
}

template<class ItemType>
bool BinaryNodeTree<ItemType>::add(const ItemType& newData)
{
   auto newNodePtr =  make_shared<BinaryNode<ItemType>>(newData);
   rootPtr = balancedAdd(rootPtr, newNodePtr);
   return true;
}

template<class ItemType>
bool BinaryNodeTree<ItemType>::remove(const ItemType& target)
{
   bool isSuccessful = false;
   rootPtr = removeValue(rootPtr, target, isSuccessful);
   return isSuccessful; 
}

template<class ItemType>
ItemType BinaryNodeTree<ItemType>::getEntry(const ItemType& anEntry) const throw(NotFoundException)
{
   bool isSuccessful = false;
   auto binaryNodePtr = findNode(rootPtr, anEntry, isSuccessful);
   
   if (isSuccessful)
      return binaryNodePtr->getItem(); 
   else 
      throw NotFoundException("Entry not found in tree!");
}

template<class ItemType>
bool BinaryNodeTree<ItemType>:: contains(const ItemType& anEntry) const
{
   bool isSuccessful = false;
   findNode(rootPtr, anEntry, isSuccessful);
   return isSuccessful;   
}


//Public Traversals Section


template<class ItemType>
void BinaryNodeTree<ItemType>::preorderTraverse(void visit(ItemType&)) const
{
   preorder(visit, rootPtr);
}  // end preorderTraverse

template<class ItemType>
void BinaryNodeTree<ItemType>::inorderTraverse(void visit(ItemType&)) const
{
   inorder(visit, rootPtr);
}

template<class ItemType>
void BinaryNodeTree<ItemType>::postorderTraverse(void visit(ItemType&)) const
{
   postorder(visit, rootPtr);
}


//Overloaded Operator 


template<class ItemType>
BinaryNodeTree<ItemType>& BinaryNodeTree<ItemType>::operator=(
                                                const BinaryNodeTree<ItemType>& rightHandSide)
{
   if (!isEmpty())
      clear();
   this = copyTree(&rightHandSide);
   return *this;
} 


