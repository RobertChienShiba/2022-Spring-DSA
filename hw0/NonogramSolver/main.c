#include <stdio.h>
#include <stdbool.h>

#define maxsize 100

void printboard(int n , int m, int board[maxsize][maxsize]){
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

bool validboard_hor(int n, int m, int col, int board[maxsize][maxsize],
    int total_blacklist[maxsize][maxsize]){
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
            if (total_blacklist[row][0] == 0 || 
                b_sequence > total_blacklist[row][v+1])
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
        if(total_blacklist[row][0] != len)
            return false;
        for(int l = 0;l < len;l++){
            if(total_blacklist[row][l + 1] != testlist[l])
                return false;
            }
    }
    
    return true;
}

bool validboard_ver(int n, int m, int total, int board[maxsize][maxsize],
    int total_blacklist[maxsize][maxsize]){
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
            if(total_blacklist[n+col][0] == 0 || 
                b_sequence > total_blacklist[n + col][v + 1])
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
        if(total_blacklist[n + col][0] != len)
            return false;
        for(int l = 0;l < len;l++){
            if(total_blacklist[n + col][l + 1] != testlist[l])
            return false;
        }
    }

    return true;
}

bool recur(int n, int m, int total, int board[maxsize][maxsize],
    int total_blacklist[maxsize][maxsize]){   
    if (total == n * m){
        if(validboard_hor(n, m, total % m, board,total_blacklist)
            &&(validboard_ver(n,m,total,board,total_blacklist))){
                printboard(n,m,board);
                return true;
        }
        else
            return false;
    }
    else{
        int row = total / m;
        int col = total % m;
        if(total > 0){
            if(!validboard_hor(row, m, col, board,total_blacklist) || 
            (!validboard_ver(n,m,total,board,total_blacklist)))
                return false;
        }
        board[row][col] = 1;
        bool succeed = recur(n, m, total + 1, board, total_blacklist);
        if(succeed)
            return true;
        board[row][col] = 2;
        succeed = recur(n, m, total + 1, board, total_blacklist);
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
    int total_blacklist[maxsize][maxsize];

    for(int i = 0;i < (n + m);i++){
        int len;
        scanf("%d", &len);
        total_blacklist[i][0] = len;
        for(int j = 1;j <= len;j++){
            int count;
            scanf("%d", &count);
            total_blacklist[i][j] = count;
        }
    }
    recur(n, m, 0, board, total_blacklist);
    return 0;
}