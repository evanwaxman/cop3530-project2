//
//  BSTLEAF.h
//  project2
//
//  Created by Evan Waxman on 12/5/17.
//  Copyright © 2017 Evan Waxman. All rights reserved.
//

#ifndef BSTLEAF_h
#define BSTLEAF_h

#include <iostream>

namespace cop3530 {

    template <typename K, typename V, bool (*compare)(const K&, const K&), bool (*is_equal)(const K&, const K&)>
    class BSTLEAF {
    public:
        BSTLEAF();  // constructor
        
        
        //root->left = copy(old_root->left)
        
        
        ~BSTLEAF(); // destructor
        
        void insert(K key, V value);
        void remove(K key);
        V& lookup(K key);
        
        
    private:
        struct Node {
            K key;
            V value;
            Node* left;
            Node* right;
            
        };  // end of Node struct
        
        Node* root;
        
        void insert(Node* curr, K key, V value);
        Node* remove(Node* curr, K key);
        V& lookup(Node* curr, K key);
        void clear_tree(Node* curr);
        Node* search_left(Node* curr);
        Node* search_right(Node* curr);
        
    };  // end of BSTLEAF class


    /******************************************
     *  constructor
     *****************************************/
    template <typename K, typename V, bool (*compare)(const K&, const K&), bool (*is_equal)(const K&, const K&)>
    BSTLEAF<K, V, compare, is_equal>::BSTLEAF() {
        root = nullptr;
    }

    /******************************************
     *  destructor
     *****************************************/
    template <typename K, typename V, bool (*compare)(const K&, const K&), bool (*is_equal)(const K&, const K&)>
    BSTLEAF<K, V, compare, is_equal>::~BSTLEAF() {
        if (root) {
            clear_tree(root);
        }
    }

    /******************************************
     *  insert (public)
     *****************************************/
    template <typename K, typename V, bool (*compare)(const K&, const K&), bool (*is_equal)(const K&, const K&)>
    void BSTLEAF<K, V, compare, is_equal>::insert(K key, V value) {
        if (root != nullptr) {
            insert(root, key, value);
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
    void BSTLEAF<K, V, compare, is_equal>::remove(K key) {
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
    V& BSTLEAF<K, V, compare, is_equal>::lookup(K key) {
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
    void BSTLEAF<K, V, compare, is_equal>::insert(Node* curr, K key, V value) {
        if (is_equal(curr->key, key)) {
            throw std::runtime_error("BSTLEAF<K, V, compare, is_equal>.insert: TRYING TO INSERT WITH A DUPLICATE KEY");
        }else if (compare(curr->key, key)) {    // go right
            if (curr->right == nullptr) {
                Node* new_node = new Node;
                curr->right = new_node;
                new_node->key = key;
                new_node->value = value;
                new_node->left = nullptr;
                new_node->right = nullptr;
            } else {
                insert(curr->right, key, value);
            }
        } else {    // go left
            if (curr->left == nullptr) {
                Node* new_node = new Node;
                curr->left = new_node;
                new_node->key = key;
                new_node->value = value;
                new_node->left = nullptr;
                new_node->right = nullptr;
            } else {
                insert(curr->left, key, value);
            }
        }
    }
    
    /******************************************
     *  remove (private)
     *****************************************/
    template <typename K, typename V, bool (*compare)(const K&, const K&), bool (*is_equal)(const K&, const K&)>
    typename BSTLEAF<K, V, compare, is_equal>::Node* BSTLEAF<K, V, compare, is_equal>::remove(Node* curr, K key) {
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
    V& BSTLEAF<K, V, compare, is_equal>::lookup(Node* curr, K key) {
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
    void BSTLEAF<K, V, compare, is_equal>::clear_tree(Node* curr) {
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
    typename BSTLEAF<K, V, compare, is_equal>::Node* BSTLEAF<K, V, compare, is_equal>::search_left(Node* curr) {
        while (curr->left) {
            curr = curr->left;
        }
        return curr;
    }
    
    /******************************************
     *  search_right
     *****************************************/
    template <typename K, typename V, bool (*compare)(const K&, const K&), bool (*is_equal)(const K&, const K&)>
    typename BSTLEAF<K, V, compare, is_equal>::Node* BSTLEAF<K, V, compare, is_equal>::search_right(Node* curr) {
        while (curr->right) {
            curr = curr->right;
        }
        return curr;
    }

}


    
#endif /* BSTLEAF_h */
