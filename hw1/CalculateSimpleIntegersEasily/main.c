#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

#define max_input 100000
#define STRLEN 21

void push(char *stack, char sign, long long *top){
    *top += 1;
    stack[*top] = sign;
}

void int_push(long long *stack,long long sign,long long *top){
     *top += 1;
    stack[*top] = sign;
}

char pop(char *stack, long long *top){
    char sign = stack[*top];
    *top -= 1;
    return sign;
}

long long int_pop(long long *stack,long long *top){
    long long sign = stack[*top];
    *top -= 1;
    return sign;
}

int priority_sign(char sign){
    switch (sign){
        case '+': case'-':
            return 1 ;
        case '*': case '/': case'%':
            return 3 ;
        case'(':
            return 5;
        default:
            break;
    }
}

int priority_stack(char *stack, long long *top){
    switch (stack[*top]){
        case '+': case'-':
            return 1;
        case '*': case '/': case'%':
            return 3;
        case '(':
            return 0;
        default:
            return -1;
    }
}

long long cal_postfix(char postfix[max_input][STRLEN], long long postfix_pos){
    long long sol_stack[max_input];
    long long top = -1;
    long long *toptr = &top;
    long long tmpint1;
    long long tmpint2;
    for(long long i = 0;i < postfix_pos;i++){
        if(strlen(postfix[i]) > 1)
            int_push(sol_stack, (long long)atoll(postfix[i]), toptr);
        else if(isdigit(postfix[i][0]))
            int_push(sol_stack, (long long)(postfix[i][0])-48, toptr);
        else{switch (postfix[i][0]){
                case '+':
                    tmpint1 = int_pop(sol_stack, toptr);
                    tmpint2 = int_pop(sol_stack, toptr);
                    int_push(sol_stack, tmpint1 + tmpint2, toptr);
                    break;
                case '-':
                    tmpint1 = int_pop(sol_stack, toptr);
                    tmpint2 = int_pop(sol_stack, toptr);
                    int_push(sol_stack, tmpint2 - tmpint1, toptr);
                    break;
                case '*':
                    tmpint1 = int_pop(sol_stack, toptr);
                    tmpint2 = int_pop(sol_stack, toptr);
                    int_push(sol_stack, tmpint1 * tmpint2, toptr);
                    break;
                case '/':
                    tmpint1 = int_pop(sol_stack, toptr);
                    tmpint2 = int_pop(sol_stack, toptr);
                    if((tmpint2 < 0 || tmpint1 < 0) && (tmpint2 % tmpint1 != 0) && 
                        !(tmpint2 < 0 && tmpint1 < 0))
                        int_push(sol_stack,(tmpint2/tmpint1)-1,toptr);
                    else
                        int_push(sol_stack,tmpint2/tmpint1,toptr);
                    break;
                case '%':
                    tmpint1 = int_pop(sol_stack, toptr);
                    tmpint2 = int_pop(sol_stack, toptr);
                    int_push(sol_stack, tmpint2 % tmpint1, toptr);
                    break;
                default:
                    break;
                }
            }
        memset(postfix[i], 0, STRLEN);
        }
    return(int_pop(sol_stack, toptr));
}

void infix_postfix(char *infix, char postfix[max_input][STRLEN], char *stack){
    long long length = strlen(infix);
    long long  infix_pos = 0;
    long long  postfix_pos = 0;
    long long  digitcount = 0;
    bool digit = false;
    char tmp;
    long long  top = -1;
    long long  *toptr = &top;
    while (infix_pos < length){
        char sign = infix[infix_pos];
        switch(sign){
            case '(':
                push(stack, sign, toptr);
                break;
            case '+' : case '-': case '*': case '/': case'%':
                if(digit){
                    postfix_pos++;
                    digit = false;
                    digitcount = 0;
                }
                while(priority_stack(stack, toptr) >= priority_sign(sign)){
                    tmp = pop(stack, toptr);
                    postfix[postfix_pos][0] = tmp;
                    postfix_pos++;
                };
                push(stack, sign, toptr);
                break;
            case ')':
                if(digit){
                    postfix_pos++;
                    digit = false;
                    digitcount = 0;
                }
                tmp = pop(stack, toptr);
                while (tmp != '('){
                    postfix[postfix_pos][0] = tmp;
                    postfix_pos++;
                    tmp = pop(stack, toptr);
                }
                break;
            case '=':
                if(digit){
                    postfix_pos++;
                    digit = false;
                    digitcount = 0;
                }
                while(priority_stack(stack, toptr) != -1){
                    tmp = pop(stack, toptr);
                    postfix[postfix_pos][0] = tmp;
                    postfix_pos++;
                }
                long long sol = cal_postfix(postfix, postfix_pos);
                printf("Print: %lld\n", sol);
                postfix_pos = 0;
                sprintf(postfix[postfix_pos], "%lld", sol);
                postfix_pos++;
                break;
            default:
                digit ? digitcount++ : digitcount;
                postfix[postfix_pos][digitcount] = sign;
                digit = true;
                break;
            }
        infix_pos++;
    }
}

int main(void){
    char infix[max_input];
    char stack[max_input];
    char postfix[max_input][STRLEN];
    scanf("%s", infix);
    infix_postfix(infix, postfix, stack);
    return 0;
}