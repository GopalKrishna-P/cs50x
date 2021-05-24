from cs50 import get_int


def get_height():
    # get user input for height and return
    height = 0
    while (height > 8 or height < 1):
        height = get_int("Height: ")
    return height


def draw(rows):
    # print the pyramid
    for row in range(1, rows + 1):
        print(" " * (rows - row) + "#" * row)


def main():
    # main function
    height = get_height()
    draw(height)


if __name__ == "__main__":
    # execute the main()
    main()