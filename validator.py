import numpy as np

# Get input
R, C = map(int, input().split())
G = []
for i in range(R):
	G.append([int(ch) for ch in input().strip()])
G = np.array(G, dtype = int)
R, C = G.shape

def printG():
    for row in G:
        for col in row:
            print(col, end='')
        print()

def step():
    global G
    # Step columns downward
    for c in range(C):
        col = G[:, c]
        nonzero = col[col != 0]
        G[:, c] = 0
        if nonzero.size > 0:
            G[-nonzero.size:, c] = nonzero
    # Step columns leftward
    nonzerocols = (G != 0).any(axis=0)
    nonzerocols = G[:, nonzerocols]
    newG = np.zeros_like(G)
    newG[:, :nonzerocols.shape[1]] = nonzerocols
    G = newG

def remove(r, c):
    global G
    r = R - r
    c = c - 1

    target = G[r, c]
    if target == 0:
        return 0

    visited = set()
    stack = [(r, c)]
    to_remove = []

    # Get group
    while stack:
        rr, cc = stack.pop()
        if (rr, cc) in visited:
            continue
        visited.add((rr, cc))
        if G[rr, cc] == target:
            to_remove.append((rr, cc))
            for dr, dc in [(-1,0), (1,0), (0,-1), (0,1)]:
                nr, nc = rr + dr, cc + dc
                if 0 <= nr < R and 0 <= nc < C:
                    if (nr, nc) not in visited and G[nr, nc] == target:
                        stack.append((nr, nc))
    for rr, cc in to_remove:
        G[rr, cc] = 0
    step()
    return int(target), len(to_remove), (len(to_remove)-1)**2

print(remove(2,2))
printG()