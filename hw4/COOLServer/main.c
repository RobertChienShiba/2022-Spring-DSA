#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

typedef struct node{
	int size;
	long long second;
	int random;
	long long sum;
    long long max;
	long long se;
	int cn;
	int reverse_tag;
	long long change_tag;
	struct node* left;
	struct node* right;
}Node;

Node* tree = NULL;

int check_size(Node* root){
	if(root){
		return root->size;
	}
	return 0;
}

long long check_max(Node* root){
	if(root){
		return root->max;
	}
	return -1000000000;
} 

long long check_se(Node* root){
	if(root){
		return root->se;
	}
	return -1000000002;
} 

void pushtag(Node *root, long long k);

void Push_Down(Node* root){
	if(root == NULL){
		return;
	}
	if(root->reverse_tag == 1){
		Node* temp = root->left;
		root->left = root->right;
		root->right = temp;
		if(root->left){
			(root->left)->reverse_tag ^= 1; 
		}
		if(root->right){
			(root->right)->reverse_tag ^= 1;
		}
		root->reverse_tag = 0;
	}
	if(root->change_tag != 0){
		pushtag(root,root->change_tag);
		if(root->left){
			if(root->left->change_tag != 0 && root->left->change_tag > root->change_tag){
				root->left->change_tag = 0;
			}
			if(root->left->change_tag == 0){
				(root->left)->change_tag += root->change_tag;
			}
		}
		if(root->right){
			if(root->right->change_tag != 0){
				if(root->right->change_tag > root->change_tag){
					root->right->change_tag = 0;
				}
			}
			if(root->right->change_tag == 0){
				(root->right)->change_tag += root->change_tag;
			}
		}
		root->change_tag = 0;
	}
}

void Pull_Up(Node* root){
	if(root == NULL){
		return;
	}
	root->size = 1;
	root->sum = root->second;
	root->max = root->second;
	root->se = -1;
	root->cn = 1;
	if(root->left){
		Push_Down(root->left);
        if(check_max(root->left) > check_max(root)){
			root->se = check_se(root->left) > check_max(root) ?  check_se(root->left) : check_max(root);
			root->max = check_max(root->left);
			root->cn = root->left->cn;
		}
		else if (check_max(root->left) < check_max(root)){
			root->se = check_se(root) > check_max(root->left) ?  check_se(root) : check_max(root->left);
		}
		else{
			root->se = check_se(root) > check_se(root->left) ? check_se(root) : check_se(root->left);
			root->cn += root->left->cn;
		}
		root->sum += root->left->sum;
		root->size += check_size(root->left);
	}
	if(root->right){
		Push_Down(root->right);\
        if(check_max(root->right) > check_max(root)){
			root->se = check_se(root->right) > check_max(root) ?  check_se(root->right) : check_max(root);
			root->max = check_max(root->right);
			root->cn = root->right->cn;
		}
		else if (check_max(root->right) < check_max(root)){
			root->se = check_se(root) > check_max(root->right) ?  check_se(root) : check_max(root->right);
		}
		else{
			root->se = check_se(root) > check_se(root->right) ? check_se(root) : check_se(root->right);
			root->cn += root->right->cn;
		}
		root->sum += root->right->sum;
		root->size += check_size(root->right);
	}
}

void SPLIT(Node* root, int p, Node **l_root, Node **r_root){
	if(root == NULL){
		*l_root = NULL;
		*r_root = NULL;
	}
	else{
		Push_Down(root);
		if(check_size(root->left) < p){
			*l_root = root;
			SPLIT(root->right, p - check_size(root->left) - 1, &((*l_root)->right), &(*r_root));
		}
		else{
			*r_root = root;
			SPLIT(root->left, p, &(*l_root), &((*r_root)->left));		
		}
		Pull_Up(root);
	}
}

Node* MERGE(Node* a, Node* b){
	if(a == NULL){
		return b;
	}
	if(b == NULL){
		return a; 
	}
	if(b->random < a->random){
		Push_Down(a);
		a->right = MERGE(a->right, b);
		Pull_Up(a);
		return a;
	}
	else{
		Push_Down(b);
		b->left = MERGE(a, b->left);
		Pull_Up(b);
		return b;
	}
}

Node* genNode(long long k){
	Node* root = (Node*)malloc(sizeof(Node)); 
	root->second = k;
	root->size = 1;
	root->sum = k;
    root->max = k;
	root->se = -1;
	root->cn = 1;
	root->random = rand();
	root->reverse_tag = 0;
	root->change_tag = 0;
	root->left = NULL;
	root->right = NULL;
	return root;
}

void INSERT(Node **root, long long k, int p){
	Node* a = NULL;
	Node* b = NULL;
	SPLIT(*root, p, &a, &b);
	*root = MERGE(MERGE(a, genNode(k)), b);
}

void DELETE(int p){
	Node* a = NULL;
	Node* b = NULL;
	Node* c = NULL;
	if(tree == NULL){
		return;
	}
	SPLIT(tree, p, &a, &c);
	SPLIT(a, p - 1, &a, &b);
    //Push_Down(b);
	tree = MERGE(a , c);
}

void REVERSE(int l, int r){
	if(l == r){
		return;
	}
	Node* a = NULL;
	Node* b = NULL;
	Node* c = NULL;
	SPLIT(tree, l - 1, &a, &c);
	SPLIT(c, r - (l - 1), &b, &c);
	b->reverse_tag = 1;
	tree = MERGE(MERGE(a, b), c);
}

void pushtag(Node *root,long long k){
	if(root->max <= k){
		return;
	}
	root->sum -= (root->max - k) * root->cn;
	if(root->second > k){
		root->second = k;
	}
	root->max = k;
}

void MODIFY(Node *root,long long k){
	if(root->max <= k){
		return;
	}
	if(root->se < k){
		pushtag(root,k);
		return ;
	}
	Push_Down(root);
	if(root->left){
		MODIFY(root->left,k);
	}
	if(root->right){
		MODIFY(root->right,k);
	}
	Pull_Up(root);
}

void CHANGE(int l, int r, long long k){
	Node* a = NULL;
	Node* b = NULL;
	Node* c = NULL;
	SPLIT(tree, l - 1, &a, &c);
	SPLIT(c, r - (l - 1), &b, &c);
	b->change_tag += k;
    MODIFY(b,k);
	tree = MERGE(MERGE(a, b), c);	
}

void QUERY_SUM(int l, int r){
	if(tree == NULL){
		return;
	}
	Node* a = NULL;
	Node* b = NULL;
	Node* c = NULL;
	SPLIT(tree, l - 1, &a, &c);
	SPLIT(c, r - (l - 1), &b, &c);
	printf("%lld\n", b->sum);
	tree = MERGE(MERGE(a, b), c);
}

void swap(int **a, int **b){
	int *temp = *a;
	*a = *b;
	*b = temp;
}

void SWAP(int *l, int *r, int *x, int *y){
	if(*r > *y){
		swap(&l, &x);
		swap(&r, &y);
	}
	Node *a = NULL;
	Node *b = NULL;
	Node *c = NULL;
	Node *d = NULL;
	Node *e = NULL;
	SPLIT(tree, *l - 1, &a, &c);
	SPLIT(c, *r - (*l - 1), &b, &c);
	SPLIT(c, *x - *r - 1, &c, &e);
	SPLIT(e, *y - (*x - 1), &d, &e);
	tree = MERGE(MERGE(MERGE(MERGE(a, d), c), b), e);
}

int main(void){
	int N = 0;
	int Q = 0;
	int operation = 0;
	int l, r, p, x, y;
    long long k;

	srand(20000154);
	
	scanf("%d", &N);
	scanf("%d", &Q);
	
	for(int i = 0; i < N; i++){
		scanf("%lld", &k);
		INSERT(&tree, k, i);
	}
	
	for(int i = 0; i < Q; i++){
		scanf("%d", &operation);
        switch (operation){
		case 1 :
			scanf("%d", &p);
			scanf("%lld", &k);
			INSERT(&tree, k, p);
            break;
		case 2 :
			scanf("%d", &p);
			DELETE(p);
            break;
        case 3 :
			scanf("%d", &l);
			scanf("%d", &r);
			REVERSE(l, r);
            break;
        case 4 :
            scanf("%d", &l);
			scanf("%d", &r);
			scanf("%d", &x);
			scanf("%d", &y);
			SWAP(&l, &r, &x, &y);
            break;
		case 5 :
			scanf("%d", &l);
			scanf("%d", &r);
			scanf("%lld", &k);
			CHANGE(l ,r, k);
            break;
		case 6 :
			scanf("%d", &l);
			scanf("%d", &r);
			QUERY_SUM(l, r);
            break;
		    }
        }

	return 0;
}