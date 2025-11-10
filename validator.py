R, C = map(int, input().split())
G = []
for i in range(R):
	G.append([int(ch) for ch in input().strip()])

groups = []

def evaluateGroups():
	visited = [[False] * C for _ in range(R)]

	def dfs(color, r, c, group):
		visited[r][c] = True
		group.append((r, c))
		for dr, dc in [(-1, 0), (0, 1), (1, 0), (0, -1)]:
			nr, nc = r + dr, c + dc
			if 0 <= nr < R and 0 <= nc < C and not visited[nr][nc] and G[nr][nc] == color:
				dfs(color, nr, nc, group)

	for r in range(R):
		for c in range(C):
			if not visited[r][c]:
				color = G[r][c]
				grp = []
				dfs(color, r, c, grp)
				groups.append(grp)

for i in range(R):
	for j in range(C):
		print(G[i][j], end=" ")
	print()

for group in groups:
	print(group)

def move(color, number, row, column):
	