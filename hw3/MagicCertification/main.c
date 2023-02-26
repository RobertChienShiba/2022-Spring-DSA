#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

#define maxStr 10000000

void reverse(char *string, int size, char *reverseString){
    for(int i = size - 1;i >= 0;i--){
        reverseString[size - i - 1] = string[i]; 
    }
    reverseString[size] = '\0';
}

void kmpMatch(int *pi, int size, char *string){
    int k = -1;
    for(int i = 1;i < size;i++){
        while(k > -1 && string[k + 1] != string[i]){
            k = pi[k];
        }
        if(string[k + 1] == string[i]){
            k++;
        }
        pi[i] = k;
    }
}

void shortestPalindrome(char *string, char *reverseString, int len){
    int checkTime = 1;
    int lastAdd_len = 0;
    char lastAdd[len * 2 + 1];
    char tmp[len + 1];
    memset(tmp, 0, sizeof(tmp));
    memset(lastAdd,0, sizeof(lastAdd));
    while(checkTime < 3){
        strcat(string, " ");
        strcat(string, reverseString);
        int catLen = strlen(string);
        int pi[catLen];
        memset(pi, -1, sizeof(pi));
        kmpMatch(pi, catLen, string);
        int addCharLen= strlen(reverseString) - (pi[catLen - 1] + 1);
        char addChar[addCharLen + len + 1];
        memset(addChar, 0, sizeof(addChar));
        strncpy(addChar, reverseString, addCharLen);
        addChar[addCharLen] = '\0';
        string[len]='\0';
        strcat(addChar, string);
        if(checkTime == 2){
            if(addCharLen == lastAdd_len){
                printf("%d\n", addCharLen);
                printf("%s\n", lastAdd);
                printf("%s\n", addChar);
            }
            else{
                printf("%d\n", addCharLen < lastAdd_len ? addCharLen : lastAdd_len);
                printf("%s\n", addCharLen < lastAdd_len ? addChar : lastAdd);
            }
            break;
        }
        lastAdd_len = addCharLen;
        strcpy(lastAdd, addChar);
        strcpy(tmp, string);
        strcpy(string, reverseString);
        strcpy(reverseString, tmp);
        memset(addChar, 0, sizeof(addChar));
        checkTime++;
    } 
}

int main(void){
    char string[2 *maxStr +2];
    scanf("%s", string);
    int len = strlen(string);
    char reverseString[len + 1];
    reverse(string, len, reverseString);
    if(strcmp(string, reverseString) != 0){
        shortestPalindrome(string, reverseString, len);
    }
    else{
        printf("%d\n", 0);
        printf("%s\n", string);
    }
    return 0;
}