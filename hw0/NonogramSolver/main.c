#include <stdio.h>
#include <stdbool.h>

#define maxsize 100

void printBoard(int n , int m, int board[maxsize][maxsize]){
    for(int i = 0;i < n;i++){
        for(int j = 0;j < m;j++){
        if(board[i][j] == 1)
            printf("o");
        else
            printf("_");
        }
        printf("\n");
    }
}

bool checkHorizontal(int n, int m, int col, int board[maxsize][maxsize],
    int totalBlackList[maxsize][maxsize]){
    int row = (col == 0) ? n-1 : n;
    int b_sequence = 0;
    int testlist[maxsize];
    int v = 0;
    bool black = false;

    for(int j = 0;j < m;j++){
        if(board[row][j] == 1){
            b_sequence++;
            black = true;
            testlist[v] = b_sequence;
            if (totalBlackList[row][0] == 0 || 
                b_sequence > totalBlackList[row][v+1])
                    return false;
        }
        else{
            if(black){
                v++;
                b_sequence=0;
                black=false;
            }
        }
    }

    if(col == 0){
        int len = (black == true) ? v+1 : v;
        if(totalBlackList[row][0] != len)
            return false;
        for(int l = 0;l < len;l++){
            if(totalBlackList[row][l + 1] != testlist[l])
                return false;
            }
    }
    
    return true;
}

bool checkVertical(int n, int m, int total, int board[maxsize][maxsize],
    int totalBlackList[maxsize][maxsize]){
    int col = (total % m - 1 < 0) ? m-1 : total % m - 1;
    int b_sequence = 0;  
    int testlist[maxsize];
    int v=0;
    bool black = false;

    for(int j=0;j<n;j++){
        if(board[j][col] == 1){
            b_sequence++;
            black=true;
            testlist[v] = b_sequence;
            if(totalBlackList[n+col][0] == 0 || 
                b_sequence > totalBlackList[n + col][v + 1])
                return false;
        }
        else{
            if(black){
                v++;
                b_sequence = 0;
                black = false;
            }
        }
    }

    if(total > n * m - m){
        int len = (black == true) ? v+1 : v;
        if(totalBlackList[n + col][0] != len)
            return false;
        for(int l = 0;l < len;l++){
            if(totalBlackList[n + col][l + 1] != testlist[l])
            return false;
        }
    }

    return true;
}

bool recur(int n, int m, int total, int board[maxsize][maxsize],
    int totalBlackList[maxsize][maxsize]){   
    if (total == n * m){
        if(checkHorizontal(n, m, total % m, board,totalBlackList)
            &&(checkVertical(n,m,total,board,totalBlackList))){
                printBoard(n, m, board);
                return true;
        }
        else
            return false;
    }
    else{
        int row = total / m;
        int col = total % m;
        if(total > 0){
            if(!checkHorizontal(row, m, col, board,totalBlackList) || 
            (!checkVertical(n, m, total, board, totalBlackList)))
                return false;
        }
        board[row][col] = 1;
        bool succeed = recur(n, m, total + 1, board, totalBlackList);
        if(succeed)
            return true;
        board[row][col] = 2;
        succeed = recur(n, m, total + 1, board, totalBlackList);
        if(succeed)
            return true;
        board[row][col] = 0;
        return false;
    }
    return true;
}

int main(void){
    int n, m;
    scanf("%d%d", &n, &m);
    int board[maxsize][maxsize];
    int totalBlackList[maxsize][maxsize];

    for(int i = 0;i < (n + m);i++){
        int len;
        scanf("%d", &len);
        totalBlackList[i][0] = len;
        for(int j = 1;j <= len;j++){
            int count;
            scanf("%d", &count);
            totalBlackList[i][j] = count;
        }
    }
    recur(n, m, 0, board, totalBlackList);
    return 0;
}