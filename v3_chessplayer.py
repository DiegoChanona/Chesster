import tkinter as tk

# Create the main window
window = tk.Tk()
window.title("Chess Board")

# Set the dimensions of the window
window_size = 400
window.geometry(f"{window_size}x{window_size}")

# Calculate the square size based on the window size
square_size = min(window_size // 8, 50)  # Set the desired square size (minimum of 50 pixels)

# Calculate the offsets to center the chessboard
offset_x = (window_size - square_size * 8) // 2
offset_y = (window_size - square_size * 8) // 2

# Create a frame to hold the chessboard
chessboard_frame = tk.Frame(window, bg="white", width=square_size * 8, height=square_size * 8)
chessboard_frame.place(x=offset_x, y=offset_y)

# Function to create a chessboard square
def create_square(row, col):
    color = "white" if (row + col) % 2 == 0 else "black"
    square = tk.Label(chessboard_frame, bg=color)
    square.place(x=col * square_size, y=row * square_size, width=square_size, height=square_size)

# Create the chessboard squares
for row in range(8):
    for col in range(8):
        create_square(row, col)

# Start the Tkinter event loop
window.mainloop()
