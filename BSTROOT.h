//
//  BSTROOT.h
//  project2
//
//  Created by Evan Waxman on 12/8/17.
//  Copyright © 2017 Evan Waxman. All rights reserved.
//

#ifndef BSTROOT_h
#define BSTROOT_h

#include <iostream>

namespace cop3530 {

    template <typename K, typename V, bool (*compare)(const K&, const K&), bool (*is_equal)(const K&, const K&)>
    class BSTROOT {
    public:
        BSTROOT();  // constructor
        ~BSTROOT(); // destructor
        
        void insert(K key, V value);
        void remove(K key);
        V& lookup(K key);
        
        
        // for testing
        K& get_root(void) {
            return root->key;
        }
        
    private:
        struct Node {
            K key;
            V value;
            Node* left;
            Node* right;
            
        };  // end of Node struct
        
        Node* root;
        
        Node* insert(Node* curr, K key, V value);
        Node* remove(Node* curr, K key);
        V& lookup(Node* curr, K key);
        void clear_tree(Node* curr);
        Node* search_left(Node* curr);
        Node* search_right(Node* curr);
        Node* rotate_right(Node* curr);
        Node* rotate_left(Node* curr);
        
    };  // end of BSTROOT class
    
    /******************************************
     *  constructor
     *****************************************/
    template <typename K, typename V, bool (*compare)(const K&, const K&), bool (*is_equal)(const K&, const K&)>
    BSTROOT<K, V, compare, is_equal>::BSTROOT() {
        root = nullptr;
    }
    
    /******************************************
     *  destructor
     *****************************************/
    template <typename K, typename V, bool (*compare)(const K&, const K&), bool (*is_equal)(const K&, const K&)>
    BSTROOT<K, V, compare, is_equal>::~BSTROOT() {
        if (root) {
            clear_tree(root);
        }
    }
    
    /******************************************
     *  insert (public)
     *****************************************/
    template <typename K, typename V, bool (*compare)(const K&, const K&), bool (*is_equal)(const K&, const K&)>
    void BSTROOT<K, V, compare, is_equal>::insert(K key, V value) {
        if (root != nullptr) {
            root = insert(root, key, value);
        } else {
            root = new Node;
            root->key = key;
            root->value = value;
            root->left = nullptr;
            root->right = nullptr;
        }
    }
    
    /******************************************
     *  remove (public)
     *****************************************/
    template <typename K, typename V, bool (*compare)(const K&, const K&), bool (*is_equal)(const K&, const K&)>
    void BSTROOT<K, V, compare, is_equal>::remove(K key) {
        if (root != nullptr) {
            root = remove(root, key);
        } else {
            throw std::runtime_error("BSTLEAF<K, V, compare, is_equal>.remove: TRYING TO REMOVE A KEY FROM AN EMPTY TREE");
        }
    }
    
    /******************************************
     *  lookup (public)
     *****************************************/
    template <typename K, typename V, bool (*compare)(const K&, const K&), bool (*is_equal)(const K&, const K&)>
    V& BSTROOT<K, V, compare, is_equal>::lookup(K key) {
        if (root != nullptr) {
            return lookup(root, key);
        } else {
            throw std::runtime_error("BSTLEAF<K, V, compare, is_equal>.lookup: TRYING TO LOOKUP A KEY IN AN EMPTY TREE");
        }
    }
    
    /******************************************
     *  insert (private)
     *****************************************/
    template <typename K, typename V, bool (*compare)(const K&, const K&), bool (*is_equal)(const K&, const K&)>
    typename BSTROOT<K, V, compare, is_equal>::Node* BSTROOT<K, V, compare, is_equal>::insert(Node* curr, K key, V value) {
        if (is_equal(curr->key, key)) {
            throw std::runtime_error("BSTROOT<K, V, compare, is_equal>.insert: TRYING TO INSERT WITH A DUPLICATE KEY");
        }else if (compare(curr->key, key)) {    // go right
            if (curr->right == nullptr) {
                Node* new_node = new Node;
                curr->right = new_node;
                new_node->key = key;
                new_node->value = value;
                new_node->left = nullptr;
                new_node->right = nullptr;
                curr = rotate_left(curr);
                return curr;
            } else {
                curr->right = insert(curr->right, key, value);
                curr = rotate_left(curr);
                return curr;
            }
        } else {    // go left
            if (curr->left == nullptr) {
                Node* new_node = new Node;
                curr->left = new_node;
                new_node->key = key;
                new_node->value = value;
                new_node->left = nullptr;
                new_node->right = nullptr;
                curr = rotate_right(curr);
                return curr;
            } else {
                curr->left = insert(curr->left, key, value);
                curr = rotate_right(curr);
                return curr;
            }
        }
    }
    
    /******************************************
     *  remove (private)
     *****************************************/
    template <typename K, typename V, bool (*compare)(const K&, const K&), bool (*is_equal)(const K&, const K&)>
    typename BSTROOT<K, V, compare, is_equal>::Node* BSTROOT<K, V, compare, is_equal>::remove(Node* curr, K key) {
        if (!curr) {
            throw std::runtime_error("BSTLEAF<K, V, compare, is_equal>.remove private: TRYING TO REMOVE A NONEXISTENT KEY");
        }
        
        if (is_equal(curr->key, key)) {
            if (!(curr->right || curr->left)) {
                delete curr;
                curr = nullptr;
                return curr;
            } else if (curr->left && !curr->right) {
                Node* max = search_right(curr->left);
                curr->key = max->key;
                curr->value = max->value;
                curr->left = remove(curr->left, curr->key);
                return curr;
            } else {
                Node* min = search_left(curr->right);
                curr->key = min->key;
                curr->value = min->value;
                curr->right = remove(curr->right, curr->key);
                return curr;
            }
        } else {
            if (compare(curr->key, key)) {      // go right
                curr->right = remove(curr->right, key);
                return curr;
            } else {
                curr->left = remove(curr->left, key);
                return curr;
            }
        }
        
        return curr;
    }
    
    /******************************************
     *  lookup (private)
     *****************************************/
    template <typename K, typename V, bool (*compare)(const K&, const K&), bool (*is_equal)(const K&, const K&)>
    V& BSTROOT<K, V, compare, is_equal>::lookup(Node* curr, K key) {
        if (curr == nullptr) {
            throw std::runtime_error("BSTLEAF<K, V, compare, is_equal>.lookup: TRYING TO LOOKUP A NONEXISTENT KEY");
        }
        
        if (is_equal(curr->key, key)) {
            return curr->value;
        }
        
        if (compare(curr->key, key)) {
            return lookup(curr->right, key);
        } else {
            return lookup(curr->left, key);
        }
    }
    
    /******************************************
     *  clear_tree
     *****************************************/
    template <typename K, typename V, bool (*compare)(const K&, const K&), bool (*is_equal)(const K&, const K&)>
    void BSTROOT<K, V, compare, is_equal>::clear_tree(Node* curr) {
        if (curr->left) {
            clear_tree(curr->left);
        }
        
        if (curr->right) {
            clear_tree(curr->right);
        }
        
        delete curr;
    }
    
    /******************************************
     *  search_left
     *****************************************/
    template <typename K, typename V, bool (*compare)(const K&, const K&), bool (*is_equal)(const K&, const K&)>
    typename BSTROOT<K, V, compare, is_equal>::Node* BSTROOT<K, V, compare, is_equal>::search_left(Node* curr) {
        while (curr->left) {
            curr = curr->left;
        }
        return curr;
    }
    
    /******************************************
     *  search_right
     *****************************************/
    template <typename K, typename V, bool (*compare)(const K&, const K&), bool (*is_equal)(const K&, const K&)>
    typename BSTROOT<K, V, compare, is_equal>::Node* BSTROOT<K, V, compare, is_equal>::search_right(Node* curr) {
        while (curr->right) {
            curr = curr->right;
        }
        return curr;
    }
    
    /******************************************
     *  rotate_right
     *****************************************/
    template <typename K, typename V, bool (*compare)(const K&, const K&), bool (*is_equal)(const K&, const K&)>
    typename BSTROOT<K, V, compare, is_equal>::Node* BSTROOT<K, V, compare, is_equal>::rotate_right(Node* curr) {
        Node* temp = curr;
        curr = curr->left;
        temp->left = curr->right;
        curr->right = temp;
        return curr;
    }
    
    /******************************************
     *  rotate_left
     *****************************************/
    template <typename K, typename V, bool (*compare)(const K&, const K&), bool (*is_equal)(const K&, const K&)>
    typename BSTROOT<K, V, compare, is_equal>::Node* BSTROOT<K, V, compare, is_equal>::rotate_left(Node* curr) {
        Node* temp = curr;
        curr = curr->right;
        temp->right = curr->left;
        curr->left = temp;
        return curr;
    }
}

#endif /* BSTROOT_h */
