import chess
import chess.engine
from colorama import init, Fore, Style

# Initialize colorama
init(autoreset=True)

board = chess.Board()
engine_path = r"C:\Users\erick\Documents\MiscApps\stockfish_15.1_win_x64_avx2\stockfish-windows-2022-x86-64-avx2.exe"
difficulty = 20  # Default difficulty level (can be adjusted)

moves = []  # List to store moves [initial_box, final_box, piece]

def print_board():
    board_str = ""
    for rank in range(8, 0, -1):
        board_str += str(rank) + " "
        for file in range(8):
            square = chess.square(file, rank - 1)
            piece = board.piece_at(square)
            if piece is None:
                board_str += Fore.WHITE + " - "
            else:
                piece_map = {
                    chess.PAWN: "P",
                    chess.ROOK: "R",
                    chess.KNIGHT: "N",
                    chess.BISHOP: "B",
                    chess.QUEEN: "Q",
                    chess.KING: "K"
                }
                piece_name = piece_map[piece.piece_type]
                if piece.color == chess.WHITE:
                    board_str += Fore.CYAN + " " + piece_name + " "
                else:
                    board_str += Fore.MAGENTA + " " + piece_name + " "
        board_str += Fore.WHITE + " " + str(rank) + "\n"
    board_str += "  " + " ".join(list("a b c d e f g h"))
    print(board_str)

def play():
    while not board.is_game_over():
        print_board()
        move = input("Enter your move (e.g., e2e4): ")
        try:
            initial_box = move[:2]
            final_box = move[2:]
            move = chess.Move.from_uci(move)
            piece = board.piece_type_at(move.from_square)

            # Append the move to the list
            moves.append([initial_box, final_box, piece])

            if move in board.legal_moves:
                board.push(move)
                print("Moved:", move)
                if not board.is_game_over():
                    result = engine.play(board, chess.engine.Limit(time=2))
                    machine_move = result.move
                    board.push(machine_move)
                    machine_initial_box = chess.square_name(machine_move.from_square)
                    machine_final_box = chess.square_name(machine_move.to_square)
                    machine_piece = board.piece_type_at(machine_move.from_square)
                    moves.append([machine_initial_box, machine_final_box, machine_piece])
                    print("Machine moved:", machine_initial_box + "," + machine_final_box + ",1")
            else:
                print("Illegal move.")
        except (ValueError, IndexError):
            print("Invalid move format.")

    print("Game over.")
    print_board()

# Initialize the engine with the specified difficulty
engine = chess.engine.SimpleEngine.popen_uci(engine_path)
engine.configure({"Skill Level": difficulty})

# Call the play function to start the game
play()

engine.quit()

# Access the moves list after the game is over
for move in moves:
    initial_box, final_box, piece = move
    print("Initial box:", initial_box)
    print("Final box:", final_box)
    print("Piece:", piece)
