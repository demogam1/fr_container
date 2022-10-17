/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bst_tri.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kali <kali@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 10:49:10 by misaev            #+#    #+#             */
/*   Updated: 2022/10/17 20:36:57 by kali             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// Binary Search Tree operations in C++

#include <iostream>


namespace ft
{
    template<class T>
    class node
    {
        public:
            typedef T value_type;
            
            node(T item)
            {
                this->key = item;
                this->left = NULL;
                this->right = NULL;
            }
            ~node(){}
        private:
            T key;
            node<T> *left;
            node<T> *right;       
    }
};



namespace tt
{
    using namespace std;

    struct node 
    {
        int key;
        struct node *left, *right;
    };

    // Create a node
    struct node *newNode(int item) 
    {
        struct node *temp = (struct node *)malloc(sizeof(struct node));
        temp->key = item;
        temp->left = temp->right = NULL;
        return temp;
    }

    // Inorder Traversal (fonction qui sert juste a afficher )
    void inorder(struct node *root) 
    {
        if (root != NULL) 
        {
            // Traverse left
            inorder(root->left);

            // Traverse root
            cout << root->key << " -> ";

            // Traverse right
            inorder(root->right);
        }
    }

    // Insert a node
    struct node *insert(struct node *node, int key) 
    {
        // Return a new node if the tree is empty
        if (node == NULL) return newNode(key);

        // Traverse to the right place and insert the node
        if (key < node->key)
            node->left = insert(node->left, key);
        else
            node->right = insert(node->right, key);

        return node;
    }

    // Find the inorder successor
    struct node *minValueNode(struct node *node) 
    {
        struct node *current = node;

        // Find the leftmost leaf
        while (current && current->left != NULL)
            current = current->left;

        return current;
    }

    // Deleting a node
    struct node *deleteNode(struct node *root, int key) 
    {
        // Return if the tree is empty
        if (root == NULL) return root;

        // Find the node to be deleted
        if (key < root->key)
            root->left = deleteNode(root->left, key);
        else if (key > root->key)
            root->right = deleteNode(root->right, key);
        else {
                // If the node is with only one child or no child
                if (root->left == NULL) {
                struct node *temp = root->right;
                free(root);
                return temp;
                } else if (root->right == NULL) {
                struct node *temp = root->left;
                free(root);
                return temp;
                }

                // If the node has two children
                struct node *temp = minValueNode(root->right);

                // Place the inorder successor in position of the node to be deleted
                root->key = temp->key;

                // Delete the inorder successor
                root->right = deleteNode(root->right, temp->key);
            }
    return root;
    }
}

// Driver code
