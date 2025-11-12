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
Groups groups;}
Board;

unsigned R;

#define GRID(row, col) grid[(col) * R + (row)]

void coords_append(Group *group, Coord x){
if(group->size == group->capacity){
group->arr = realloc(group->arr, (group->capacity = group->capacity == 0 ? 4 : group->capacity * 2) * sizeof(Coord));}
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
for(unsigned i = 0; i < 4; ++i){
unsigned nr = r + (const unsigned[]){-1, 0, 1, 0}[i];
unsigned nc = c + (const unsigned[]){0, 1, 0, -1}[i];
if(nr < R && nc < boardPtr->C && !VISITED(nr, nc) && GRID(nr, nc) == color){
dfs(color, nr, nc, group);}}}

for(unsigned r = 0; r < R; ++r){
for(unsigned c = 0; c < boardPtr->C; ++c){
if(!VISITED(r, c)){
Color color = GRID(r, c);
if(color != 0){
Group group = {0};
dfs(color, r, c, &group);
if(boardPtr->groups.size == boardPtr->groups.capacity){
boardPtr->groups.arr = realloc(boardPtr->groups.arr, (boardPtr->groups.capacity = boardPtr->groups.capacity == 0 ? 4 : boardPtr->groups.capacity * 2) * sizeof(Group));}
boardPtr->groups.arr[boardPtr->groups.size++] = group;}
else{
VISITED(r, c) = true;}}}}
free(visited);}

unsigned getLargestGroupSizeIndex(Groups groups){
unsigned largestSize = 0;
unsigned index = -1;
for(unsigned i = 0; i < groups.size; ++i){
Group group = groups.arr[i];
if(largestSize < group.size){
largestSize = group.size;
index = i;}}
return index;}

void makeMove(Board *boardPtr, unsigned i){
char *grid = boardPtr->grid;

if(i < 0 || i >= boardPtr->groups.size){
printf("error: out of bounds index\n");}

Group group = boardPtr->groups.arr[i];
if(group.size < 2){
printf("error: group size must be at least 2\n");}


for(unsigned i = 0; i < group.size; ++i){
unsigned rr = group.arr[i].r;
unsigned cc = group.arr[i].c;




for(unsigned row = rr; row > 0; --row){
if(!(GRID(row, cc) = GRID(row - 1, cc))){
break;}}

GRID(0, cc) = 0;}


unsigned c = 0;
while(c < boardPtr->C){
if(GRID(R - 1, c) == 0){

for(unsigned c2 = c; c2 < boardPtr->C - 1; ++c2){
for(unsigned r = 0; r < R; ++r){
GRID(r, c2) = GRID(r, c2 + 1);}}
--boardPtr->C;}




else{
++c;}}

evaluateGroups(boardPtr);}

typedef struct{
unsigned color;
unsigned length;
unsigned row;
unsigned col;}
Move;

Move makeBestMove(Board *boardPtr, unsigned reasoningDepth, bool inactionIsAcceptable){
if(reasoningDepth == 0){
return (Move) { 0 };}

int index;
if((index = getLargestGroupSizeIndex(boardPtr->groups)) == -1){
return (Move) { 0 };}
int bestMoveRating = boardPtr->groups.arr[index].size;
int bestMoveIndex = -1;

for(unsigned i = 0; i < boardPtr->groups.size; ++i){
if(boardPtr->groups.arr[i].size >= 2){
Board newBoard = *boardPtr;
newBoard.grid = memcpy(malloc(R * boardPtr->C), boardPtr->grid, R * boardPtr->C);
makeMove(&newBoard, i);
makeBestMove(&newBoard, reasoningDepth - 1, true);
int thisMoveRating = 0;
if(bestMoveRating < thisMoveRating){
bestMoveRating = thisMoveRating;
bestMoveIndex = i;}}}

if(bestMoveIndex == -1){
if(inactionIsAcceptable){
return (Move) { 0 };}
bestMoveIndex = getLargestGroupSizeIndex(boardPtr->groups);
if(bestMoveIndex == -1){
printf("IMPOSSIBLE!\n");}}

Group group = boardPtr->groups.arr[bestMoveIndex];
unsigned r0 = group.arr[0].r;
unsigned c0 = group.arr[0].c;
Move bestMove = (Move) { boardPtr->GRID(r0, c0), group.size, r0, c0 };
makeMove(boardPtr, bestMoveIndex);
return bestMove;}

int main(){
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

typedef struct{
Move *arr;
unsigned size;
unsigned capacity;}
Moves;
Moves moves = {0};

unsigned index;
while((index = getLargestGroupSizeIndex(board.groups)) != -1 && board.groups.arr[index].size > 1){
if(moves.size == moves.capacity){
moves.arr = realloc(moves.arr, (moves.capacity = moves.capacity == 0 ? 4 : moves.capacity * 2) * sizeof(Move));}
moves.arr[moves.size++] = makeBestMove(&board, 5, false);}


unsigned totalScore = 0;
for(unsigned i = 0; i < moves.size; ++i){
unsigned tmp = moves.arr[i].length - 1;
totalScore += tmp * tmp;}
printf("%d\n%d\n", totalScore, moves.size);
for(unsigned i = 0; i < moves.size; ++i){
Move m = moves.arr[i];
printf("%d %d %d %d\n", m.color, m.length, R - m.row, m.col + 1);}

return 0;}
