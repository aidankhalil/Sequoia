#include "sequoia.h"

// ************** SEARCH FUNCTION ************** //
//Return pointer to 1st node containing x in this sequoia (null pointer - x not found)
SequoiaNode* SequoiaNode::search(int x){
    if (x == value)
        return this;
    else if (x < value)
    {
      if (left != nullptr)
        return left->search(x);
      else
        return nullptr;
    }
    else if (x > value && right != nullptr)
        return right->search(x);
    else
        return nullptr;
}



// ************** ROTATE LEFT FUNCTION ************** //
//Case 1 : the left subtree height is greater than or equal to the right subtree height, but less than double
//Case 1 Solution : Rotate right child of the node to the left, using this function.
void SequoiaNode::rotateLeft(){
  SequoiaNode* innerChild = left;
  SequoiaNode* grandparent = parent->parent;
  this->left = parent;
  this->left->parent = this;
  parent->right = innerChild;
  if (innerChild != nullptr)
    innerChild->parent = parent;
  if ( (grandparent != nullptr) && (parent->value < grandparent->value) )
    grandparent->left = this;
  else if (grandparent != nullptr)
    grandparent->right = this;
  parent = grandparent;
}



// ************** ROTATE RIGHT FUNCTION ************** //
//Case 2 : the right subtree height is greater than the left but not double
//Case 2 Solution : Rotate left child of the node to the right, using this function.
void SequoiaNode::rotateRight(){
  SequoiaNode* innerChild = right;
  SequoiaNode* grandparent = parent->parent;
  this->right = parent;
  this->right->parent = this;
  parent->left = innerChild;
  if (innerChild != nullptr)
    innerChild->parent = parent;
  if (grandparent != nullptr && parent->value < grandparent->value)
    grandparent->left = this;
  else if (grandparent != nullptr)
    grandparent->right = this;
  parent = grandparent;
}



// ************** INSERT TO TREE FUNCTION ************** //
//Insert a new node into Sequoia & Updates root (if NULL)
/* alternative insert function 
void Sequoia::insert(int x){
  if (root == nullptr)
    root = new SequoiaNode(x);
  else
    root->insert(x);
  while (root->parent != nullptr)
    root = root->parent;
  size++;
} 
*/
void Sequoia::insert(int x){
  if (root != nullptr){
    root->insert(x);
    }
  else
    root = new SequoiaNode(x);
  size++;
  while (root->parent != nullptr)
    root = root->parent;
}



// ************** INSERT NODE FUNCTION ************** //
//add x to the tree rooted here
void SequoiaNode::insert(int x){
    if (x < value)
    {
        //Left
        if (left == nullptr)
        {
            left = new SequoiaNode(x);
            left->parent = this;
            fixBalanceInsert(); //updateheight and fix tallness.
        }
        else
            left->insert(x);
    }
    else
    {
        //Right
        if (right == nullptr)
        {
            right = new SequoiaNode(x);
            right->parent = this;
            fixBalanceInsert(); //updateheight and fix tallness
        }
        else
            right->insert(x);
    }
}



// ************** FIXBALANCE INSERT FUNCTION ************** //
// Fix Sequoia tallness post-insertion (same function will be applied post-deletion, no need for seperate "fixbalanceRemove")
void SequoiaNode::fixBalanceInsert()  // called on newly INSERTED node's parent, or DELETED node's original parent (use temp variable to store this parent, as it will be lost post-removal otherwise)
{
//****** UPDATE HEIGHT ******: 

  for(SequoiaNode* it=this; it!=nullptr; it=it->parent)
  {
    it->height = it->updateHeight();
  }


//****** ENSURE TALLNESS  (by proper rotations) ******: 
  int lht,rht = 0;
  if (left!=nullptr)
    lht = left->height;
  if (right!=nullptr)
    rht = right->height;

  SequoiaNode* temp = parent;

  if( (lht >= rht) && (lht < 2*(rht)) && rht > 0)
  {
    //case 1.
    right->rotateLeft();                    //rotate right child to the left.
    this->height = updateHeight();          //update height of this node and the rotated child.
    parent->height = parent->updateHeight();
  }
  else if( (rht >= lht) && (rht < 2*(lht)) && lht > 0)
  {
    //case 2.
    left->rotateRight();                    //rotate right child to the left.
    this->height = updateHeight();          //update height of this node and the rotated child.
    parent->height = parent->updateHeight();
  }

  if(temp!=nullptr)
    temp->fixBalanceInsert();               //recursive call of fixbalanceInsert() function to go up the tree (parent, parent->parent, parent->parent->parent, etc) ensuring tallness and updated heights until root.
}



// ************** REMOVE FROM TREE FUNCTION ************** //
//Removes a node from Sequoia & Updates root (if victim)
/* alternative remove function
void Sequoia::remove(int x)
{
  if (root == nullptr)
    return;
  SequoiaNode* victim = root->search(x);
  if (victim == nullptr)
    return;
  else{
    if (victim == root)
      if(root->left != nullptr)
        root = root->left;
      else
        root=root->right;
      victim = victim->remove();
      delete victim;  
    size--;
  }
}
*/
void Sequoia::remove(int x){
  SequoiaNode* victim = root->search(x);
  if (victim == nullptr)
    return;
  else if (victim == root)
    root = victim->remove();
  else
    victim->remove();
  size--;
  while (root->parent != nullptr)
    root = root->parent; //re-updating the root post-deletion
} 


// ************** REMOVE NODE FUNCTION ************** //
//remove this node from Sequoia tree & returns new root node, regardless of case.
SequoiaNode* SequoiaNode::remove()
{
  SequoiaNode* root = nullptr;
  SequoiaNode* tempnode = parent;   //used for deletion, temp will be == nullptr if deleting the root.

  //Leaf deletion
  if (left == nullptr && right == nullptr)
  {
    if (parent == nullptr)
      root = nullptr;
    else if (parent->left == this)
      parent->left = nullptr;
    else
      parent->right = nullptr;
    
    delete this;
    tempnode->fixBalanceInsert();
  }
  //Single child deletion
  else if ((left != nullptr && right == nullptr) || (right != nullptr && left == nullptr))
  {
    SequoiaNode* child;
    if (left != nullptr)
      child = left;
    else
      child = right;
    
    child->parent = parent;
    if (parent == nullptr)
      root = child;
    else if (parent->left == this)
      parent->left = child;
    else
      parent->right = child;
    
    left = right = nullptr;
    delete this;
    tempnode->fixBalanceInsert();
  }
  //double child deletion
  else 
  {
    //Swap node is max(lhs)
    SequoiaNode* swap = left;
    while (swap->right != nullptr)
      swap = swap->right;
    
    //Swap data values between swap & victim
    int temp = swap->value;
    swap->value = value;
    value = temp;
    
    //Delete swap node (0 or 1 children)
    swap->remove(); // This recursive call means there is no need to fixBalance() for this third case.
  }
  return root;
}



// ************** UPDATEHEIGHT FUNCTION ************** //
//calculate height of this node.  [ syntax: node->height = node->updateHeight()  ]
int SequoiaNode::updateHeight() const{
  int lht = 0, rht = 0;
  if (left != nullptr)
    lht = left->height;
  if (right != nullptr)
    rht = right->height;
  if (lht > rht)
    return 1 + lht;
  else
    return 1 + rht; //returns 1 + height of the taller child
}

