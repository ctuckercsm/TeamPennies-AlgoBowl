#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

int R, C
char *grid
#define GRID(row, col) grid[(col) * R + (row)]

typedef struct
	int r, c
Coord
typedef struct
	Coord *data
	int size, cap
Coords

void coords_free(Coords *v)
	free(v->data)
	v->data = NULL
	v->size = v->cap = 0
void coords_append(Coords *v, Coord x)
	if v->size == v->cap
		int newcap = v->cap == 0 ? 4 : v->cap * 2
		Coord *tmp = realloc(v->data, newcap * sizeof(Coord))
		v->data = tmp
		v->cap = newcap
	v->data[v->size++] = x

typedef struct
	Coords *data
	int size, cap
Groups

Groups groups

void groups_free()
	if groups.data
		for int i = 0; i < groups.size; ++i
			coords_free(&groups.data[i])
		free(groups.data)
	groups.data = NULL
	groups.size = groups.cap = 0
void groups_append(Coords grp)
	if groups.size == groups.cap
		int newcap = groups.cap == 0 ? 4 : groups.cap * 2
		Coords *tmp = realloc(groups.data, newcap * sizeof(Coords))
		groups.data = tmp
		groups.cap = newcap
	groups.data[groups.size++] = grp

void evaluateGroups()
	groups_free()

	bool *visited = calloc(R * C, sizeof(bool))
	#define VISITED(row, col) (visited[(col) * R + (row)])

	void dfs(int color, int r, int c, Coords *group)
		VISITED(r, c) = true
		Coord xy = {r, c}
		coords_append(group, xy)
		for int k = 0; k < 4; ++k
			int nr = r + (const int[]){-1, 0, 1, 0}[k]
			int nc = c + (const int[]){0, 1, 0, -1}[k]
			if nr >= 0 && nr < R && nc >= 0 && nc < C && !VISITED(nr, nc) && GRID(nr, nc) == color
				dfs(color, nr, nc, group)
	
	for int r = 0; r < R; ++r
		for int c = 0; c < C; ++c
			if !VISITED(r, c)
				int color = GRID(r, c)
				if color != 0
					Coords group = {0}
					dfs(color, r, c, &group)
					groups_append(group)
				else
					VISITED(r, c) = true
	free(visited)

typedef struct
	int color
	int length
	int row
	int col
Move

typedef struct
	Move *data
	int size
	int cap
Moves

Moves moves

void moves_init(Moves *m)
	m->data = NULL
	m->size = 0
	m->cap = 0
void moves_free(Moves *m)
	free(m->data)
	m->data = NULL
	m->size = m->cap = 0
void moves_append(Moves *m, Move mv)
	if m->size == m->cap
		int newcap = m->cap == 0 ? 4 : m->cap * 2
		Move *tmp = realloc(m->data, newcap * sizeof(Move))
		m->data = tmp; m->cap = newcap
	m->data[m->size++] = mv

void removeGroup(int index)
	if index < 0 || index >= groups.size
		printf("error: out of bounds index\n")

	Coords *grp = &groups.data[index]
	if grp->size == 0
		return

	int r0 = grp->data[0].r
	int c0 = grp->data[0].c
	Move mv = { GRID(r0, c0), grp->size, r0, c0 }
	moves_append(&moves, mv)

	// When you remove a group, all squares above fall down to fill the gaps
	for int k = 0; k < grp->size; ++k
		int rr = grp->data[k].r
		int cc = grp->data[k].c
		for int row = rr; row > 0; --row
			int val = GRID(row - 1, cc)
			GRID(row, cc) = val
			if val == 0
				break
		GRID(0, cc) = 0

	// If an entire column becomes empty, all columns to the right shift left to fill the gap
	int c = 0
	while c < C
		if GRID(R - 1, c) == 0
			for int c2 = c; c2 < C - 1; ++c2
				for int r = 0; r < R; ++r
					GRID(r, c2) = GRID(r, c2 + 1)
			--C
		else
			++c

	evaluateGroups()

int main()
	scanf("%u %u", &R, &C)
	while getchar() != '\n'
	grid = malloc(R * C)
	for int row = 0; row != R; ++row
		char line[C + 2]
		fgets(line, C + 2, stdin)
		for int col = 0; col != C; ++col
			grid[col * R + row] = line[col] - '0'

	evaluateGroups()
	for
		bool noValidMoveExists = true;
		for int i = 0; i < groups.size; ++i
			if groups.data[i].size > 1
				noValidMoveExists = false
				removeGroup(0)
				break
		if noValidMoveExists
			break

	int totalScore = 0
	for int i = 0; i < moves.size; ++i
		int l = moves.data[i].length - 1
		totalScore += l * l
	printf("%d\n", totalScore)
	printf("%d\n", moves.size)
	for int i = 0; i < moves.size; ++i
		Move m = moves.data[i]
		printf("%d %d %d %d\n", m.color, m.length, R - m.row, m.col + 1)