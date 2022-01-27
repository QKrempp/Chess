from tkinter import *
import subprocess
import platform

root = Tk()
col = []
pcs = []
mvs = []
css = {'a':0, 'b':1, 'c':2, 'd':3, 'e':4, 'f':5, 'g':6, 'h':7}
x_selected  = 0;
y_selected  = 0;
selected    = 0;

exefile     = "./Chess.xpp"
gamefile    = "data/game.txt"
boardfile   = "data/board.txt"
movefile    = "data/moves.txt"
imgfolder   = "img/"

if(platform.system() == "Windows"):
    #import os
    #os.chdir('win/')
    exefile = "Chess.exe"
    imgfolder = "../img/"

canvas= Canvas(root, width=(8 * 45), height=(8 * 45))

def reset_color():
    global col
    global css
    col = [[("#ffce9e", "#d18b47")[(i + j) % 2] for j in range(8)] for i in range(8)]

def show_last_move():
    f = open(gamefile, "r")
    tmp = f.readlines()
    last_mv = tmp[-1]
    f.close()
    if last_mv == 'D':
        pass
    elif last_mv == 'B':
        pass
    elif last_mv == 'W':
        pass
    else:
        col[css[last_mv[0]]][7 - (int(last_mv[1]) - 1)] = ["#ff9ea0", "#d14b47"][(css[last_mv[0]] + int(last_mv[1])) % 2]
        col[css[last_mv[2]]][7 - (int(last_mv[3]) - 1)] = ["#ff9ea0", "#d14b47"][(css[last_mv[2]] + int(last_mv[3])) % 2]

def create_pieces():
    global pcs
    pcs = []
    board = open(boardfile, "r")
    pieces = {1:'plt', 2:'rlt', 3:'nlt', 4:'blt', 5:'qlt', 6:'klt', 9:'pdt', 10:'rdt', 11:'ndt', 12:'bdt', 13:'qdt', 14:'kdt'}
    for i in range(8):
        for j in range(8):
            p = int(board.readline())
            if p != 0:
                pcs.append((PhotoImage(file=imgfolder+pieces[p]+'.png'), 7 - i, j))
            else:
                pcs.append((0, 7 - i, j))
    board.close()

def create_moves():
    global mvs
    mvs = [[[] for j in range(8)] for i in range(8)]
    moves = open(movefile, "r")
    indice = 0
    for i in range(8):
        for j in range(8):
            m = moves.readline()[:-1]
            m = int(m)
            for k in range(64):
                if((m >> k) & 1):
                    mvs[i][j].append((k % 8, k // 8))
    moves.close()

def draw_moves(x, y):
    for t in mvs[x][y]:
        col[t[0]][7 - t[1]] = [["#9ed3ff", "#47abd1"][(t[0] + t[1] + 1) % 2]]

def draw_pieces():
    for p in pcs:
        if(p[0] != 0):
            canvas.create_image(22 + p[2] * 45, 22 + p[1] * 45, image=p[0])

def draw_board():
    for i in range(8):
        for j in range(8):
            canvas.create_rectangle((i * 45), (j * 45), ((i + 1) * 45), ((j + 1) * 45), fill=col[i][j])

def callback(event):
    global selected
    global x_selected
    global y_selected
    if(not selected):
        x_selected = 7 - event.y // 45
        y_selected = event.x // 45
        draw_moves(x_selected, y_selected)
        draw_board()
        draw_pieces()
        selected = 1
    else:
        x_click = event.x // 45
        y_click = 7 - event.y // 45
        if((x_click, y_click) in mvs[x_selected][y_selected]):
            f = open(gamefile, "a")
            f.write("abcdefgh"[y_selected] + str(x_selected + 1) + "abcdefgh"[x_click] + str(y_click + 1) + "\n")
            f.close()
            subprocess.run([exefile, "-u"])
            create_pieces()
            create_moves()
            reset_color()
            show_last_move()
            draw_board()
            draw_pieces()
            root.update()
            subprocess.run([exefile, "-i", "-s", "-b", "-m"])
            create_pieces()
            create_moves()
            reset_color()
            show_last_move()
            draw_board()
            draw_pieces()
            selected = 0
        else:
            reset_color()
            draw_board()
            draw_pieces()
            selected = 0

f = open(gamefile, "w")
f.write("")
f.close()
subprocess.run([exefile, "-u"])
reset_color()
draw_board()
create_pieces()
draw_pieces()
create_moves()

canvas.bind("<Button-1>", callback)
canvas.pack()

root.mainloop()
