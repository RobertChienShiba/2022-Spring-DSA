#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define most_digits 300

void swap_array(int *ptr1, int *ptr2,int k, int shrink){
 for(int i = k - 1;i > -1 + shrink;i--){
	int temp = ptr1[i];
    ptr1[i] = ptr2[i];
    ptr2[i] = temp;
    }
}

void input_number(char *charint, int countint, int *bigint){
    for(int i = 0;i < countint;i++){
        bigint[i]=(int)charint[i] - 48;
    }
}

bool noteq_zero(int *bigint2, int k, int shrink){
    for(int i = k - 1;i > -1 + shrink;i--){
        if (bigint2[i]!=0)
            return true;
        }  
    return false;}

void minus(int *big, int *small, int k, int *result, int shrink){
    bool math_carry = false;
    for(int m = k - 1;m > -1 + shrink;m--){
        if (math_carry){
            if(big[m] == 0){
                big[m] = 9;
                math_carry = true;
            }
            else{
                big[m] = big[m] - 1;
                math_carry = false;
            }
        }
        if (big[m] >= small[m])
            result[m] = (big[m]-small[m]);
        else{
            result[m] = big[m] + 10 - small[m];
            math_carry=true;
        }
    }
}

void printgcd(int *resultptr, int countint, int shrink){
    // int m=0;
    // while (resultptr[m]==0){m++;}
    for(int n = shrink;n < countint;n++){
        printf("%d",resultptr[n]);
    }
    printf("\n");
}

int change_digits(int *check_digits){
    int m = 0;
    while (check_digits[m]==0){m++;}
    return m;
}

void fill_zero(int countint, int *intptr, int diff_digit){
    for(int i = countint - 1;i > -1;i--){
        int idx = i - diff_digit;
        if(idx < 0)
            intptr[i] = 0;
        else
            intptr[i]=intptr[idx];
    }
}

bool array_compare(int countint, int *intptr1, int *intptr2, int shrink){
    for(int j = shrink;j < countint;j++){
        if (intptr1[j] != 0 || intptr2[j] != 0){
            if (intptr1[j]>intptr2[j])
                return false;
            else if (intptr1[j]<intptr2[j])
                return true;
        }
    }
}

int main(void){
    char charint1[most_digits];
    char charint2[most_digits];
    scanf("%s",charint1);
    scanf("%s",charint2);
    int countint1 = strlen(charint1);
    int countint2 = strlen(charint2);
    int ptr1[most_digits], ptr2[most_digits];
    int *intptr1 = ptr1,*intptr2=ptr2;
    bool swap = false;
    int m;
    input_number(charint1,countint1,intptr1);
    input_number(charint2,countint2,intptr2);
    if (countint1 < countint2)
        swap = true;
    int countint = (countint1>=countint2) ? countint1 : countint2;
    int diff_digit = abs(countint1-countint2);
    if(diff_digit != 0){
        if(swap)
            fill_zero(countint,intptr1,diff_digit);
        else
            fill_zero(countint,intptr2,diff_digit);
        }
    else
        swap=array_compare(countint,intptr1,intptr2,0); 
    do{
    m = (swap ? change_digits(intptr2) : change_digits(intptr1));
    int result[most_digits];
    if (swap){
        minus(intptr2,intptr1,countint,result,m);
        swap_array(intptr2,result,countint,m);
    }
    else{
        minus(intptr1,intptr2,countint,result,m);
        swap_array(intptr1,result,countint,m);
    }
    swap = array_compare(countint,intptr1,intptr2,m);
    }
    while (noteq_zero(swap ? intptr1 : intptr2, countint, m));
    if(swap)
        printgcd(intptr2,countint,m);
    else
        printgcd(intptr1,countint,m);

    return 0;
}

