import sys
import random

while(True):
    lr, lc = [int(i) for i in input().split()]
    num_moves = int(input())
    cur_moves = []
    for n in range(num_moves):
        cur_moves.append([int(i) for i in input().split()])
    print("Choices:", cur_moves, file=sys.stderr)
    if (len(cur_moves) == 0):
        print("-1 -1")
        continue
    move = random.choice(cur_moves)
    print("-> {} {}".format(move[0], move[1]), file=sys.stderr)
    print("{} {}".format(move[0], move[1]))

