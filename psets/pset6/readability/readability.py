from cs50 import get_string

# get the user input
text = get_string("Text: ")

# initate variable for furthur use
letters, words, sentences = 0, 1, 0

# parse through the sentence
for letter in text:

    if 90 >= ord(letter) >= 65 or 122 >= ord(letter) >= 97:
        letters += 1

    if ord(letter) == 32:
        words += 1

    if ord(letter) == 33 or ord(letter) == 46 or ord(letter) == 63:
        sentences += 1


# Coleman-Liau index
index = round(0.0588 * (letters * 100 / words) - 0.296 * (sentences * 100 / words) - 15.8)

# print the grade
if index < 1:
    print("Before Grade 1")
elif index >= 16:
    print("Grade 16+")
else:
    print(f"Grade {index}")
