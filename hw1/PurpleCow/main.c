#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#define max_group 100
#define max_bathroom 100000

typedef struct head_tail{
    struct node *head;
    struct node *tail;
}Group;

typedef struct node{
    int id;
    int group;
    struct node *prev;
    struct node *next;
}Node;

Node *genNode(int id, int group, Node *prev, Node *next){
    Node *node = (Node*)malloc(sizeof(Node));
    assert(node!=NULL);
    node->id = id;
    node->group = group;
    node->prev = prev;
    node->next = next;
    return  node;
}

Group *genheadtail(Node *head, Node *tail){
    Group *ht = (Group*)malloc(sizeof(Group));
    assert(ht!=NULL);
    ht->head = head;
    ht->tail = tail;
    return ht;
}

void enter_command(Node **group_seq, Node **group_tail, int id, int group,
    Group *ht[max_group]){
    Node *NewNode = genNode(id,group,NULL,NULL);
    Node *NewGroup = genNode(group,group,NULL,NULL);
    if(*group_seq==NULL){
        *group_seq=NewGroup;
        ht[NewNode->group] = genheadtail(NewNode,NewNode);
        *group_tail = NewGroup;
        return;
    }
    if(ht[NewNode->group] != NULL){
        if(ht[NewNode->group]->tail->next == NULL){
            NewNode->next = ht[NewNode->group]->tail->next; 
            ht[NewNode->group]->tail->next = NewNode;
            NewNode->prev = ht[NewNode->group]->tail;
        }
        else{
            NewNode->next = ht[NewNode->group]->tail->prev; 
            ht[NewNode->group]->tail->prev = NewNode;
            NewNode->prev = ht[NewNode->group]->tail;
        }
        ht[NewNode->group]->tail = NewNode;
    }
    else{
        ht[NewNode->group] = genheadtail(NewNode,NewNode);
        (*group_tail)->next = NewGroup;
        NewGroup->prev = *group_tail;
        *group_tail = NewGroup;
    }
}

void go_command(Node **group_seq, Group *ht[max_group], Node **group_tail){
    int initial_group = (*group_seq)->id;
    Node *HeadNode = ht[initial_group]->head;
    if(ht[initial_group]->head->prev == NULL)
        ht[initial_group]->head = ht[initial_group]->head->next;
    else
        ht[initial_group]->head = ht[initial_group]->head->prev;
    if(ht[initial_group]->head == NULL){
        ht[initial_group] = NULL;
        (*group_seq) = (*group_seq)->next;
        if(*group_seq == NULL){
            *group_tail = NULL;
            return;
        }
        (*group_seq)->prev = NULL;
        return;
    }
    if(ht[initial_group]->head->prev == HeadNode)
        ht[initial_group]->head->prev = NULL;
    else
        ht[initial_group]->head->next = NULL;
}

void leave_command(Node **group_seq, Group *ht[max_group], Node **group_tail){
    Node *LastNode = ht[(*group_tail)->id]->tail;
    if(ht[(*group_tail)->id]->tail->next == NULL)
        ht[(*group_tail)->id]->tail = ht[(*group_tail)->id]->tail->prev;
    else
        ht[(*group_tail)->id]->tail = ht[(*group_tail)->id]->tail->next;
    if(ht[(*group_tail)->id]->tail == NULL){
        ht[(*group_tail)->id] = NULL;
        *group_tail = (*group_tail)->prev;
        if(*group_tail == NULL){
            *group_seq=NULL;
            return;
        }
        (*group_tail)->next = NULL;
        return;
    }
    if(ht[(*group_tail)->id]->tail->next == LastNode)
        ht[(*group_tail)->id]->tail->next = NULL;
    else
        ht[(*group_tail)->id]->tail->prev = NULL;
}

void close_command(Node **l_group_seq, Node **e_group_seq, Group *l_ht[max_group],
    Group *e_ht[max_group], Node **l_group_tail, Node **e_group_tail){
    Node *g_head;
    Node *g_tail;
    while(*l_group_seq != NULL){
        g_head = l_ht[(*l_group_tail)->id]->head;
        g_tail = l_ht[(*l_group_tail)->id]->tail;
        if(*e_group_seq != NULL){
            if(e_ht[g_tail->group] != NULL){
                if(e_ht[g_tail->group]->tail->next == NULL&&g_tail->prev == NULL){
                    e_ht[g_tail->group]->tail->next = g_tail;
                    g_tail->prev = e_ht[g_tail->group]->tail;
                    e_ht[g_tail->group]->tail = g_head;
                }
                else if(e_ht[g_tail->group]->tail->next == NULL&&g_tail->next == NULL){
                    e_ht[g_tail->group]->tail->next = g_tail;
                    g_tail->next = e_ht[g_tail->group]->tail;
                    e_ht[g_tail->group]->tail = g_head;
                }
                else if(e_ht[g_tail->group]->tail->prev == NULL&&g_tail->prev == NULL){
                    e_ht[g_tail->group]->tail->prev = g_tail;
                    g_tail->prev = e_ht[g_tail->group]->tail;
                    e_ht[g_tail->group]->tail = g_head;
                }
                else{
                    e_ht[g_tail->group]->tail->prev = g_tail;
                    g_tail->next = e_ht[g_tail->group]->tail;
                    e_ht[g_tail->group]->tail = g_head;
                }
            }
            else{
                e_ht[g_tail->group] = genheadtail(g_tail, g_head);
                (*e_group_tail)->next = genNode(g_tail->group, g_tail->group, NULL, NULL);
                (*e_group_tail)->next->prev = *e_group_tail;
                *e_group_tail = (*e_group_tail)->next;
            }
        }
        else{
            *e_group_seq=genNode(g_tail->group, g_tail->group, NULL, NULL);
            e_ht[g_tail->group] = genheadtail(g_tail, g_head);
            *e_group_tail = *e_group_seq;
        }
        l_ht[(*l_group_tail)->id] = NULL;
        (*l_group_tail) = (*l_group_tail)->prev;
        if(*l_group_tail != NULL)
            (*l_group_tail)->next = NULL;
        else
            *l_group_seq=NULL;
    }
}

Node *concat_group(Node *group_seq, Group *ht[max_group]){
    Node *total_dll = NULL;
    Node *tmp;
    while (group_seq != NULL){
        if(total_dll == NULL){
            total_dll = ht[group_seq->id]->head;
            tmp=ht[group_seq->id]->tail;
        }
        else{
            if(tmp->next == NULL && ht[group_seq->id]->head->prev == NULL){
                tmp->next = ht[group_seq->id]->head;
                tmp->next->prev = tmp;
            }
            else if(tmp->next == NULL && ht[group_seq->id]->head->next == NULL){
                tmp->next = ht[group_seq->id]->head;
                tmp->next->next = tmp;
            }
            else if(tmp->prev == NULL && ht[group_seq->id]->head->next == NULL){
                tmp->prev = ht[group_seq->id]->head;
                tmp->prev->next = tmp;
            }
            else{
                tmp->prev = ht[group_seq->id]->head;
                tmp->prev->prev = tmp;
            }
            tmp = ht[group_seq->id]->tail;
        }
        group_seq = group_seq->next;
    }
    return total_dll;
}

void printlist(Node *group_seq[max_bathroom], Group *ht[max_bathroom][max_group],
    int k_bathroom, int k_group){
    Node *start;
    bool reverse;
    for(int i = 0;i < k_bathroom;i++){
        reverse = false;
        Node *store_last_id  = concat_group(group_seq[i], ht[0] + k_group * i);
        if (store_last_id == NULL){
            printf("\n");
            continue;
        }
        if(store_last_id->next == NULL){
            start = store_last_id->prev;
            reverse = true;}
        else
            start = store_last_id->next;
        printf("%d ",store_last_id->id);
        while(start != NULL){
            printf("%d ",start->id);
            if(reverse){
                if(start->prev == store_last_id){
                    start = start->next;
                    reverse = false;
                }
                else
                    start=start->prev;
                store_last_id = store_last_id->prev;
            }
            else{
                if(start->next == store_last_id){
                    start = start->prev;
                    reverse = true;
                }
                else
                    start=start->next;
                store_last_id = store_last_id->next;
            }  
        }
        printf("\n");
    }
}

void circular_bathroom(Node **destination_bath, int bathroom){
    Node *start;
    for(int id = 0;id < bathroom;id++){
        if(id == 0){
            *destination_bath = (Node*)malloc(sizeof(Node));
            assert(*destination_bath!=NULL);
            (*destination_bath)->id = id;
            (*destination_bath)->group = -1;
            (*destination_bath)->prev = *destination_bath;
            (*destination_bath)->next = *destination_bath;
            start = *destination_bath;
            continue;
        }
        Node *last = *destination_bath;
        *destination_bath++;
        *destination_bath = genNode(id, -1, last, start);
        start->prev = *destination_bath;
        last->next = *destination_bath;
    }
}

int find_update_bathroom(Node **l_bath){
    if((*l_bath)->prev == *l_bath && (*l_bath)->next==*l_bath){
        (*l_bath)=NULL;
        return -1;
    }
    int destination_bathroom = (*l_bath)->prev->id;
    (*l_bath)->next->prev = (*l_bath)->prev;
    (*l_bath)->prev->next = (*l_bath)->next;
    (*l_bath)=NULL;
    return destination_bathroom;
}   

int main(void){
    int k_bathroom;
    int k_solution;
    int k_group;
    scanf("%d", &k_bathroom);
    scanf("%d", &k_solution);
    scanf("%d", &k_group);
    Node *destination_bath[k_bathroom], *group_seq[k_bathroom], *group_tail[k_bathroom];
    Group *store_headtail[k_bathroom][k_group];
    memset(group_seq, 0, sizeof(group_seq));
    memset(store_headtail, 0, sizeof(store_headtail));
    memset(destination_bath, 0, sizeof(destination_bath));
    memset(group_tail, 0, sizeof(group_tail));
    circular_bathroom(&destination_bath[0], k_bathroom);
    char command[6];
    int id, group, bathroom;
    for(int i = 0;i < k_solution;i++){
        scanf("%s", command);
        switch (command[0]){
        case 'e':
            scanf("%d %d %d", &group, &id, &bathroom);
            enter_command(&(group_seq[bathroom]), &(group_tail[bathroom]), id,
                group, store_headtail[bathroom]);
            break;
        case 'g':
            scanf("%d",&bathroom);
            go_command(&(group_seq[bathroom]), store_headtail[bathroom],
                &(group_tail[bathroom]));
            break;
        case 'l':
            scanf("%d",&bathroom);
            leave_command(&(group_seq[bathroom]), store_headtail[bathroom],
                &(group_tail[bathroom]));
            break;
        case 'c':
            scanf("%d",&bathroom);
            int destination_id = find_update_bathroom(&(destination_bath[bathroom]));
            if(destination_id == -1)
                break;
            close_command(&(group_seq[bathroom]), &(group_seq[destination_id]),
                store_headtail[bathroom], store_headtail[destination_id],
            &(group_tail[bathroom]), &(group_tail[destination_id]));
            break;
        default:
            break;
        }
    }
    printlist(group_seq,store_headtail,k_bathroom,k_group);
    return 0;
}