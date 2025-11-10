R, C = map(int, input().split())
G = []
for i in range(R):
	G.append([int(ch) for ch in input().strip()])

groups = []
groupIndexes = [[10000] * C for _ in range(R)]

def evaluateGroups():
	groups.clear()
	groupIndexes[:] = [[10000] * C for _ in range(R)]
	visited = [[False] * C for _ in range(R)]

	def dfs(groupIndex, color, r, c, group):
		visited[r][c] = True
		groupIndexes[r][c] = groupIndex
		group.append((r, c))
		for dr, dc in [(-1, 0), (0, 1), (1, 0), (0, -1)]:
			nr = r + dr
			nc = c + dc
			if 0 <= nr < R and 0 <= nc < C and not visited[nr][nc] and G[nr][nc] == color:
				dfs(groupIndex, color, nr, nc, group)

	groupIndex = 0
	for r in range(R):
		for c in range(C):
			if not visited[r][c]:
				color = G[r][c]
				if color != 0:
					group = []
					dfs(groupIndex, color, r, c, group)
					# group.sort(key=lambda p: (p[0], p[1]))
					groups.append(group)
					groupIndex = groupIndex + 1

moves = []

def removeGroup(index):
	group = groups[index]
	r, c = group[0]
	moves.append((G[r][c], len(group), r, c))

	# When you remove a group, all squares above fall down to fill the gaps.
	for r, c in group:
		for row in range(r, 0, -1):
			G[row][c] = G[row - 1][c]
			if G[row][c] == 0:
				break
		G[0][c] = 0

	# If an entire column becomes empty, all columns to the right shift left to fill the gap.
	c = 0
	global C
	while c < C:
		if G[R - 1][c] == 0:
			for c2 in range(c, C - 1):
				for r in range(R):
					G[r][c2] = G[r][c2 + 1]
			C = C - 1
		else:
			c += 1

	evaluateGroups()

evaluateGroups()

while True:
	noValidMoveExists = True
	for group in groups:
		if len(group) > 1:
			noValidMoveExists = False
			removeGroup(0)
			break
	if noValidMoveExists:
		break

#for i in range(R):
#	for j in range(C):
#		print(G[i][j], end=" ")
#	print()

#for group in groups:
#	print(group)

totalScore = 0
for move in moves:
	totalScore += (move[1] - 1) ** 2

print(totalScore)
print(len(moves))
for move in moves:
	print(move[0], move[1], R - move[2], move[3] + 1)