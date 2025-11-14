import numpy as np

# Get "input" input
infile = open("input.txt", "r")
firstline = True
G = []
for line in infile:
    if(firstline):
        R, C = map(int, line.strip().split())
        firstline = False
    else:
        G.append([int(ch) for ch in line.strip()])
G = np.array(G, dtype = int)
R, C = G.shape

# Get "output" input
outfile = open("outputs/output_from_" + input() + "_to_1091.txt")
linenum = 0
moves = []
score = 0
m = 0
for line in outfile:
    if(linenum == 0):
        score = int(line.strip())
        linenum += 1
    elif(linenum == 1):
        m = int(line.strip())
        linenum += 1
    else:
        t, n, r, c = map(int, line.strip().split())
        moves.append([t,n,r,c])

def printG():
    for row in G:
        for col in row:
            print(col, end='')
        print()

# Advance physics
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

# Remove tile group
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

# Simulate moves
def isValid(debug):
    totalscore = 0
    for move in moves:
        res = remove(move[2],move[3])
        if(res==0):
            continue
        totalscore += res[2]
        if(debug):
            printG()
            print("="*C)
        # Check validity
        if(move[0] != res[0] or move[1] != res[1]):
            print("Invalid move: ")
            print("Move made:")
            print(move)
            if(move[0] != res[0]):
                print("Color does not match")
            if(move[1] != res[1]):
                print("Amount does not match")
            print()
            return False
    if(totalscore != score):
        print("Score does not match")
        return False
    validMoveCoord = validMovesRemain()
    if(validMoveCoord is not None):
        print("Valid move remains")
        print(validMoveCoord)
        return False
    return True

def validMovesRemain():
    for r in range(R):
        for c in range(C):
            if c + 1 < C and G[r][c] == G[r][c + 1] and G[r][c] != 0:
                return (r, c)
            if r + 1 < R and G[r][c] == G[r + 1][c] and G[r][c] != 0:
                return (r, c)
    return None

# Set to true to see the board after every move
debug = False  
if(debug):
    printG()
print(isValid(debug))