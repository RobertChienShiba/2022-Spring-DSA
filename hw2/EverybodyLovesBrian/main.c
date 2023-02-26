#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include "price.h"

typedef struct  node{
    unsigned long long param1;
    unsigned long long param2;
}Node;

Node *genNode(unsigned long long param1,unsigned long long param2){
    Node *node = (Node*)malloc(sizeof(Node));
    assert(node!=NULL);
    node->param1 = param1;
    node->param2 = param2;
    return  node;
}

void swap(Node **stockArray, unsigned long long idx1, unsigned long long idx2){
	Node *tmp = stockArray[idx1];
	stockArray[idx1] = stockArray[idx2];
	stockArray[idx2] = tmp;
}

long long compareMin(Node **stockArray, unsigned long long left, 
    unsigned long long right, unsigned long long root, unsigned long long size){
	long long smaller = -1;
	if ((left < size && (stockArray[left])->param2 < (stockArray[root])->param2)){
		if ((right < size && (stockArray[right])->param2 < (stockArray[left])->param2)){
			swap(stockArray, right, root);
			smaller = right;
		}
		else{
			swap(stockArray, left, root);
			smaller = left;
		}
	}
	else if ((right < size && (stockArray[right])->param2 < (stockArray[root])->param2)){
		swap(stockArray, right, root);
		smaller = right;
	}
	return smaller;
}

long long compareMax( Node **stockArray, 
    unsigned long long left, unsigned long long right, unsigned long long root, 
    unsigned long long size){
	long long larger = -1;
	if ((left < size && (stockArray[left])->param2 > (stockArray[root])->param2)){
		if ((right < size && (stockArray[right])->param2 > (stockArray[left])->param2)){
			swap(stockArray, right, root);
			larger = right;
		}
		else{
			swap(stockArray, left, root);
			larger = left;
		}
	}
	else if ((right < size && (stockArray[right])->param2 > (stockArray[root])->param2)){
		swap(stockArray, right, root);
		larger = right;
	}
	return larger;
}

void minHeapify(Node **stockArray, unsigned long long size, unsigned long long root){
	unsigned long long left = 2 * root + 1;
	unsigned long long right = 2 * root + 2;
	long long smaller = compareMin(stockArray, left, right, root, size);
	if ((smaller != -1)){
		minHeapify(stockArray, size, smaller);
	}
}

void maxHeapify(Node **stockArray, unsigned long long size, unsigned long long root){
	unsigned long long left = 2 * root + 1;
	unsigned long long right = 2 * root + 2;
	long long larger = compareMax(stockArray, left, right, root, size);
	if ((larger != -1)){
		maxHeapify(stockArray, size, larger);
	}
}

void createMinHeap(Node **stockArray, unsigned long long size){
	long long i = (size / 2) - 1;
	while (i >= 0){
		minHeapify(stockArray, size, i);
		i--;
	}
}

void createMaxHeap(Node **stockArray, unsigned long long size){
	long long i = (size / 2) - 1;
	while (i >= 0){
		maxHeapify(stockArray, size, i);
		i--;
	}
}

void heapSortMax(Node **h_array,unsigned long long size){
    unsigned long long rank = 0;
    for(long long i = size - 1;i >= 0;i--){
        swap(h_array, i, 0);
        maxHeapify(h_array, i, 0);
    }
}

unsigned long long binarySearchSort(unsigned long long stockPrice,
    Node **stockSort,const unsigned long long maxQuery){
    unsigned long long l = 0;
    unsigned long long r = maxQuery-1;
    unsigned long long m;
    if(stockPrice >= (stockSort[maxQuery-1])->param2)
        return maxQuery;
    else if(stockPrice < (stockSort[0])->param2)
        return 0;
    while(l <= r){
        m = (l + r) / 2;
        if(stockPrice == (stockSort[m])->param2){
            while((stockSort[m])->param2 == stockPrice && m < maxQuery)
                m++;
            return m;
        }
        else if(stockPrice > (stockSort[m])->param2)
            l=m+1;
        else
            r=m-1;
    }
    return l;
}

long long kthSweetWithExtra(const unsigned long long extraStock,
    const unsigned long long kthSweet,
    unsigned long long N,Node **stockSort, unsigned long long **stockRankPtr){
    unsigned long long l = 1;
    unsigned long long r = kthSweet;
    unsigned long long m;
    while(l <= r){
        m = (l + r) / 2;
        unsigned long long stockPrice = price(extraStock, m);
        unsigned long long sortRank = binarySearchSort(stockPrice, stockSort, kthSweet);
        *stockRankPtr = &sortRank;
        unsigned long long lowBound = m > N ? m - N + 1 : 1;
        if(sortRank + lowBound <= kthSweet && kthSweet <= sortRank + m + N - 1)
            return m;
        else if(sortRank + m + N - 1 < kthSweet)
            l=m+1;
        else if(sortRank + lowBound > kthSweet)
            r=m-1;
    }
    if(r != 0 && r != m)
        return r+1;
    else
        return r;
}

 unsigned long long kthSweet(Node **stockSort, unsigned long long sizeSort, 
    Node **extra, unsigned long long sizeExtra,
    const unsigned long long kth){
	if (sizeSort > sizeExtra)
		return kthSweet(extra, sizeExtra, stockSort, sizeSort, kth);
	if (sizeSort == 0)
		return extra[kth - 1]->param2;
	if (kth == 1)
		return (stockSort[0])->param2<extra[0]->param2 ? 
            stockSort[0]->param2 : extra[0]->param2;
	unsigned long long i = sizeSort>(kth / 2) ? kth / 2 : sizeSort;
    unsigned long long j = sizeExtra>kth / 2 ? kth / 2:sizeExtra;
	if (stockSort[i - 1]->param2 > extra[j - 1]->param2)
		return kthSweet(stockSort, sizeSort, extra + j, sizeExtra - j, kth - j);
	else
		return kthSweet(stockSort + i, sizeSort - i, extra, sizeExtra, kth - i);
}

int main(void){
    int A,Q,N;
    scanf("%d", &A);
    scanf("%d", &Q);
    scanf("%d", &N);
    Node *stockArray[N * A];
    Node *s_hash[A];
    Node *s_query[Q];
    unsigned long long maxQuery = 0;
    memset(stockArray, 0, sizeof(stockArray));
    memset(s_hash, 0, sizeof(s_hash));
    memset(s_query, 0, sizeof(s_query));
    for(int i = 1;i <= A;i++){
        unsigned long long stockId;
        scanf("%llu", &stockId);
        for(int j = 1;j <= N;j++){
            unsigned long long stockPrice = price(stockId, j);
            Node *stock = genNode(i-1, stockPrice);
            stockArray[(i-1) * N + (j - 1)] = stock;
        }
        s_hash[i - 1] = genNode(stockId, N);
    }
    for(int m = 0;m < Q;m++){
        unsigned long long extraStock;
        unsigned long long kth;
        scanf("%llu", &extraStock);
        scanf("%llu", &kth);
        s_query[m] = genNode(extraStock, kth);
        maxQuery = kth>maxQuery ? kth : maxQuery;
    }
    Node **stockSort = calloc(maxQuery, sizeof(Node));
    createMinHeap(stockArray, N * A);
    for(unsigned long long n = 0;n < maxQuery;n++){
        Node *minNode = stockArray[0];
        stockSort[n] = minNode;
        ((s_hash[minNode->param1])->param2)++;
        unsigned long long stockIdber = (s_hash[minNode->param1])->param1;
        unsigned long long t_day = ((s_hash[minNode->param1])->param2);
        unsigned long long stockPrice = price(stockIdber, t_day);
        Node *newNode = genNode(minNode->param1,stockPrice);
        stockArray[0] = newNode;
        minHeapify(stockArray, N * A, 0);
    }
    for(int h = 0;h < Q;h++){
        unsigned long long extraStock = (s_query[h])->param1;
        unsigned long long kth = (s_query[h])->param2;
        unsigned long long *stockRankPtr = NULL;;
        bool done = false;
        if(extraStock != 0){
            unsigned long long possibleDay = kthSweetWithExtra(
                extraStock, kth, N, stockSort, &stockRankPtr);
            unsigned long long stockUpperBound = *stockRankPtr;
            unsigned long long stockLowerBound= *stockRankPtr;
            if(possibleDay != 0){
                unsigned long long extraUpBound = possibleDay;
                unsigned long long extraLowBound = possibleDay;
                unsigned long long lowBound = extraUpBound > N ? 
                    extraUpBound - N + 1 : 1;
                unsigned long long stockPrice ;
                int multiple = 1;
                unsigned long long newUp;
                unsigned long long newLow;
                while (stockUpperBound + lowBound < kth){
                    extraUpBound = extraUpBound + multiple * N;
                    lowBound = extraUpBound > N ? extraUpBound - N + 1 : 1;
                    stockPrice = price(extraStock, extraUpBound);
                    newUp = binarySearchSort(stockPrice, stockSort, kth);
                    if(newUp - stockUpperBound < 0.5 * N){
                        if(multiple <= 2){
                            multiple += 1;
                        }
                    }
                    stockUpperBound = newUp;
                }
                multiple = 1;
                while(stockLowerBound + extraLowBound + N - 1 > kth){
                    if(extraLowBound < multiple * N){
                        extraLowBound = 1;
                        break;
                    }
                    extraLowBound = extraLowBound - multiple * N;
                    stockPrice = price(extraStock, extraLowBound);
                    newLow = binarySearchSort(stockPrice, stockSort, kth);
                    if(stockLowerBound - newLow < 0.5 * N){
                        if(multiple <= 2)
                            multiple += 1;
                    }
                    stockLowerBound = newLow;
                }
                lowBound = extraLowBound > N ? extraLowBound - N + 1 : 1;
                unsigned long long size = 0;
                Node **possibleRange = calloc((extraUpBound + N - lowBound),
                    sizeof(Node));
                for(unsigned long long t = lowBound;t < extraUpBound + N;t++){
                    Node *node = genNode(A + 1,price(extraStock, t));
                    possibleRange[size] = node;
                    size++;
                }
                createMaxHeap(possibleRange, size);
                heapSortMax(possibleRange, size);
                unsigned long long kth_ans = kthSweet(stockSort, kth, possibleRange,
                    size, kth - lowBound + 1);
                printf("%llu\n", kth_ans);
                free(possibleRange);
            }
            else{
                unsigned long long size = 0;
                unsigned long long lowBound = 1;
                Node **possibleRange = calloc((1 + N - lowBound),sizeof(Node));
                for(unsigned long long t = lowBound;t < 1 + N;t++){
                        Node *node = genNode(A + 1,price(extraStock, t));
                        possibleRange[size] = node;
                        size++;
                }
                createMaxHeap(possibleRange, size);
                heapSortMax(possibleRange, size);
                unsigned long long kth_ans = kthSweet(stockSort, kth, possibleRange, size, kth);
                printf("%llu\n", kth_ans);
                free(possibleRange);
            }
        }
        else{printf("%llu\n",(stockSort[kth - 1])->param2);}
    }
    return 0;
}