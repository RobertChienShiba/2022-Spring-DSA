#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>

int query_sol[1000001] = {};
bool set[1000000] = {};

typedef struct day{
    int day;
    struct day *next;
}Day;

typedef struct shop{
    int parent;
    int rank;
}Shop;

typedef struct command{
    char *command;
    int num1;
    int num2;
    int minus_rank;
    int delete;
    int totalGroup;
}Command;

Shop *genshop(int i){
    Shop *node = (Shop*)malloc(sizeof(Shop));
    assert(node!=NULL);
    node->parent = i-1;
    node->rank = 1;
    return node;
}

Command *gencom(char *command, int num1, int num2, int totalGroup){
    Command *node = (Command*)malloc(sizeof(Command));
    assert(node!=NULL);
    node->command = command;
    node->num1 = num1;
    node->num2 = num2;
    node->minus_rank = -1;
    node->delete = -1;
    node->totalGroup = totalGroup;
    return  node;
}

Day *genday(int day, Day *next){
    Day *node = (Day*)malloc(sizeof(Day));
    assert(node!=NULL);
    node->day = day;
    node->next = next;
    return node;
}

inline void static init(int shop, Shop**dsTree) {
    if (!set[shop-1]) {
        dsTree[shop-1] = genshop(shop);
        set[shop-1] = 1;
    }
}

int find_set(int shop, Shop**ds) {
    if(ds[shop]->parent == shop){
        return shop;
    }
    return find_set(ds[shop]->parent,ds);
}

bool group(Command**node, Shop**ds){
    int shop1 = (*node)->num1;
    int shop2 = (*node)->num2;
    int a = find_set(shop1 - 1, ds), b = find_set(shop2 - 1, ds);
    if(a == b){
        return false;
    }
    if (ds[a]->rank < ds[b]->rank){
        ds[a]->parent = b;
        (*node)->delete = a;
    }
    else if(ds[a]->rank > ds[b]->rank){
        ds[b]->parent = a;
        (*node)->delete = b;
    }
    else{
        ds[b]->parent = a;
        ds[a]->rank++;
        (*node)->delete = b;
        (*node)->minus_rank = a;
    }
    (*node)->totalGroup--;
    return true;
}

void create_dstree(int day, Command**c_tree, Day**day_ll, Shop**dsTree,
    int M,int totalGroup,bool back,int first_boom_day){
    c_tree[day]->totalGroup = totalGroup;
    if(day!=0)
        day++;
    while(day <= M && (c_tree[day]->command)[0] != 'b'){ 
        c_tree[day]->totalGroup = totalGroup;
        if((c_tree[day]->command)[0] == 'm'){
            init(c_tree[day]->num1,dsTree);
            init(c_tree[day]->num2,dsTree);
            group(&c_tree[day],dsTree) == 1 ? totalGroup-- : totalGroup;
        }
        else if((c_tree[day]->command)[0] == 'q'){
            query_sol[day] = c_tree[day]->totalGroup;
        }
        Day *ll = day_ll[day];
        while(ll){
            back = true;
            create_dstree(ll->day, c_tree, day_ll, dsTree,M, 
                c_tree[day]->totalGroup, back, first_boom_day);
            ll = ll->next;
        }
        day++;
    }
    if(back && day != first_boom_day){
        day--;
        while((c_tree[day]->command)[0] != 'b'){
            if(c_tree[day]->delete != -1 && c_tree[day]->minus_rank != -1){
                dsTree[c_tree[day]->delete]->parent = c_tree[day]->delete;
                dsTree[c_tree[day]->minus_rank]->rank--;
            }
            else if(c_tree[day]->delete != -1){
                dsTree[c_tree[day]->delete]->parent = c_tree[day]->delete;
            }
            day--;
        }
    }
}

int main(void){
    int N, M;
    scanf("%d", &N);
    scanf("%d", &M);
    Command *c_tree[M + 1];
    Shop *dsTree[N];
    Day *day_ll[M + 1];
    Day **day_tail = calloc(M + 1, sizeof(Day));
    memset(c_tree, 0, sizeof(c_tree));
    memset(dsTree, 0, sizeof(dsTree));
    memset(day_ll, 0, sizeof(day_ll));
    c_tree[0] = gencom("start", -1, -1, N);
    bool first_boom = true;
    int first_boom_day;
    for(int i = 1;i <= M;i++){
        char *command = malloc(6*sizeof(char));
        int shop1,shop2;
        scanf("%s", command);
        if(command[0] == 'b'){
            scanf("%d", &shop1);
            c_tree[i] = gencom(command, shop1, -1, N);
            if(first_boom){
                first_boom_day = i;
                first_boom = false;
            }
            while ((c_tree[shop1]->command)[0] == 'b'){
                shop1 = c_tree[shop1]->num1;
            }
            Day *newday = genday(i, NULL);
            if(day_ll[shop1] == NULL){
                day_ll[shop1] = newday;
            }
            else{
                day_tail[shop1]->next = newday;
            }
            day_tail[shop1] = newday;
        }
        else if(command[0] == 'm'){
            scanf("%d", &shop1);
            scanf("%d", &shop2);
            c_tree[i] = gencom(command, shop1, shop2, N);
        }
        else{
            c_tree[i] = gencom(command, -1, -1, N);
        }
    }
    free(day_tail); 
    create_dstree(0, c_tree, day_ll, dsTree, M, N, false, first_boom_day);
    for(int i = 0;i <= M;i++){
        if(query_sol[i] != 0){
            printf("%d\n", query_sol[i]);
        }
    }
    return 0;   
}