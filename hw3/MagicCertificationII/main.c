#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h> 
#define max_bound UINT64_MAX

typedef struct string{
    unsigned long long h_value;
    unsigned long long index;
}String;

String *genStr(unsigned long long h_value,unsigned long long index){
    String *string = (String*)malloc(sizeof(String));
    assert(string!=NULL);
    string->h_value = h_value;
    string->index = index;
    return  string;
}

unsigned long long Rabin_Karp(unsigned long long l,char *string,int d,unsigned long long q,unsigned long long *d_mod){
    unsigned long long h_value = 0;
    unsigned long long h = 1;
    for(unsigned long long i=0;i<l;i++,d_mod++){
        h_value = (d*h_value+(string[i]-32))%q;
        if(*d_mod==0){
            d_mod[0] = h;
            h*=d;
            if(h>q){
                h%=q;
            }
        }
    }
    return h_value;
}

unsigned long long adjusted_h_value(String *h_array,char *string,unsigned long long space_idx,
    unsigned long long l,unsigned long long d,unsigned long long q,unsigned long long *d_mod){
    long long old_h_value = h_array->h_value;
    int string_id = string[space_idx]-32;
    unsigned long long h = d_mod[l-space_idx-1];
    long long tmp = (string_id*h)%q;
    while (old_h_value<tmp){
        tmp-=q;
    }
    unsigned long long new_h_value = old_h_value-tmp;
    return new_h_value;
}

void merge(String **h_array, unsigned long long start, unsigned long long mid,unsigned long long end){
    unsigned long long l_size = mid- start + 1;
    unsigned long long r_size = end - mid;
    String *L[l_size], *R[r_size];
    for (unsigned long long i = 0; i < l_size; i++)
        L[i] = h_array[start + i];
    for (unsigned long long j = 0; j < r_size; j++)
        R[j] = h_array[mid + 1 + j];
    unsigned long long l_idx = 0; 
    unsigned long long r_idx = 0;
    unsigned long long m_idx = start; 
    while (l_idx < l_size && r_idx < r_size) {
        if (L[l_idx]->h_value <= R[r_idx]->h_value) {
            h_array[m_idx] = L[l_idx];
            l_idx++;
        }
        else {
            h_array[m_idx] = R[r_idx];
            r_idx++;
        }
        m_idx++;
    }
    while (l_idx < l_size) {
        h_array[m_idx] = L[l_idx];
        l_idx++;
        m_idx++;
    }
    while (r_idx < r_size) {
        h_array[m_idx] = R[r_idx];
        r_idx++;
        m_idx++;
    }
}
  
void mergeSort(String **h_array, unsigned long long start, unsigned long long end){
    if (start < end) {
        unsigned long long mid = start + (end - start) / 2;
        mergeSort(h_array, start, mid);
        mergeSort(h_array, mid + 1, end);
        merge(h_array, start, mid, end);
    }
}

long long count_duplicated(unsigned long long k,String **h_array,bool flag){
    unsigned long long last = 0;
    unsigned long long similar_pair = 0;
    for(unsigned long long i=0;i<k;i++){
        unsigned long long count = 1;
        while(i<k&&i>0&&h_array[i]->h_value == last){
            count++;
            last = h_array[i]->h_value;
            if(!flag){
                printf("Yes\n");
                printf("%llu %llu",h_array[i-1]->index,h_array[i]->index);
                return -1;
            }
            i++;
        }
        similar_pair+=(((count)*(count-1))/2);
        if(i==k){break;}
        last = h_array[i]->h_value;
    }
    return similar_pair;
}

int main(void){
    unsigned long long k,l;
    int flag;
    scanf("%llu",&k);
    scanf("%llu",&l);
    scanf("%d",&flag);
    String *h_array[k];
    char magic_string[k][l+1];
    String *partial_h_array[k];
    unsigned long long d_mod[l];
    int d = 94;
    unsigned long long q = max_bound/d;
    memset(d_mod,0,sizeof(d_mod));
    if(k==1){printf("No\n");return 0;}
    for(unsigned long long i=0;i<k;i++){
        scanf("%s",magic_string[i]);
        h_array[i]=genStr(0,i);
        partial_h_array[i]=genStr(0,i);
    }
    unsigned long long total_pair = 0;
    long long duplicated = 0;
    long long pair = 0;
    for(unsigned long long i=0;i<=l;i++){
        for(unsigned long long j=0;j<k;j++){
            if(i==0){
                
                h_array[j]->h_value = Rabin_Karp(l,magic_string[j],d,q,d_mod);
            }
            else{
                if(l==1){break;}
                partial_h_array[j] = genStr(adjusted_h_value(h_array[j],magic_string[j],i-1,l,d,q,d_mod),j);
            }
        }
        if(i!=0){
            mergeSort(partial_h_array,0,k-1);
            pair = count_duplicated(k,partial_h_array,flag);
            if(pair==-1){break;}
            total_pair+=pair;
            memset(partial_h_array,0,sizeof(partial_h_array));
        }
    }
    if(pair!=-1)
        if(total_pair!=0 || l==1){
            mergeSort(h_array,0,k-1);
            if(l==1&&flag==0){
                duplicated=count_duplicated(k,h_array,0);
                if(duplicated!=-1){printf("No\n");}
                return 0;
            }
            if(l!=1){
                duplicated = count_duplicated(k,h_array,1);
            }
            printf("Yes\n");
            total_pair-=(l-1)*duplicated;
            printf("%llu\n",total_pair);
        }
        else{
            printf("No\n");
        }
    return 0;
}