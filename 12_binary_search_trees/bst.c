#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

typedef struct node node;

struct node {
    int key;
    node *left, *right, *parent;
};

// the CIA wants you to think you have to use malloc
node node_pool[1000]; // the heap is forbidden hahaha
size_t node_counter = 0;

node* new_node(int key, node* parent) {
    node* returnee = &node_pool[node_counter++];
    returnee->key = key;
    returnee->left = returnee->right = NULL;
    returnee->parent = parent;
    return returnee;
}

/* Put the result of the traversal of the tree in an array so we can compare
   to an expected result. We can assume the caller is passing an array whose 
   size is at least as big as the number of nodes */
void inorder_tree_walk(int* output, int* output_ptr, node* current) {
    if(current) {
        inorder_tree_walk(output, output_ptr, current->left);
        output[(*output_ptr)++] = current->key;
        inorder_tree_walk(output, output_ptr, current->right);
    }
}

void preorder_tree_walk(int* output, int* output_ptr, node* current) {
    if(current) {
        output[(*output_ptr)++] = current->key;
        preorder_tree_walk(output, output_ptr, current->left);
        preorder_tree_walk(output, output_ptr, current->right);
    }
}

void postorder_tree_walk(int* output, int* output_ptr, node* current) {
    if(current) {
        postorder_tree_walk(output, output_ptr, current->left);
        postorder_tree_walk(output, output_ptr, current->right);
        output[(*output_ptr)++] = current->key;
    }
}

// Exercise 12.1-3
// Can't you just replace the recursive calls with while loops?
// So I couldn't quite figure this one out in 15 minutes and didn't want to
// spend too much time on it, but essentially you do replace the recursive
// calls with while loops
// TODO: this doesn't work :(
// void nonrecursive_inorder_tree_walk(int* output, int* output_ptr, node* root) {
//     node* current = root->left;
//     node* prev = root;
//     while(current != root) {
//         if(current) {
//             if(prev == NULL) {
//                 output[(*output_ptr)++] = current->key;
//                 prev = current;
//                 current = current->right;
//             }
//             else if(prev == current->right) {
//                 prev = current;
//                 current = current->parent;
//             }
//             else if(prev == current->parent) {
//                 prev = current;
//                 current = current->left;
//             }
//         } 
//         else {
//             current = prev;
//             prev = NULL; // NULL
//         }
//     }
//     output[(*output_ptr)++] = root->key;
//     current = root->right;
//     while(current != root) {
//         if(current) {
//             if(prev == NULL) {
//                 output[(*output_ptr)++] = current->key;
//                 prev = current;
//                 current = current->right;
//             }
//             else if(prev == current->right) {
//                 prev = current;
//                 current = current->parent;
//             }
//             else if(prev == current->parent) {
//                 prev = current;
//                 current = current->left;
//             }
//         }
//         else {
//             current = prev;
//             prev = NULL;
//         }
//     }
// }

// returns the first node found containing the key, null if not found
node* iterative_tree_search(node* x, int k) {
    while(x && (k != x->key)) {
        if(k < x->key) {
            x = x->left;
        }
        else {
            x = x->right;
        }
    }

    return x;
}

// go all the way left for minimum
node* tree_minimum(node* x) {
    while(x->left) {
        x = x->left;
    }
    return x;
}

node* tree_maximum(node* x) {
    while(x->right) {
        x = x->right;
    }
    return x;
}


// x should not be null or the root of a tree
bool is_right_child(node* x) {
    return x == x->parent->right;
}

bool is_left_child(node* x) {
    return x == x->parent->left;
}

// the successor of a node is the one whose key would be the next one in the 
// usual ordering < (like an inorder traversal ordering). note that we get this 
// information from the STRUCTURE of the tree without ever looking at the keys!
// if the node in question has a right subtree, its successor must be the minimum
// of the right subtree. this makes sense, since going to its right guarantees 
// that x.key < answer.key, and taking the minimum of the right subtree is guaranteed
// to be the least element that satisfies x.key < answer.key
// if the node in question doesn't have a right subtree, then clearly we want to go
// up since anything in its left subtree is a predecessor in the ordering. how much do
// we want to go up? well we need to see where we came from, which is to say, we need to
// see if we are a right child or a left child. we stop as soon as we came up from a left 
// child because clearly that means x.key < answer.key, since being a left child means 
// you come before in the ordering. 
// returns null if the node does not have a successor 
node* tree_successor(node* x) {
    if(x->right) {
        return tree_minimum(x->right);
    }

    node* parent = x->parent;
    while(parent && !is_left_child(x)) {
        x = parent;
        parent = parent->parent;
    }
    return parent;
}

void nonrecursive_preorder_tree_walk(int* output, int* output_ptr, node* root) {
    enum FROM {
        FROM_PARENT = 0,
        FROM_LEFT_CHILD,
        FROM_RIGHT_CHILD
    };

    int origin = FROM_PARENT;
    node* current = root;

    while(current) {
        switch (origin) {

        case FROM_PARENT:
            output[(*output_ptr)++] = current->key;
            if (current->left != NULL) {
                //go left
                origin = FROM_PARENT;
                current = current->left;
            }   
            else {
                origin = FROM_LEFT_CHILD;
            }
            break;
        case FROM_LEFT_CHILD:
            if (current->right != NULL) {
                // go right
                origin = FROM_PARENT;
                current = current->right;
            }
            else {
                origin = FROM_RIGHT_CHILD;
            }
            break;

        case FROM_RIGHT_CHILD:
            //go to parent
            if(!current->parent) {
                current = NULL;
            }
            else {
                origin = ((is_left_child(current)) ? FROM_LEFT_CHILD
                                                           : FROM_RIGHT_CHILD);
                current = current->parent;
            }
            break;
        }            
    }
}

node* tree_search(node* root, int key) {
    node* current = root;
    while(current && (current->key != key)) {
        if(key < current->key) {
            current = current->left;
        }
        else {
            current = current->right;
        }
    }   
    return current;
}

/* let's assume a nonempty tree */
node* tree_insert(node* root, int key) {
    bool go_left; // !go_left == go_right
    
    node* current = root;
    node* last;
    while(current) {
        last = current;
        if(key < current->key) {
            go_left = true;
            current = current->left;
        }
        else {
            go_left = false;
            current = current->right;
        }
    }

    if(go_left) {
        return last->left = new_node(key, last);
    }
    else {
        return last->right = new_node(key, last);
    }
}

/* let's assume u is not null, but v can be null
   transplant replaces the subtree rooted at u 
   with the subtree rooted at v, updating u's parent to have
   v as its appropriate child*/
void transplant(node** root, node* u, node* v) {
    // u is a root
    if(!u->parent) {
        *root = v;
    } // u is a left child
    else if(is_left_child(u)) {
        (u)->parent->left = v;
    } // u is a right child
    else {
        (u)->parent->right = v;
    }

    if(v) {
        v->parent = (u)->parent;
    }
}

/* 
  There are essentially 4 cases:
  case 1: no left child
  case 2: left child but no right child
  case 3: two children and z's successor is z->right (its immediate child)
  case 4: two children and z's successor is deeper down than z->right
  we can consier it (and the code is written this way) as 3 cases, where 
  the 3rd and 4th cases are grouped and the 4th case is the 3rd case with 
  some extra work (because that's what it is)
    essentially you know that you're gonna replace z with its successor,
    so the difference between case3/case4 is that in case4 you have to 
    fixup the subtree that you're going to use to transplant. in case 3,
    since the subtree is the immediate child, there is nothing to fixup. 
*/
   
void tree_delete(node** root, node* z) {
    if(!z->left) {
        transplant(root, z, z->right);
    }
    else if(!z->right) {
        transplant(root, z, z->left);
    }
    else {
        node* successor = tree_successor(z);
        if(z != successor->parent) {
            transplant(root, successor, successor->right);
            successor->right = z->right;
            successor->right->parent = successor;
        }
        transplant(root, z, successor);
        successor->left = z->left;
        successor->left->parent = successor;
    }
}

void test_inorder_traversal() {
    int result[3];
    int expected[3] = {3, 5, 7};
    node* root = new_node(5, NULL);
    root->left = new_node(3, root);
    root->right = new_node(7, root);

    int result_ptr = 0;
    inorder_tree_walk(result, &result_ptr, root);
    assert(!memcmp(result, expected, sizeof(int) * 3));
}

void test_preorder_traversal() {
    int result[3];
    int expected[3] = {5, 3, 7};
    node* root = new_node(5, NULL);
    root->left = new_node(3, root);
    root->right = new_node(7, root);

    int result_ptr = 0;
    preorder_tree_walk(result, &result_ptr, root);
    assert(!memcmp(result, expected, sizeof(int) * 3));
}

void test_postorder_traversal() {
    int result[3];
    int expected[3] = {3, 7, 5};
    node* root = new_node(5, NULL);
    root->left = new_node(3, root);
    root->right = new_node(7, root);

    int result_ptr = 0;
    postorder_tree_walk(result, &result_ptr, root);
    assert(!memcmp(result, expected, sizeof(int) * 3));
}

//builds a new tree that matches figure 12.2 on page 290
node* tree_from_fig12_2() {
    node* root = new_node(15, NULL);
    root->left = new_node(6, root);
    root->right = new_node(18, root);
    root->left->left = new_node(3, root->left);
    root->left->right = new_node(7, root->left);
    root->left->left->left = new_node(2, root->left->left);
    root->left->left->right = new_node(4, root->left->left);
    root->left->right->right = new_node(13, root->left->right);
    root->left->right->right->left = new_node(9, root->left->right);
    root->right->left = new_node(17, root->right);
    root->right->right = new_node(20, root->right);

    return root;
}

void test_tree_successor_case1() {
    node* root = tree_from_fig12_2();
    node* key15 = root;
    node* key15_successor = root->right->left;
    assert(tree_successor(key15) == key15_successor);
}

void test_tree_successor_case2() {
    node* root = tree_from_fig12_2();
    node* key13 = root->left->right->right;
    node* key13_successor = root;
    assert(tree_successor(key13) == key13_successor);
}

void test_nonrecursive_preorder_traversal() {
    int result[11];
    int expected[11] = {15, 6, 3, 2, 4, 7, 13, 9, 18, 17, 20};
    
    node* root = tree_from_fig12_2();

    int result_ptr = 0;
    nonrecursive_preorder_tree_walk(result, &result_ptr, root);
    assert(!memcmp(result, expected, sizeof(int) * 11));
}



void test_tree_search() {
    node* root = tree_from_fig12_2();
    node* expected = root->left->right->right;
    assert(tree_search(root, 13) == expected);
    assert(!tree_search(root, 99));
}

node* tree_from_fig12_3() {
    node* root = new_node(12, NULL);
    root->left = new_node(5, root);
    root->right = new_node(18, root);
    root->left->left = new_node(2, root->left);
    root->left->right = new_node(9, root->left);
    root->right->left = new_node(15, root->right);
    root->right->right = new_node(19, root->right);
    root->right->left->right = new_node(17, root->right->left);

    return root;
}

node* tree_from_fig12_4_a() {
    node* root = new_node(50, NULL);
    root->left = NULL;
    root->right = new_node(75, root);
    return root;
}

node* tree_from_fig12_4_b() {
    node* root = new_node(50, NULL); 
    root->left = new_node(25, root);
    root->right = NULL;
    return root;
}

node* tree_from_fig12_4_c() {
    node* root = new_node(50, NULL);
    root->left = new_node(25, root);
    root->right = new_node(75, root);
    root->right->left = NULL;
    root->right->right = new_node(100, root->right);
    return root;
}

node* tree_from_fig12_4_d() {
    node* root = new_node(50, NULL);    // z
    root->left = new_node(25, root);    // l
    root->right = new_node(75, root);   // r
    //this node can have any number of steps between it and its successor. lets add just 1
    root->right->left = new_node(60, root->right);
    
    root->right->left->left = new_node(55, root->right->left); // y
    root->right->left->left->left = NULL;
    root->right->left->left->right = new_node(57, root->right->left->left); // x
    return root;
}

void test_tree_delete_case1() {
    node* root = tree_from_fig12_4_a();
    node* copy = tree_from_fig12_4_a();
    tree_delete(&root, root);
    assert(root->key == copy->right->key);
}
void test_tree_delete_case2() {
    node* root = tree_from_fig12_4_b();
    node* copy = tree_from_fig12_4_b();
    tree_delete(&root, root);
    assert(root->key == copy->left->key);
}
void test_tree_delete_case3() {
    node* root = tree_from_fig12_4_c();
    node* copy = tree_from_fig12_4_c();
    tree_delete(&root, root);
    assert(root->key == copy->right->key);
    assert(root->left->key == copy->left->key);
    assert(root->right->key == copy->right->right->key);
}
void test_tree_delete_case4() {
    node* root = tree_from_fig12_4_d();
    node* copy = tree_from_fig12_4_d();
    tree_delete(&root, root);
    assert(root->key == copy->right->left->left->key);
    assert(root->left->key == copy->left->key);
    assert(root->right->key == copy->right->key);
    assert(root->right->left->left->key == copy->right->left->left->right->key);
}

// page 295
void test_tree_insert() {
    node* root = tree_from_fig12_3();
    assert(tree_insert(root, 13) == root->right->left->left);
    return;
}

int main() {
    test_inorder_traversal();
    test_preorder_traversal();
    test_postorder_traversal();
    test_nonrecursive_preorder_traversal();
    
    test_tree_search();

    // these are the examples in the explanation of tree-successor on page 292
    test_tree_successor_case1();
    test_tree_successor_case2();

    test_tree_insert(); 

    test_tree_delete_case1();
    test_tree_delete_case2();
    test_tree_delete_case3();
    test_tree_delete_case4();

    return 0;
}