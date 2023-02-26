#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

typedef struct  node{
    int id;
    struct node *next;
}Node;

Node *genNode(int id, Node *next){
    Node *node = (Node*)malloc(sizeof(Node));
    assert(node!=NULL);
    node->id = id;
    node->next = next;
    return  node;
}

void createAdj(Node **city1, Node **city2, Node **city1Tail, Node **city2Tail,
    int city1Id, int city2Id){
    Node *newnode1 = genNode(city1Id, NULL);
    Node *newnode2 = genNode(city2Id, NULL);
    if(*city1 == NULL)
        *city1 = newnode2;
    else
        (*city1Tail)->next = newnode2;
    *city1Tail = newnode2;
    if(*city2 == NULL)
        *city2 = newnode1;
    else
        (*city2Tail)->next = newnode1;
    *city2Tail = newnode1;
}

bool create_qtb_main(Node **adj, int last, int index,int *queryArray,
    int R, int S, int N){
    if(S == R){
        for(int i = 0;i < N;i++)
            queryArray[i] = S;
        return false;
    }
    Node *ll = adj[index - 1];
    if(ll->next == NULL && last == ll->id)
        return false;
    while(ll){
        if(ll->id == last){
            ll = ll->next;
            continue;
        }
        if(ll->id == R){
            queryArray[(ll->id) - 1] = ll->id;
            return true; 
        }
        if(create_qtb_main(adj, index, ll->id, queryArray, R, S, N)){
            queryArray[(ll->id) - 1] = ll->id;
            return true;
        }
        ll = ll->next;
    }
}

void create_qtb_branch(Node **adj, int index, int *queryArray, int root){
    Node *ll = adj[index];
    while(ll){
        if(queryArray[(ll->id) - 1] != 0){
            ll = ll->next;
            continue;
        }
        queryArray[(ll->id) - 1] = root;
        create_qtb_branch(adj, (ll->id) - 1, queryArray, root);
        ll = ll->next;
    }
}

int main(void){
    int N, Q, S, R;
    scanf("%d", &N);
    scanf("%d", &Q);
    scanf("%d", &S);
    scanf("%d", &R);
    Node *adjList[N];
    Node *cityTail[N];
    memset(adjList, 0, sizeof(adjList));
    memset(cityTail, 0, sizeof(cityTail));
    int quertAns[N];
    memset(quertAns, 0, sizeof(quertAns));
    for(int i = 0;i < N - 1;i++){
        int city1, city2;
        scanf("%d", &city1);
        scanf("%d", &city2);
        createAdj(&adjList[city1 - 1], &adjList[city2 - 1], 
            &cityTail[city1 - 1], &cityTail[city2 - 1], city1, city2);
    }
    quertAns[S - 1] = S;
    if(create_qtb_main(adjList, -1, S, quertAns, R, S, N)){
        for(int j = 0;j < N;j++){
            if(quertAns[j] != 0){
                create_qtb_branch(adjList, j, quertAns, j + 1);
            }
        }
    }
    for(int m = 0;m < Q;m++){
        int tmp;
        scanf("%d", &tmp);
        printf("%d\n", quertAns[tmp - 1]);
    }
    return 0;
}