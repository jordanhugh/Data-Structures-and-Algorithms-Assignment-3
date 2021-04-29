#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Tree_Node{
        char data;
        struct Tree_Node *left;
        struct Tree_Node *right;
        int height;
}Tree_Node;

int max(int a, int b); 
int height(Tree_Node* node);
int balance(Tree_Node* node);
void leftRotate(Tree_Node** root);
void leftRightRotate(Tree_Node** root);
void rightLeftRotate(Tree_Node** root);
void rightRotate(Tree_Node** root);

/*
int recursHeight(Tree_Node* node);
int recursBalance(Tree_Node* node);
bool checkIfBalanced(Tree_Node* root);
bool checkBelow(Tree_Node* node, char data);
bool checkForDuplicates(Tree_Node* root);
*/

Tree_Node* node_init(char data);
void tree_insert(Tree_Node** root, char data);
Tree_Node* tree_search(Tree_Node* root, char data);
void tree_print_sorted(Tree_Node* root);
void tree_delete(Tree_Node* root);

int main(){
    Tree_Node* root;
    root = (Tree_Node*)malloc(sizeof(Tree_Node));
    root = NULL;

    char sequence[] = "FLOCCINAUCINIHILIPILIFICATION";
    int length = 0;

    printf("ADDING: ");
    while (sequence[length] != '\0'){
        printf("%c ", sequence[length]);
        tree_insert(&root, sequence[length]);

        length++;
    }
    printf("\nSORTED: ");
    tree_print_sorted(root);

    //Randomly chooses a character between 'A' and 'Z'
    char letter = (rand()%26) + 65;
    Tree_Node* node = tree_search(root, letter); //Searches for Node in Tree
    if (node != NULL){
        printf("\n%c in tree", letter);
    }
    else{
        printf("\n%c not in tree", letter);
    }

    letter = (rand()%26) + 65;
    node = tree_search(root, letter);
    if (node != NULL){
        printf("\n%c in tree", letter);
    }
    else{
        printf("\n%c not in tree", letter);
    }

    letter = (rand()%26) + 65;
    node = tree_search(root, letter);
    if (node != NULL){
        printf("\n%c in tree", letter);
    }
    else{
        printf("\n%c not in tree", letter);
    }

    tree_delete(root);
    return 0;
}


int max(int a, int b){ //Returns the maximum of two numbers
    return (a > b)? a : b;
}


int height(Tree_Node* node){ //Returns the height of a node
    if (node == NULL) 
        return 0; 
        
    return (node->height); 
}


int balance(Tree_Node* node){ //Returns the balance of a node
    if (node == NULL){
        return 0;
    }
    else{
        return(height(node->left) - height(node->right));
    }
}

/*
int recursHeight(Tree_Node* node){
    if (node == NULL) 
        return 0; 
        
    return (1 + max(recursHeight(node->left), recursHeight(node->right))); 
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
    if(root == NULL) 
    return true;  
    
    int bal = recursBalance(root);
    if( abs(bal) < 2 && checkIfBalanced(root->left) && checkIfBalanced(root->right)){
        return true;
    }

    return false;
} 

bool checkBelow(Tree_Node* node, char data){
    if(node == NULL){
        return false;
    }

    if(data != node->data && !checkBelow(node->left, data) && !checkBelow(node->right, data)){
        return false;
    }
    
    return true;
}

bool checkForDuplicates(Tree_Node* root){
    if(root == NULL){
        return false;
    }

    if(!checkBelow(root->left, root->data) && !checkForDuplicates(root->left) && 
       !checkBelow(root->right, root->data) && !checkForDuplicates(root->right)){

        return false;
    }

    return true;
}
*/


void leftRotate(Tree_Node** root){ //Rotates a subtree left around a pivot
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

void leftRightRotate(Tree_Node** root){
    leftRotate(&((*root)->left));
    rightRotate(root);
}

void rightLeftRotate(Tree_Node** root){
    rightRotate(&((*root)->right));
    leftRotate(root);
}


void rightRotate(Tree_Node** root){ //Rotates a subtree right around a pivot 
    Tree_Node* pivot = (*root)->left;

    //Perform Rotation
    (*root)->left = pivot->right;
    pivot->right = *root;

    // Update Heights
    (*root)->height = 1 + max(height((*root)->left), height((*root)->right));
    pivot->height = 1 + max(height(pivot->left), height(pivot->right));

    // Update Root
    *root = pivot;
}

Tree_Node* node_init(char data){
    Tree_Node* node;

    // Allocate memory for the new list node
    node = (Tree_Node*)malloc(sizeof(Tree_Node));
    
    // Malloc can fail, so make sure the node is pointing to something before we try
    // writing to it.
    if (node) {  // Store data in the list node
        node->data = data;
        node->left = NULL;
        node->right = NULL;
        node->height = 1;
    }
    else{ // If calloc failed, release any memory that was allocated and report an error by returning NULL
        free(node);
    }
    return node;
}

void tree_insert(Tree_Node** root, char data){
    if (*root == NULL){ //If cell is empty insert the node here
        *root = node_init(data);
        return;
    }

    if (data <= (*root)->data){
        tree_insert(&((*root)->left), data); 
    }
    else {
        tree_insert(&((*root)->right), data);
    }

    //Update height of the node to calculate the balance factor of the node on the next recursion
    (*root)->height = 1 + max(height((*root)->left), height((*root)->right));
    int bal = balance(*root);

    //If the node is unbalanced then rotate the node
    //Right Rotate
    if (bal > 1 && data < (*root)->left->data){
        rightRotate(root);
    }
    //Left Right Rotate
    else if (bal > 1 && data > (*root)->left->data){
        leftRightRotate(root);
    }
    //Right Left Rotate
    else if (bal < -1 && data < (*root)->right->data){
        rightLeftRotate(root);
    }
    //Left Rotate
    else if (bal < -1 && data > (*root)->right->data){
        leftRotate(root);
    }
}

Tree_Node* tree_search(Tree_Node* root, char data){
    Tree_Node* curr = root;
	
	while (curr != NULL) { //Re
        if (data < curr->data){ 
            curr = curr->left;
        }
        else if (data > curr->data) {
            curr = curr->right;
        }
        else {
            return curr;
        }
    }
    return NULL;
}

void tree_print_sorted(Tree_Node* root){
    if (root != NULL){
        tree_print_sorted(root->left);
        printf("%c ", root->data);
        tree_print_sorted(root->right);
    }
}

void tree_delete(Tree_Node* root){

    if (root != NULL){
        tree_delete(root->left);
        tree_delete(root->right);
        free(root);
    }

}