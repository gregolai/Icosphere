#ifndef SIMPLE_BINARY_TREE_H
#define SIMPLE_BINARY_TREE_H

#include "_header.h"

template<class T>
class SimpleBinaryTree
{
public:
	SimpleBinaryTree(bool allow_repeats)
		: mAllowRepeats(allow_repeats), mRoot(NULL)
	{
	}
	~SimpleBinaryTree()
	{
		RemoveAllRecursive(mRoot);
	}

	bool Insert(const T& item)
	{
		Node* cur = mRoot;
		Node* prev = NULL;
		if(mAllowRepeats) {
			while(cur != NULL) {
				prev = cur;
				if(item <= cur->item) { cur = cur->left; }
				else { cur = cur->right; }
			}
		} else {
			while(cur != NULL) {
				prev = cur;
				if(item < cur->item) { cur = cur->left; }
				else if(item > cur->item) { cur = cur->right; }
				else { return false; }
			}
		}
		cur = new Node;
		cur->item = item;
		cur->parent = prev;
		cur->left = NULL;
		cur->right = NULL;
		if(prev) {
			if(item <= prev->item) {
				prev->left = cur;
			} else {
				prev->right = cur;
			}
		} else {
			mRoot = cur;
		}
		return true;
	}
private:
	struct Node
	{
		T item;
		Node* left;
		Node* right;
		Node* parent;
	};

	void RemoveAllRecursive(Node* cur)
	{
		if(cur) {
			RemoveAllRecursive(cur->left);
			RemoveAllRecursive(cur->right);
			delete cur;
			cur = NULL;
		}
	}

	Node* mRoot;
	bool mAllowRepeats;
};

#endif
