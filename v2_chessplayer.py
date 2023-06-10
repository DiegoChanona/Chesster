import tkinter as tk
import chess
import chess.svg

# Create the main window
window = tk.Tk()
window.title("Chess GUI")
window.overrideredirect(False)  # Display minimize, maximize, close buttons


# Calculate the dimensions of the chessboard
board_size = 8
square_size = 80
canvas_width = square_size * board_size
canvas_height = square_size * board_size

# Calculate the dimensions of the window to center the chessboard
window_width = canvas_width + 20  # Add some padding
window_height = canvas_height + 80  # Add padding and space for buttons
screen_width = window.winfo_screenwidth()
screen_height = window.winfo_screenheight()
x = (screen_width - window_width) // 2
y = (screen_height - window_height) // 2
window.geometry(f"{window_width}x{window_height}+{x}+{y}")

# Create the chessboard frame
chessboard_frame = tk.Frame(window, bg="black")
chessboard_frame.grid(row=0, column=0, pady=10)


# Create the chessboard canvas
chessboard = tk.Canvas(chessboard_frame, width=canvas_width, height=canvas_height, bg="white")
chessboard.grid(row=0, column=0)

# Load the chess piece images
piece_images = {
    "r": tk.PhotoImage(file=r"C:\Users\erick\Desktop\PIM\Chess Stuff\ChessPieceImg\b_pawn.png"),
    "n": tk.PhotoImage(file=r"C:\Users\erick\Desktop\PIM\Chess Stuff\ChessPieceImg\b_pawn.png"),
    "b": tk.PhotoImage(file=r"C:\Users\erick\Desktop\PIM\Chess Stuff\ChessPieceImg\b_pawn.png"),
    "q": tk.PhotoImage(file=r"C:\Users\erick\Desktop\PIM\Chess Stuff\ChessPieceImg\b_pawn.png"),
    "k": tk.PhotoImage(file=r"C:\Users\erick\Desktop\PIM\Chess Stuff\ChessPieceImg\b_pawn.png"),
    "p": tk.PhotoImage(file=r"C:\Users\erick\Desktop\PIM\Chess Stuff\ChessPieceImg\b_pawn.png"),
    "R": tk.PhotoImage(file=r"C:\Users\erick\Desktop\PIM\Chess Stuff\ChessPieceImg\b_pawn.png"),
    "N": tk.PhotoImage(file=r"C:\Users\erick\Desktop\PIM\Chess Stuff\ChessPieceImg\b_pawn.png"),
    "B": tk.PhotoImage(file=r"C:\Users\erick\Desktop\PIM\Chess Stuff\ChessPieceImg\b_pawn.png"),
    "Q": tk.PhotoImage(file=r"C:\Users\erick\Desktop\PIM\Chess Stuff\ChessPieceImg\b_pawn.png"),
    "K": tk.PhotoImage(file=r"C:\Users\erick\Desktop\PIM\Chess Stuff\ChessPieceImg\b_pawn.png"),
    "P": tk.PhotoImage(file=r"C:\Users\erick\Desktop\PIM\Chess Stuff\ChessPieceImg\b_pawn.png"),
}


# Create an empty list to store the move history
move_history = []

# Function to update the chessboard display
def update_chessboard():
    # Clear the chessboard
    chessboard.delete("piece")

    # Redraw the chess pieces
    for square, piece in board.piece_map().items():
        x = (square % 8) * square_size
        y = ((7 - square // 8) * square_size)
        image = piece_images[piece.symbol()]
        chessboard.create_image(x, y, image=image, tags=("piece",), anchor="c")

    # Update the display
    chessboard.update()

# Function to handle making a move
def make_move(source_square, target_square):
    move = chess.Move(source_square, target_square)
    if move in board.legal_moves:
        board.push(move)
        move_history.append(move)
        update_chessboard()

# Function to handle the "Undo" button click
def undo_move():
    if len(move_history) > 0:
        move = move_history.pop()
        board.pop()
        update_chessboard()

# Function to generate a chessboard
def generate_chessboard():
    # Clear the move history
    move_history.clear()

    # Clear the chessboard
    chessboard.delete("piece")

    # Create a new chess board
    global board
    board = chess.Board()

    # Update the chessboard display
    update_chessboard()

# Bind the drag and drop events
# ...

# Create the "Undo" button
undo_button = tk.Button(window, text="Undo", command=undo_move)
undo_button.grid(row=1, column=0, pady=5)

# Create the "Generate Board" button
generate_button = tk.Button(window, text="Generate Board", command=generate_chessboard)
generate_button.grid(row=2, column=0, pady=5)

# Generate the initial chessboard
generate_chessboard()

# Center the window on the screen
window.update()
window_width = window.winfo_width()
window_height = window.winfo_height()
screen_width = window.winfo_screenwidth()
screen_height = window.winfo_screenheight()
x = (screen_width - window_width) // 2
y = (screen_height - window_height) // 2
window.geometry(f"+{x}+{y}")

# Start the Tkinter event loop
window.mainloop()
