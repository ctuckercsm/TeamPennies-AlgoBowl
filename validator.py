import numpy as np

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

step()
printG()