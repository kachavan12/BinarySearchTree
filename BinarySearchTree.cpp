#include "BinarySearchTree.h"
#include "BinaryNodeTree.h" 
#include "BinaryNode.h" 
#include <iostream>


  template<class ItemType>
  auto BinarySearchTree<ItemType>::placeNode( shared_ptr<BinaryNode<ItemType>> subTreePtr,
                                        shared_ptr<BinaryNode<ItemType>> newNode)
    {
           if (subTreePtr == nullptr)
              return newNode;
           else
           {
              if (subTreePtr->getItem() > newNode->getItem())
              {
                 subTreePtr->setLeftChildPtr(placeNode(subTreePtr->getLeftChildPtr() , newNode));
              }
              else
              {
                 subTreePtr->setRightChildPtr(placeNode(subTreePtr->getRightChildPtr() , newNode));
              }

              return subTreePtr;
           }
    }

   // Removes the given target value from the tree while maintaining a binary search tree.
   template<class ItemType>
    shared_ptr<BinaryNode<ItemType>> BinarySearchTree<ItemType>::removeValue( shared_ptr<BinaryNode<ItemType>> subTreePtr,
                                     const ItemType target,
                                     bool& success)
        {
            if (subTreePtr == nullptr)
                {
                    success = false;
                }
            else if (subTreePtr->getItem() == target)
            {
                subTreePtr = removeNode(subTreePtr);
                success = true;
            }
            else if (subTreePtr->getItem() > target)
            {
                 shared_ptr<BinaryNode<ItemType>> tempPtr = removeValue(subTreePtr->getLeftChildPtr(), target, success);
                subTreePtr->setLeftChildPtr(tempPtr);
            }
            else
            {
                 shared_ptr<BinaryNode<ItemType>> tempPtr = removeValue(subTreePtr->getRightChildPtr(), target, success);
            	subTreePtr->setRightChildPtr(tempPtr);
            }

            return subTreePtr;
        }

	template<class ItemType>
	bool BinarySearchTree<ItemType>::add(const ItemType& newData)
	{
		auto newNodePtr =  make_shared<BinaryNode<ItemType>>(newData);
		rootPtr = placeNode(rootPtr, newNodePtr);
		return true;
	}

   // Removes a given node from a tree while maintaining a binary search tree.
  template<class ItemType>
  auto BinarySearchTree<ItemType>::removeNode( shared_ptr<BinaryNode<ItemType>> nodePtr)
   {
    if(nodePtr->getLeftChildPtr() == nullptr || nodePtr->getRightChildPtr() == nullptr)
        {
		nodePtr = nullptr;
        return nodePtr;
        }
    else if((nodePtr->getLeftChildPtr() !=nullptr) != (nodePtr->getRightChildPtr() !=nullptr))
    {	 shared_ptr<BinaryNode<ItemType>> nodeToConnectPtr = nullptr;
        if(nodePtr->getLeftChildPtr() !=nullptr)
        {
        nodeToConnectPtr = nodePtr->getLeftChildPtr();
    	}
        else
        {
        nodeToConnectPtr = nodePtr->getRightChildPtr();
        }
        return nodeToConnectPtr;
    }
    else
	{
		auto tempPtr = removeLeftmostNode(nodePtr->getRightChildPtr(), nodePtr->getItem());
		nodePtr->setRightChildPtr(tempPtr);
		nodePtr->setItem(nodePtr->getItem());
		return nodePtr;
	}
   }

   // Removes the leftmost node in the left subtree of the node
   // pointed to by nodePtr.
   // Sets inorderSuccessor to the value in this node.
   // Returns a pointer to the revised subtree.
   template<class ItemType>
   auto BinarySearchTree<ItemType>::removeLeftmostNode( shared_ptr<BinaryNode<ItemType>> subTreePtr,
                                            ItemType inorderSuccessor) 
    {
        if(subTreePtr->getLeftChildPtr() == nullptr)
       {
           inorderSuccessor = subTreePtr->getItem();
            return removeNode(subTreePtr);
       }
       else
        {
        auto tempPtr = removeLeftmostNode(subTreePtr->getLeftChildPtr(), inorderSuccessor);
        subTreePtr->setLeftChildPtr(tempPtr);
        return subTreePtr;
        }
    }

   // Returns a pointer to the node containing the given value, or nullptr if not found.
   template<class ItemType>
    shared_ptr<BinaryNode<ItemType>> BinarySearchTree<ItemType>::findNode( shared_ptr<BinaryNode<ItemType>> treePtr,
                                  const ItemType& target, bool& isSuccessful) const
        {
        if(treePtr == nullptr)
            return nullptr;
        else if(treePtr->getItem() == target)
        {
        	isSuccessful = true;
        	return treePtr;
		}
            
        else if(treePtr->getItem() > target)
            return findNode(treePtr->getLeftChildPtr(), target, isSuccessful);
        else
            return findNode(treePtr->getRightChildPtr(), target, isSuccessful);
        }
        
        
	template<class ItemType>
	BinarySearchTree<ItemType>::BinarySearchTree()
	{  }
	
	template<class ItemType>
	BinarySearchTree<ItemType>::BinarySearchTree(const ItemType& rootItem):rootPtr( make_shared<BinaryNode<ItemType>>(rootItem, nullptr, nullptr))
	{  }
	
	//copy constr
	template<class ItemType>
	BinarySearchTree<ItemType>::BinarySearchTree(const BinarySearchTree<ItemType>& tree)
	{
	   rootPtr = this->copyTree(tree.rootPtr);
	}
	
   //dest
	template<class ItemType>
	BinarySearchTree<ItemType>::~BinarySearchTree()
	{
	   this->destroyTree(rootPtr);
	}
	
/*	template<class ItemType>
	bool  BinarySearchTree<ItemType>::isEmpty() const 
	{
		???
	}*/

	template<class ItemType>
	int BinarySearchTree<ItemType>::getHeight() const
	{
		return this->getHeightHelper(rootPtr);
	}
		
	
	template<class ItemType>
	bool BinarySearchTree<ItemType>::isEmpty() const
	{
	   return rootPtr == nullptr;
	}
	
	
	template<class ItemType>
	int BinarySearchTree<ItemType>::getNumberOfNodes() const
	{
	   return this->getNumberOfNodesHelper(rootPtr);
	}
	
	template<class ItemType>
	void BinarySearchTree<ItemType>::clear()
	{
	   this->destroyTree(rootPtr);
	   this->rootPtr.reset();
	}
	
	template<class ItemType>
	ItemType BinarySearchTree<ItemType>::getRootData() const throw(PrecondViolatedExcep)
	{
	   if (isEmpty())
	      throw PrecondViolatedExcep("getRootData() called with empty tree."); 
	      
	   return this->rootPtr->getItem();
	}
	
	template<class ItemType>
	void BinarySearchTree<ItemType>::setRootData(const ItemType& newData) const throw(PrecondViolatedExcep)
	{
	   if (isEmpty())
	      this->rootPtr =  make_shared<BinaryNode<ItemType>>(newData, nullptr, nullptr);
	   else
	      this->rootPtr->setItem(newData);
	}
	
	
	
	template<class ItemType>
	ItemType BinarySearchTree<ItemType>::getEntry(const ItemType& anEntry) const throw(NotFoundException)
	{
	   bool isSuccessful = false;
	   auto binaryNodePtr = findNode(rootPtr, anEntry, isSuccessful);
	   
	   if (isSuccessful)
	      return binaryNodePtr->getItem(); 
	   else 
	      throw NotFoundException("Entry not found in tree!");
	}
	
	template<class ItemType>
	bool BinarySearchTree<ItemType>:: contains(const ItemType& anEntry) const
	{
	   bool isSuccessful = false;
	   findNode(rootPtr, anEntry, isSuccessful);
	   return isSuccessful;   
	}
	
	
	template<class ItemType>
	bool BinarySearchTree<ItemType>::remove(const ItemType& anEntry)
	{
	   bool isSuccessful = false;
	   rootPtr = removeValue(rootPtr, anEntry, isSuccessful);
	   return isSuccessful; 
	}
	
	//Public Traversals Section

	template<class ItemType>
	void BinarySearchTree<ItemType>::preorderTraverse(void visit(ItemType&)) const
	{
	   this->preorder(visit, rootPtr);
	}
	
	template<class ItemType>
	void BinarySearchTree<ItemType>::inorderTraverse(void visit(ItemType&)) const
	{
	   this->inorder(visit, rootPtr);
	}
	
	template<class ItemType>
   void BinarySearchTree<ItemType>::postorderTraverse(void visit(ItemType&)) const
   {
      this->postorder(visit, rootPtr);
   }
