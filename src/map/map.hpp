#ifndef MAP_HPP
#define MAP_HPP

#include "types.h"
#include <initializer_list>
#include "const_size_allocator.hpp"

namespace nsRedBlackTree
{
  template <typename tKey, typename tValue>
  struct sTuple
  {
    tKey key;
    tValue value;
  };

  enum class eColor
  {
    Red,
    Black
  };

  template <typename tKey, typename tValue>
  struct sNode
  {
    sTuple<tKey, tValue> data;
    eColor color;

    sNode *parent;
    sNode *left;
    sNode *right;

    sNode<tKey, tValue> *GetOtherSibling(sNode<tKey, tValue> *arg_child)
    {
      if ((this->left != nullptr) && (this->left->data.key != arg_child->data.key))
      {
        return this->left;
      }
      else if ((this->right != nullptr) && (this->right->data.key != arg_child->data.key))
      {
        return this->right;
      }
      else
      {
        return nullptr;
      }
    }
  };

  template <typename tKey, typename tValue, ui32 MaxSize>
  class cRedBlackTree
  {
  public:
    cRedBlackTree(void);
    cRedBlackTree(const std::initializer_list<sTuple<tKey, tValue>> arg_initializerList);

    bool Insert(const tKey arg_key, const tValue arg_value = (tValue)0);
    bool Delete(const tKey arg_key);
    bool Search(const tKey arg_key, tValue &arg_value) const;
    void Clean(void);

  private:
    cConstSizeAllocator<sNode<tKey, tValue>, MaxSize> allocator;
    sNode<tKey, tValue> *root;

    sNode<tKey, tValue> *AllocateNewNode(const tKey arg_key = (tKey)0, const tValue arg_value = (tValue)0);
    void RemoveConsecutiveRedEdges(sNode<tKey, tValue> *arg_node);
    void MoveUp(sNode<tKey, tValue> *arg_node);
  };

  //***************************************************************************
  //
  //! Constructor.
  //
  //***************************************************************************
  template <typename tKey, typename tValue, ui32 MaxSize>
  inline cRedBlackTree<tKey, tValue, MaxSize>::cRedBlackTree(void) : root(nullptr)
  {
  }

  //***************************************************************************
  //
  //! Constructor with initializer list.
  //
  //***************************************************************************
  template <typename tKey, typename tValue, ui32 MaxSize>
  inline cRedBlackTree<tKey, tValue, MaxSize>::cRedBlackTree(const std::initializer_list<sTuple<tKey, tValue>> arg_initializerList) : cRedBlackTree<tKey, tValue, MaxSize>::cRedBlackTree()
  {
    for (const sTuple<tKey, tValue> &tuple : arg_initializerList)
    {
      this->Insert(tuple.key, tuple.value);
    }
  }

  //***************************************************************************
  //
  //! Returns new allocated node.
  //
  //***************************************************************************
  template <typename tKey, typename tValue, ui32 MaxSize>
  inline sNode<tKey, tValue> *cRedBlackTree<tKey, tValue, MaxSize>::AllocateNewNode(const tKey arg_key, const tValue arg_value)
  {
    sNode<tKey, tValue> *result = nullptr;
    allocator.Allocate(result);

    if (result != nullptr)
    {
      result->color = eColor::Black;
      result->left = nullptr;
      result->right = nullptr;
      result->parent = nullptr;
      result->data.key = arg_key;
      result->data.value = arg_value;
    }

    return result;
  }

  //***************************************************************************
  //
  //! Insert.
  //
  //***************************************************************************
  template <typename tKey, typename tValue, ui32 MaxSize>
  inline bool cRedBlackTree<tKey, tValue, MaxSize>::Insert(const tKey arg_key, const tValue arg_value)
  {
    // Handle empty root
    if (root == nullptr)
    {
      root = this->AllocateNewNode(arg_key, arg_value);
      return true;
    }

    // Find parent root for new node
    sNode<tKey, tValue> *parent = nullptr;
    sNode<tKey, tValue> *current = root;

    while (current != nullptr)
    {
      parent = current;

      if (arg_key < current->data.key)
      {
        current = current->left;
      }
      else if (arg_key > current->data.key)
      {
        current = current->right;
      }
      else
      {
        current->data.value = arg_value;
        return true;
      }
    }

    // Create new node and attach it to the tree
    sNode<tKey, tValue> *node = this->AllocateNewNode(arg_key, arg_value);

    if (node == nullptr)
    {
      return false;
    }

    node->color = eColor::Red;
    node->parent = parent;

    if (arg_key < parent->data.key)
    {
      parent->left = node;
    }
    else
    {
      parent->right = node;
    }

    // Check absence of two consecutive red edges
    this->RemoveConsecutiveRedEdges(node);

    return true;
  }

  //***************************************************************************
  //
  //! Remove two consecutive red edges.
  //
  //***************************************************************************
  template <typename tKey, typename tValue, ui32 MaxSize>
  inline void cRedBlackTree<tKey, tValue, MaxSize>::RemoveConsecutiveRedEdges(sNode<tKey, tValue> *arg_node)
  {
    sNode<tKey, tValue> *parent = arg_node->parent;

    if (parent == nullptr)
    {
      // node is root - make it black
      arg_node->color = eColor::Black;
      return;
    }

    if (parent->color == eColor::Black)
    {
      // no consecutive red edges
      return;
    }

    sNode<tKey, tValue> *grandparent = parent->parent;

    if (grandparent == nullptr)
    {
      // parent is root - make it black
      parent->color = eColor::Black;
      return;
    }

    sNode<tKey, tValue> *uncle = grandparent->GetOtherSibling(parent);

    // Red uncle found - change colors
    if ((uncle != nullptr) && (uncle->color == eColor::Red))
    {
      parent->color = eColor::Black;
      uncle->color = eColor::Black;
      grandparent->color = eColor::Red;

      RemoveConsecutiveRedEdges(grandparent);
      return;
    }

    // Rotations
    if (grandparent->left == parent)
    {
      // L
      if (parent->left == arg_node)
      {
        // LL
        this->MoveUp(parent);
      }
      else
      {
        // LR
        this->MoveUp(arg_node);
        this->MoveUp(arg_node);
      }
    }
    else
    {
      // R
      if (parent->left == arg_node)
      {
        // RL
        this->MoveUp(arg_node);
        this->MoveUp(arg_node);
      }
      else
      {
        // RR
        this->MoveUp(parent);
      }
    }
  }

  template <typename tKey, typename tValue, ui32 MaxSize>
  inline void cRedBlackTree<tKey, tValue, MaxSize>::MoveUp(sNode<tKey, tValue> *arg_node)
  {
    sNode<tKey, tValue> *parent = arg_node->parent;

    if (parent == nullptr)
    {
      // Noone to swap with
      return;
    }

    sNode<tKey, tValue> *grandParent = parent->parent;
    parent->parent = arg_node;

    if (grandParent != nullptr)
    {
      // Swap child of grandparent
      arg_node->parent = grandParent;

      if (grandParent->left == parent)
      {
        grandParent->left = arg_node;
      }
      else
      {
        grandParent->right = arg_node;
      }
    }
    else
    {
      // Parent was root so node is the new root
      arg_node->parent = nullptr;
      root = arg_node;
    }

    // Swap colors
    const eColor nodeColor = arg_node->color;
    arg_node->color = parent->color;
    parent->color = nodeColor;

    // Carrie one child of node to parent and set correct child of node
    if (parent->left == arg_node)
    {
      parent->left = arg_node->right;
      if (parent->left != nullptr)
      {
        parent->left->parent = parent;
      }

      arg_node->right = parent;
    }
    else
    {
      parent->right = arg_node->left;
      if (parent->right != nullptr)
      {
        parent->right->parent = parent;
      }

      arg_node->left = parent;
    }
  }

  //***************************************************************************
  //
  //! Delete.
  //
  //***************************************************************************
  template <typename tKey, typename tValue, ui32 MaxSize>
  inline bool cRedBlackTree<tKey, tValue, MaxSize>::Delete(const tKey arg_key)
  {
    return false;
  }

  //***************************************************************************
  //
  //! Search.
  //
  //***************************************************************************
  template <typename tKey, typename tValue, ui32 MaxSize>
  inline bool cRedBlackTree<tKey, tValue, MaxSize>::Search(const tKey arg_key, tValue &arg_value) const
  {
    sNode<tKey, tValue> *node = root;

    while (node != nullptr)
    {
      if (arg_key < node->data.key)
      {
        node = node->left;
      }
      else if (arg_key > node->data.key)
      {
        node = node->right;
      }
      else
      {
        break;
      }
    }

    if (node != nullptr)
    {
      arg_value = node->data.value;
    }

    return node != nullptr;
  }

  //***************************************************************************
  //
  //! Clean.
  //
  //***************************************************************************
  template <typename tKey, typename tValue, ui32 MaxSize>
  inline void cRedBlackTree<tKey, tValue, MaxSize>::Clean(void)
  {
    this->allocator.Clean();
    this->root = nullptr;
  }
}

#endif /* MAP_HPP */
