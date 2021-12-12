from tkinter import *
import subprocess

root = Tk()
col = []
pcs = []
mvs = []
x_selected = 0;
y_selected = 0;
selected = 0;

canvas= Canvas(root, width=(8 * 45), height=(8 * 45))

def reset_color():
    global col
    col = [[("#ffce9e", "#d18b47")[(i + j) % 2] for j in range(8)] for i in range(8)]

def create_pieces():
    global pcs
    pcs = []
    board = open("board.txt", "r")
    pieces = {1:'plt', 2:'rlt', 3:'nlt', 4:'blt', 5:'qlt', 6:'klt', 9:'pdt', 10:'rdt', 11:'ndt', 12:'bdt', 13:'qdt', 14:'kdt'}
    for i in range(8):
        for j in range(8):
            p = int(board.readline())
            if p != 0:
                pcs.append((PhotoImage(file='img/'+pieces[p]+'.png'), i, j))
            else:
                pcs.append((0, i, j))
    board.close()

def create_moves():
    global mvs
    mvs = [[[] for j in range(8)] for i in range(8)]
    moves = open("moves.txt", "r")
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
        canvas.create_rectangle((t[0] * 45), (t[1] * 45), ((t[0] + 1) * 45), ((t[1] + 1) * 45), fill='blue')

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
        x_selected = event.y // 45
        y_selected = event.x // 45
        draw_board()
        draw_moves(x_selected, y_selected)
        draw_pieces()
        selected = 1
    else:
        x_click = event.x // 45
        y_click = event.y // 45
        if((x_click, y_click) in mvs[x_selected][y_selected]):
            f = open("game.txt", "a")
            f.write("abcdefgh"[y_selected] + str(x_selected + 1) + "abcdefgh"[x_click] + str(y_click + 1) + "\n")
            f.close()
            subprocess.run(["./Chess.xpp", "-u"])
            create_pieces()
            create_moves()
            draw_board()
            draw_pieces()
            subprocess.run(["./Chess.xpp", "-i", "-s", "-b", "-m"])
            create_pieces()
            create_moves()
            draw_board()
            draw_pieces()
            selected = 0
        else:
            draw_board()
            draw_pieces()
            selected = 0

reset_color()
draw_board()
create_pieces()
draw_pieces()
create_moves()

canvas.bind("<Button-1>", callback)
canvas.pack()

root.mainloop()
