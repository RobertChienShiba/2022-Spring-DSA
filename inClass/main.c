#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
typedef struct disjointSet{
    // TODO: Determine fields to use by your method
    const char *name;
    int rank;
    int parent;
} DisjointSet;

DisjointSet *ds[1 << 24];
bool set[1 << 24] = {};

int c2i(const char c) { 
    if ('0' <= c && c <= '9') return c - '0';
    else if ('a' <= c && c <= 'z') return c - 'a' + 10;
    else if ('A' <= c && c <= 'Z') return c - 'A' + 36;
    return -1;
}

int hash(const char* s) {  
    int ret = 0;
    int mask = (1 << 24) - 1;
    int len = strlen(s);
    for (int i = 0; i < len; i++)
        ret = mask & (ret << 4) | c2i(s[i]);
    return ret;
}

void makeset(const char* s,int i){
    // TODO: Initialize a set with hash value
    DisjointSet *new = malloc(sizeof(DisjointSet));
    new->parent = i;
    new->rank = 1;
    ds[i] = new;
}

inline int static init(const char* s) {
    int i = hash(s);
    if (!set[i]) {
        makeset(s,i);
        set[i] = 1;
    }
    return i;
}

int find_set(int hash) {
    // TODO: Implement your find algorithm here
    
    if(ds[hash]->parent!=hash){
        ds[hash]->parent = find_set(ds[ds[hash]->parent]->parent);
    }
    return ds[hash]->parent;
}

void group(int name1_h, int name2_h) {
    int a = find_set(name1_h), b = find_set(name2_h);
    // TODO: Implement your union algorithm here
    if(a==b)
        return;
    if (ds[a]->rank < ds[b]->rank){
        ds[a]->parent = b;
    }
    else if(ds[a]->rank > ds[b]->rank){
        ds[b]->parent = a;
    }
    else{
        ds[b]->parent = a;
        ds[a]->rank++; 
    }
}

bool same_set(int name1_h, int name2_h) {
    // TODO: Implement your algorithm here
    if (find_set(ds[name1_h]->parent) == find_set(ds[name2_h]->parent))
        return true;
    else
        return false;
}

int main(void) {
    // TODO: Implement your algorithm here
    int N;
    scanf("%d", &N);
    for(int i = 0;i < N;i++){
        char command[6];
        char *name1 = malloc(13*sizeof(char));
        char *name2 = malloc(13*sizeof(char));
        scanf("%s", command);
        scanf("%s", name1);
        scanf("%s", name2);
        int name1_h = init (name1);
        int name2_h = init (name2);
        if(command[0] == 'g'){
            group(name1_h, name2_h);
        }
        else{
            if(same_set(name1_h, name2_h))
                printf("Positive\n");
            else
                printf("Negative\n");
        }
    }
    return 0;
}