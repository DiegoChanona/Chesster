import chess
import chess.svg
import chess.engine
from colorama import init, Fore, Back, Style

init(autoreset=True)

def print_board():
    print(Back.BLUE + "  a b c d e f g h")
    for rank in range(8, 0, -1):
        print(Back.BLUE + f"{rank}", end=" ")
        for file in range(8):
            square = chess.square(file, rank - 1)
            piece = board.piece_at(square)
            if piece:
                piece_color = Fore.BLACK if piece.color == chess.WHITE else Fore.WHITE
                piece_symbol = piece.symbol()
                print(Back.BLUE + piece_color + f" {piece_symbol} ", end="")
            else:
                print(Back.BLUE + "   ", end="")
        print(Back.BLUE + f"{rank}")
    print(Back.BLUE + "  a b c d e f g h")

def save_move(initial_box, final_box, piece_type):
    moves.append([initial_box, final_box, piece_type])

def print_machine_move(move):
    initial_box = chess.square_name(move.from_square)
    final_box = chess.square_name(move.to_square)
    piece_type = board.piece_type_at(move.from_square)
    print("Machine's Move:")
    print(f"Initial Box: {initial_box}")
    print(f"Final Box: {final_box}")
    print(f"Piece Type: {piece_type}")

def play():
    while not board.is_game_over():
        if board.turn == chess.WHITE:
            print("Human player's turn:")
            print_board()
            user_input = input("Enter your move (e.g., e2-e4): ")
            try:
                move = board.parse_san(user_input)
                if move in board.legal_moves:
                    initial_box = chess.square_name(move.from_square)
                    final_box = chess.square_name(move.to_square)
                    piece_type = board.piece_type_at(move.from_square)
                    save_move(initial_box, final_box, piece_type)
                    board.push(move)
                else:
                    print("Illegal move. Try again.")
            except ValueError:
                print("Invalid move format. Try again.")
        else:
            print("Machine player's turn:")
            machine_move = engine.play(board, chess.engine.Limit(time=2.0))
            print_machine_move(machine_move.move)
            initial_box = chess.square_name(machine_move.move.from_square)
            final_box = chess.square_name(machine_move.move.to_square)
            piece_type = board.piece_type_at(machine_move.move.from_square)
            save_move(initial_box, final_box, piece_type)
            board.push(machine_move.move)

# Create an initial chess board
board = chess.Board()

# Initialize the list to store moves
moves = []

# Load the Stockfish engine
engine = chess.engine.SimpleEngine.popen_uci(r"C:\Users\erick\Documents\MiscApps\stockfish_15.1_win_x64_avx2\stockfish-windows-2022-x86-64-avx2.exe")

# Play the game
play()

# Close the Stockfish engine
engine.quit()

# Print the saved moves
print("Machine's Moves:")
for move in moves:
    print(f"Initial Box: {move[0]}")
    print(f"Final Box: {move[1]}")
    print(f"Piece Type: {move[2]}")
