#line 0"main.c"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef unsigned char Color;

typedef struct{
unsigned r, c;}
Coord;

typedef struct{
Coord *arr;
unsigned size, capacity;}
Group;

typedef struct{
Group *arr;
unsigned size, capacity;}
Groups;

typedef struct{
unsigned C;
Color *grid;
Groups groups;
unsigned largestGroupSizeIndex;}
Board;

unsigned R;

#define GRID(row, col) grid[(col) * R + (row)]

void coords_append(Group *group, Coord x){
if(group->size == group->capacity){
group->arr = realloc(group->arr, (group->capacity = group->capacity ? group->capacity * 2 : 1) * sizeof(Coord));}
group->arr[group->size++] = x;}

void freeGroups(Groups groups){
for(unsigned i = 0; i != groups.size; ++i){
free(groups.arr[i].arr);}
free(groups.arr);}

void evaluateGroups(Board *boardPtr){
char *grid = boardPtr->grid;
boardPtr->groups.arr = NULL;
boardPtr->groups.size = boardPtr->groups.capacity = 0;

bool *visited = calloc(R * boardPtr->C, sizeof(bool));
#define VISITED(row, col) visited[(col) * R + (row)]

void dfs(Color color, unsigned r, unsigned c, Group *group){
VISITED(r, c) = true;
coords_append(group, (Coord){r, c});
{
unsigned nr = r - 1;
if((nr < R && !VISITED(nr, c) && GRID(nr, c) == color)){
dfs(color, nr, c, group);}}
{
unsigned nr = r + 1;
if((nr < R && !VISITED(nr, c) && GRID(nr, c) == color)){
dfs(color, nr, c, group);}}
{
unsigned nc = c + 1;
if((nc < boardPtr->C && !VISITED(r, nc) && GRID(r, nc) == color)){
dfs(color, r, nc, group);}}
{
unsigned nc = c - 1;
if((nc < boardPtr->C && !VISITED(r, nc) && GRID(r, nc) == color)){
dfs(color, r, nc, group);}}}

unsigned largestSize = 0;
boardPtr->largestGroupSizeIndex = -1;
for(unsigned r = 0; r < R; ++r){
for(unsigned c = 0; c < boardPtr->C; ++c){
if(!VISITED(r, c)){
Color color = GRID(r, c);
if(color != 0){
Group group = {0};
dfs(color, r, c, &group);
if(boardPtr->groups.size == boardPtr->groups.capacity){
boardPtr->groups.arr = realloc(boardPtr->groups.arr, (boardPtr->groups.capacity = boardPtr->groups.capacity ? boardPtr->groups.capacity * 2 : 4) * sizeof(Group));}
boardPtr->groups.arr[boardPtr->groups.size] = group;
if(largestSize < group.size){
largestSize = group.size;
boardPtr->largestGroupSizeIndex = boardPtr->groups.size;}
++boardPtr->groups.size;}
else{
VISITED(r, c) = true;}}}}
free(visited);}

void makeMove(Board *boardPtr, unsigned i){
char *grid = boardPtr->grid;

if(i < 0 || i >= boardPtr->groups.size){
printf("error: out of bounds index\n");}

Group group = boardPtr->groups.arr[i];
if(group.size < 2){
printf("error: group size must be at least 2\n");}


bool *col_touched = calloc(boardPtr->C, sizeof(bool));
for(unsigned i = 0; i < group.size; ++i){
unsigned rr = group.arr[i].r;
unsigned cc = group.arr[i].c;
GRID(rr, cc) = 0;
col_touched[cc] = true;}

for(unsigned cc = 0; cc < boardPtr->C; ++cc){
if(!col_touched[cc]){
continue;}

int write_idx = R - 1;

for(int row = (int)R - 1; row >= 0; --row){
int val;
if(val = GRID((unsigned)row, cc)){

GRID((unsigned)write_idx, cc) = val;
--write_idx;}}

for(int row = write_idx; row >= 0; --row){
GRID(row, cc) = 0;}}
free(col_touched);


unsigned c = 0;
while(c < boardPtr->C){
if(GRID(R - 1, c) == 0){
char *dst = grid + (c * R);
memmove(dst, dst + R, (--boardPtr->C - c) * R);}
else{
++c;}}}

typedef struct{
unsigned color;
unsigned length;
unsigned row;
unsigned col;}
Move;
typedef struct{
Move *arr;
unsigned size;
unsigned capacity;}
Moves;
Moves moves = {0};

void makeBestMove(Board *boardPtr, unsigned reasoningDepth, bool inactionIsAcceptable){
if(reasoningDepth == 0 || boardPtr->largestGroupSizeIndex == -1){
return;}

int currentLargestGroupSize = boardPtr->groups.arr[boardPtr->largestGroupSizeIndex].size;
int bestMoveIndex = -1;

for(unsigned i = 0; i < boardPtr->groups.size; ++i){
if(boardPtr->groups.arr[i].size >= 2){
Board newBoard = *boardPtr;
newBoard.grid = memcpy(malloc(R * boardPtr->C), boardPtr->grid, R * boardPtr->C);
makeMove(&newBoard, i);
evaluateGroups(&newBoard);
makeBestMove(&newBoard, reasoningDepth - 1, true);
int potentialLargestGroupSize;
if(newBoard.largestGroupSizeIndex == -1){
potentialLargestGroupSize = 0;}
else{
potentialLargestGroupSize = newBoard.groups.arr[newBoard.largestGroupSizeIndex].size;}

if(currentLargestGroupSize < potentialLargestGroupSize){
currentLargestGroupSize = potentialLargestGroupSize;
bestMoveIndex = i;}
free(newBoard.grid);
freeGroups(newBoard.groups);}}

if(bestMoveIndex == -1){
if(inactionIsAcceptable){
return;}
bestMoveIndex = boardPtr->largestGroupSizeIndex;
if(bestMoveIndex == -1){
printf("IMPOSSIBLE!\n");}}

if(!inactionIsAcceptable){
Group group = boardPtr->groups.arr[bestMoveIndex];
unsigned r0 = group.arr[0].r;
unsigned c0 = group.arr[0].c;
if(moves.size == moves.capacity){
moves.arr = realloc(moves.arr, (moves.capacity = moves.capacity ? moves.capacity * 2 : 4) * sizeof(Move));}
moves.arr[moves.size++] = (Move) { boardPtr->GRID(r0, c0), group.size, r0, c0 };}
makeMove(boardPtr, bestMoveIndex);
freeGroups(boardPtr->groups);
evaluateGroups(boardPtr);
return;}

int main(int argc, char *argv[]){
FILE *f = fopen(argv[1], "w");
if(!f){
perror("fopen");
return EXIT_FAILURE;}

Board board;
scanf("%u %u", &R, &board.C);
while(getchar() != '\n');
board.grid = malloc(R * board.C);
for(unsigned row = 0; row != R; ++row){
char line[board.C + 2];
fgets(line, board.C + 2, stdin);
for(unsigned col = 0; col != board.C; ++col){
board.GRID(row, col) = line[col] - '0';}}

evaluateGroups(&board);








int moveNumber = 0;
while(board.largestGroupSizeIndex != -1 && board.groups.arr[board.largestGroupSizeIndex].size > 1){
makeBestMove(&board, 10, false);
if(moveNumber % 10 == 0){
printf("move: %d\n", ++moveNumber);}}


unsigned totalScore = 0;
for(unsigned i = 0; i < moves.size; ++i){
unsigned tmp = moves.arr[i].length - 1;
totalScore += tmp * tmp;}
fprintf(f, "%d\n%d\n", totalScore, moves.size);
for(unsigned i = 0; i < moves.size; ++i){
Move m = moves.arr[i];
fprintf(f, "%d %d %d %d\n", m.color, m.length, R - m.row, m.col + 1);}

for(unsigned row = 0; row != R; ++row){
for(unsigned col = 0; col != board.C; ++col){
printf("%d, ", (int)board.GRID(row, col));}
printf("\n");}

return 0;}
