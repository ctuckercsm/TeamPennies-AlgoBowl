#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct
	unsigned r, c
Coord

typedef struct
	Coord *arr
	unsigned size, cap
Group

typedef struct
	Group *arr
	unsigned size, cap
Groups

typedef struct
	unsigned C
	char *grid
	Groups groups
Board

unsigned R

#define GRID(row, col) grid[(col) * R + (row)]

void coords_free(Group *v)
	free(v->arr)
	v->arr = NULL
	v->size = v->cap = 0
void coords_append(Group *v, Coord x)
	if v->size == v->cap
		unsigned newcap = v->cap == 0 ? 4 : v->cap * 2
		v->arr = realloc(v->arr, newcap * sizeof(Coord))
		v->cap = newcap
	v->arr[v->size++] = x

void evaluateGroups(Board *boardPtr)
	char *grid = boardPtr->grid

	if boardPtr->groups.arr
		for unsigned i = 0; i < boardPtr->groups.size; ++i
			coords_free(&boardPtr->groups.arr[i])
		free(boardPtr->groups.arr)
	boardPtr->groups.arr = NULL
	boardPtr->groups.size = boardPtr->groups.cap = 0

	bool *visited = calloc(R * boardPtr->C, sizeof(bool))
	#define VISITED(row, col) (visited[(col) * R + (row)])

	void dfs(unsigned color, unsigned r, unsigned c, Group *group)
		VISITED(r, c) = true
		coords_append(group, (Coord){r, c})
		for unsigned k = 0; k < 4; ++k
			unsigned nr = r + (const unsigned[]){-1, 0, 1, 0}[k]
			unsigned nc = c + (const unsigned[]){0, 1, 0, -1}[k]
			if nr < R && nc < boardPtr->C && !VISITED(nr, nc) && GRID(nr, nc) == color
				dfs(color, nr, nc, group)
	
	for unsigned r = 0; r < R; ++r
		for unsigned c = 0; c < boardPtr->C; ++c
			if !VISITED(r, c)
				unsigned color = GRID(r, c)
				if color != 0
					Group group = {0}
					dfs(color, r, c, &group)
					if boardPtr->groups.size == boardPtr->groups.cap
						unsigned newcap = boardPtr->groups.cap == 0 ? 4 : boardPtr->groups.cap * 2
						Group *tmp = realloc(boardPtr->groups.arr, newcap * sizeof(Group))
						boardPtr->groups.arr = tmp
						boardPtr->groups.cap = newcap
					boardPtr->groups.arr[boardPtr->groups.size++] = group
				else
					VISITED(r, c) = true
	free(visited)

void makeMove(Board *boardPtr, unsigned i)
	char *grid = boardPtr->grid

	if i < 0 || i >= boardPtr->groups.size
		printf("error: out of bounds index\n")

	Group grp = boardPtr->groups.arr[i]
	if grp.size < 2
		printf("error: group size must be at least 2\n")

	// When you remove a group, all squares above fall down to fill the gaps
	for unsigned k = 0; k < grp.size; ++k
		unsigned rr = grp.arr[k].r
		unsigned cc = grp.arr[k].c

		// char *dst = grid + (cc * R + rr)
		// memmove(dst, dst - 1, rr)

		for unsigned row = rr; row > 0; --row
			if !(GRID(row, cc) = GRID(row - 1, cc))
				break

		GRID(0, cc) = 0

	// If an entire column becomes empty, all columns to the right shift left to fill the gap
	unsigned c = 0
	while c < boardPtr->C
		if GRID(R - 1, c) == 0

			for unsigned c2 = c; c2 < boardPtr->C - 1; ++c2
				for unsigned r = 0; r < R; ++r
					GRID(r, c2) = GRID(r, c2 + 1)
			--boardPtr->C

			// char *dst = grid + (c * R)
			// memmove(dst, dst + R, R * (--boardPtr->C - c))

		else
			++c

	evaluateGroups(boardPtr)

unsigned main()
	Board board
	scanf("%u %u", &R, &board.C)
	while getchar() != '\n'
	board.grid = malloc(R * board.C)
	for unsigned row = 0; row != R; ++row
		char line[board.C + 2]
		fgets(line, board.C + 2, stdin)
		for unsigned col = 0; col != board.C; ++col
			board.GRID(row, col) = line[col] - '0'
	evaluateGroups(&board)

	typedef struct
		unsigned color
		unsigned length
		unsigned row
		unsigned col
	Move
	typedef struct
		Move *arr
		unsigned size
		unsigned cap
	Moves
	Moves moves = {0}

	for
		bool noValidMoveExists = true;
		for unsigned i = 0; i < board.groups.size; ++i
			Group grp = board.groups.arr[i]
			if grp.size > 1
				noValidMoveExists = false

				unsigned r0 = grp.arr[0].r
				unsigned c0 = grp.arr[0].c
				if moves.size == moves.cap
					unsigned newcap = moves.cap == 0 ? 4 : moves.cap * 2
					moves.arr = realloc(moves.arr, newcap * sizeof(Move))
					moves.cap = newcap
				moves.arr[moves.size++] = (Move) { board.GRID(r0, c0), grp.size, r0, c0 }

				makeMove(&board, i)
				break
		if noValidMoveExists
			break

	unsigned totalScore = 0
	for unsigned i = 0; i < moves.size; ++i
		unsigned l = moves.arr[i].length - 1
		totalScore += l * l
	printf("%d\n", totalScore)
	printf("%d\n", moves.size)
	for unsigned i = 0; i < moves.size; ++i
		Move m = moves.arr[i]
		printf("%d %d %d %d\n", m.color, m.length, R - m.row, m.col + 1)
	
	return 0