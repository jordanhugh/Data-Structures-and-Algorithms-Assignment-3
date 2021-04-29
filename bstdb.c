#include "bstdb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Tree_Node {
    int doc_id;
    char *name;
    int word_count;
    int height;
    struct Tree_Node *left;
    struct Tree_Node *right;
}Tree_Node;

int next_id;
Tree_Node *bst;

int num_comps;
int num_searches;

void tree_delete(Tree_Node* root){
    if (root != NULL){
        tree_delete(root->left);
        tree_delete(root->right);
        if (root->name){
            free(root->name);
        }
        free(root);
    }
}

int bst_len (Tree_Node* root) {
    static int count = 0;
    if (root != NULL){
        count++;
        bst_len(root->left);
        bst_len(root->right);
    }
    return count;
}

static Tree_Node *find_document (int doc_id) {
    Tree_Node* curr = bst;
    
    // Count how many times this function was called so we can get 
    // average number of nodes traversed for each query
    num_searches++;

    while (curr != NULL) {
        // Count the number of traversals
        num_comps++;

        if (doc_id < curr->doc_id){
            curr = curr->left;
        }
        else if (doc_id > curr->doc_id) {
            curr = curr->right;
        }
        else {
            return curr;
        }
    }
    return NULL;
}

static Tree_Node* node_init(char* name, int word_count, int doc_id){
    struct Tree_Node* node;
    size_t len_name;

    // Allocate memory for the new list node
    node = (Tree_Node*)malloc(sizeof(Tree_Node));
    
    // Malloc can fail, so make sure the node is pointing to something before we try
    // writing to it.
    if (node) {       
        // Store data in the list node
        node->doc_id = doc_id;
        node->word_count = word_count;
        node->height = 1;
        node->left = NULL;
        node->right = NULL;

        // Allocate memory to store the file name and copy the string into the
        // new list node
        len_name = strlen(name) + 1;
        node->name = calloc(len_name, sizeof(char));
        if (node->name) {
            // If calloc was successful, copy the filename into the node
            strcpy(node->name, name);
        } 
        else {
            // If calloc failed, release any memory that was allocated and 
            // report an error by returning NULL
            free(node->name);
            free(node);
            node = NULL;
        }
    }

    return node;
}

int max(int a, int b){
    if (a > b){
        return a;
    }
    else{
        return b;
    }
}

int height(Tree_Node* node){
     if (node == NULL) 
        return 0; 
    return (node->height); 
}

int recursHeight(Tree_Node* node){
     if (node == NULL) 
        return 0; 
    return (1 + max(recursHeight(node->left), recursHeight(node->right))); 
}

int balance(Tree_Node* node){
    if (node == NULL){
        return 0;
    }
    else{
        return(height(node->left) - height(node->right));
    }
}

int recursBalance(Tree_Node* node){
    if (node == NULL){
        return 0;
    }
    else{
        return(recursHeight(node->left) - recursHeight(node->right));
    }
}

bool checkIfBalanced(Tree_Node* root){
    if (root == NULL){
        return true;
    }
    
    int bal = recursBalance(root);
    if(abs(bal) < 2 && checkIfBalanced(root->left) && checkIfBalanced(root->right)){
        return true;    
    }

    return false;
}

bool checkBelow(Tree_Node* node, int doc_id){
    if(node == NULL){
        return false;
    }

    if(doc_id != node->doc_id && !checkBelow(node->left, doc_id) && !checkBelow(node->right, doc_id)){
        return false;
    }
    
    return true;
}

bool checkForDuplicates(Tree_Node* root){
    if(root == NULL){
        return false;
    }

    if(!checkBelow(root->left, root->doc_id) && !checkForDuplicates(root->left) && 
       !checkBelow(root->right, root->doc_id) && !checkForDuplicates(root->right)){

        return false;
    }

    return true;
}

void leftRotate(Tree_Node** root){
    Tree_Node* pivot = (*root)->right;

    //Perform Rotation
    (*root)->right = pivot->left;
    pivot->left = *root;

    //Update Heights
    (*root)->height = 1 + max(height((*root)->left), height((*root)->right));
    pivot->height = 1 + max(height(pivot->left), height(pivot->right));

    //Update Root
    *root = pivot;
}
void rightRotate(Tree_Node** root){
    Tree_Node* pivot = (*root)->left;

    //Perform Rotation
    (*root)->left = pivot->right;
    pivot->right = *root;

    //Update Heights
    (*root)->height = 1 + max(height((*root)->left), height((*root)->right));
    pivot->height = 1 + max(height(pivot->left), height(pivot->right));

    //Update Root
    *root = pivot;
}

void leftRightRotate(Tree_Node** root){
    leftRotate(&((*root)->left));
    rightRotate(root);
}

void rightLeftRotate(Tree_Node** root){
    rightRotate(&((*root)->right));
    leftRotate(root);
}

void tree_insert(Tree_Node** root, Tree_Node* node){
    if (*root == NULL){
        *root = node;
        return;
    }

    if (node->doc_id < (*root)->doc_id){
        tree_insert(&((*root)->left), node); 
    }
    else {
        tree_insert(&((*root)->right), node);
    }

    (*root)->height = 1 + max(height((*root)->left), height((*root)->right));
    int bal = balance(*root);

    //If the node is unbalanced then rotate the node
    //Right Rotate
    if (bal > 1 &&  node->doc_id < (*root)->left->doc_id){
        rightRotate(root);
    }
    //Left Right Rotate
    else if (bal > 1 && node->doc_id > (*root)->left->doc_id){
        leftRightRotate(root);
    }
    //Right Left Rotate
    else if (bal < -1 && node->doc_id < (*root)->right->doc_id){
        rightLeftRotate(root);
    }
    //Left Rotate
    else if (bal < -1 && node->doc_id > (*root)->right->doc_id){
        leftRotate(root);
    }
}

int bstdb_init ( void ) {
    bst = NULL;
    next_id = 0;
    num_comps = 0;
    num_searches = 0;
    return 1;
}

int bstdb_add (char *name, int word_count ) {
    Tree_Node* node;
    node = node_init(name, word_count, next_id);
    if(!node) { 
        return -1;
    }
    tree_insert(&bst, node);
    return next_id++;
}

int bstdb_get_word_count ( int doc_id ) {
	struct Tree_Node *node = find_document(doc_id);
    return (node)? node->word_count : -1;
}

char* bstdb_get_name ( int doc_id ) {
	struct Tree_Node* node = find_document(doc_id);
    return (node)? node->name : NULL;
}

void bstdb_stat ( void ) {
    printf("STAT\n");
    printf("Check if Balanced -> %c\n",
        ((checkIfBalanced(bst))? 'Y' : 'N') );
    printf("Correct Number of Nodes -> %c\n",
        ((next_id == bst_len(bst))? 'Y' : 'N') );
    printf("Avg Nodes Traverse per Search -> %lf\n",
        (double)num_comps/num_searches);
    printf("Check for Duplicates -> %c\n",
        ((checkForDuplicates(bst))? 'Y' : 'N') );
}

void bstdb_quit ( void ) {

    tree_delete(bst);

}