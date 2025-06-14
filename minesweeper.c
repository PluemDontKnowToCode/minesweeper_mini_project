#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

int height;
int width;

void PrintArray(char array[height][width]);
void RevealBomb(char map[height][width],char tempMap[height][width]);
void SaveMap(char *filename, char Map[height][width]);
void GenerateMap(int mine,char map[height][width]);
void Reveal(int row, int col, char map[height][width], char tempMap[height][width], bool visited[height][width]);
char GetSelect(int row, int column,char map[height][width]);

int main(int argc, char *argv[]) 
{
    height = atoi(argv[1]);
    width = atoi(argv[2]);
    int mine = atoi(argv[3]);

    char map[height][width], tempMap[height][width];
    GenerateMap(mine, map);
    GenerateMap(0, tempMap);

    bool isAlive = true;

    while(isAlive)
    {
        //command s = select, f = flag
        char command;
        int row, column;

        printf("\nInput Commnad Row And Column : ");
        scanf("%c %d %d",&command ,&row ,&column);

        if(row >= height)
        {
            printf("row is more than %d", height);
        }
        else if(column >= width)
        {
            printf("column is more than %d", width);
        }

        if(command == 's') 
        {
            if(map[row][column] == '*') 
            {
                isAlive = false;
                RevealBomb(map, tempMap);  // show all bombs
                PrintArray(tempMap);
                break;
            } 
            else 
            {
                bool visited[height][width];
                memset(visited, false, sizeof(visited));
                Reveal(row, column, map, tempMap, visited);
            }
        }
        else if(command == 'f')
        {
            if(map[row][column] == '*')
            {
                mine--;
                if(mine == 0)
                    break;
            }
        }
        PrintArray(tempMap);
    }
    if(!isAlive)
        printf("You lose");
    else
        printf("You Win");
    return 0;
}
void PrintArray(char array[height][width]) 
{
    for (int i = 0; i < height; i++) 
    {
        for (int j = 0; j < width; j++)
            printf("%c ", array[i][j]);
        printf("\n");
    }
}
void RevealBomb(char map[height][width],char tempMap[height][width])
{
    for (int i = 0; i < height; i++) 
        for (int j = 0; j < width; j++) 
            if(map[i][j] == '*')
                tempMap[i][j] = '*';
}
void SaveMap(char *filename, char Map[height][width])
{
    FILE *fp;
    fp = fopen(filename,"w");

    for (int i = 0; i < height; i++) 
    {
        for(int j = 0;j < width;j++)
            fprintf(fp,"%c", Map[i][j]);
        fprintf(fp,"\n");
    }
    fclose(fp);
}
void Reveal(int row, int col, char map[height][width], char tempMap[height][width], bool visited[height][width]) 
{
    if (row < 0 || row >= height || col < 0 || col >= width)
        return;

    if (visited[row][col])
        return;

    visited[row][col] = true;

    char value = GetSelect(row, col, map);
    tempMap[row][col] = value;

    if (value == '0') 
    {
        for (int i = -1; i <= 1; i++) 
        {
            for (int j = -1; j <= 1; j++) 
            {
                if (i != 0 || j != 0)
                    Reveal(row + i, col + j, map, tempMap, visited);
            }
        }
    }
}
char GetSelect(int row, int column,char map[height][width])
{
    if(map[row][column] == '*')
    {
        return '*';
    }
    int op = 0;
    for(int i = -1;i <= 1;i++)
    {
        for(int j = -1;j <= 1;j++)
        {
            if(i == 0 && j == 0)
                continue;
            int newX = row + i;
            int newY = column + j;

            if(newX < 0 || newX >= height || newY < 0 || newY >= width)
            {
                continue;
            }
            if(map[newX][newY] == '*')
            {
                op++;
            }
        }
    }
    return op + 48;
}
void GenerateMap(int mine,char map[height][width])
{
    for (int i = 0; i < height; i++) 
        for (int j = 0; j < width; j++) 
        {
            map[i][j] = '.';
        }

    int placed = 0;
    while (placed < mine) 
    {
        int r = rand() % height;
        int c = rand() % width;
        if (map[r][c] != '*') 
        {
            map[r][c] = '*';
            placed++;
        }
    }
    char *filename = (mine == 0) ? "TempMap.txt" : "Map.txt";
    SaveMap(filename, map);
}


