import chess
import chess.engine
import chess.svg
import io
import tkinter as tk
#import cairosvg
from PIL import Image, ImageTk
import svglib

import cairo
#import rsvg

import subprocess
import os

import svglib
from svglib.svglib import svg2rlg
from reportlab.graphics import renderPM

class ChessGUI(tk.Frame):
    def __init__(self, parent, fen=None):
        tk.Frame.__init__(self, parent)
        self.parent = parent
        self.canvas_size = 400
        self.canvas = tk.Canvas(self, width=self.canvas_size, height=self.canvas_size)
        self.canvas.pack()
        self.chessboard = chess.Board(fen)
        self.draw_board()
        self.bind("<Button-1>", self.on_click)
        self.engine = chess.engine.SimpleEngine.popen_uci("stockfish_path")

    def draw_board(self):
        board_svg = chess.svg.board(board=self.chessboard)
        board_svg = board_svg.replace('width="560"', f'width="{self.canvas_size}"')
        board_svg = board_svg.replace('height="560"', f'height="{self.canvas_size}"')
        board_png = self.svg_to_png(board_svg)
        self.board_image = ImageTk.PhotoImage(board_png)
        self.canvas.create_image(0, 0, image=self.board_image, anchor="nw")

    def on_click(self, event):
        row = int((self.canvas_size - event.y) / (self.canvas_size / 8))
        col = int(event.x / (self.canvas_size / 8))
        square = chess.square(col, row)
        if self.selected_square is None:
            self.selected_square = square
        else:
            move = chess.Move(self.selected_square, square)
            if move in self.chessboard.legal_moves:
                self.chessboard.push(move)
                self.draw_board()
                self.make_engine_move()
            self.selected_square = None

    def make_engine_move(self):
        result = self.engine.play(self.chessboard, chess.engine.Limit(time=2.0))
        self.chessboard.push(result.move)
        self.draw_board()

    @staticmethod
    def svg_to_png(svg_data):
        svg_file = "temp.svg"
        png_file = "temp.png"

        with open(svg_file, "w") as f:
            f.write(svg_data)

        try:
            subprocess.run(["inkscape", "-z", "-e", png_file, svg_file])
            with open(png_file, "rb") as f:
                png_data = f.read()
            return io.BytesIO(png_data)
        finally:
            os.remove(svg_file)
            os.remove(png_file)

if __name__ == "__main__":
    root = tk.Tk()
    root.title("Chess Game")
    gui = ChessGUI(root)
    gui.pack(side="top", fill="both", expand="true")
    root.mainloop()
