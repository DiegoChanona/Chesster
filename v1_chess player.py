import chess
import chess.engine
from colorama import init, Fore, Back

# Initialize colorama
init()

# Create a chessboard
board = chess.Board()

# Initialize the stockfish engine
engine = None

# Function to set Stockfish skill level
def set_stockfish_skill_level(level):
    global engine
    if engine is not None:
        engine.quit()
    engine_path = r"C:\Users\erick\Documents\MiscApps\stockfish_15.1_win_x64_avx2\stockfish-windows-2022-x86-64-avx2.exe"
    engine = chess.engine.SimpleEngine.popen_uci(engine_path)

# Print the initial chessboard
def print_board():
    print("\n")
    for rank in chess.RANK_NAMES[::-1]:
        for file in chess.FILE_NAMES:
            square = chess.square(chess.FILE_NAMES.index(file), chess.RANK_NAMES.index(rank))
            piece = board.piece_at(square)
            if piece is None:
                print(Fore.WHITE + Back.GREEN + "  " + Back.RESET, end="")
            else:
                piece_color = Fore.WHITE if piece.color == chess.WHITE else Fore.YELLOW
                print(piece_color + Back.GREEN + piece.symbol() + " " + Back.RESET, end="")
        print(Fore.WHITE + Back.GREEN + " " + rank + Back.RESET)
    print(Fore.WHITE + Back.GREEN + "  " + " ".join(chess.FILE_NAMES) + Back.RESET + "\n")

# Play the game
def play_game():
    while not board.is_game_over():
        print_board()
        if board.turn == chess.WHITE:
            player_move = input("Enter your move (e.g., 'e2e4'): ")
            try:
                move = chess.Move.from_uci(player_move)
                if move in board.legal_moves:
                    board.push(move)
                else:
                    print(Fore.RED + "Illegal move" + Fore.RESET)
                    continue
            except ValueError:
                print(Fore.RED + "Invalid move format" + Fore.RESET)
                continue
        else:
            result = engine.play(board, chess.engine.Limit(time=2.0))
            board.push(result.move)
            print(f"The machine moved: {result.move}")

    print_board()
    print("Game over.")
    if engine is not None:
        engine.quit()

# Start the game
set_stockfish_skill_level(1)  # Set the initial Stockfish skill level
play_game()
