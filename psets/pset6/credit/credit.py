from cs50 import get_string
from math import floor

# get user input
card = get_string("Card Number: ")
card = card.strip()
count = len(card)


# luhn's algorithm
even = count % 2 == 0
total = 0
for c in card:
    if even:
        total += (2 * int(c)) // 10 + (2 * int(c)) % 10
    else:
        total += int(c)
    even = not even


if total % 10 != 0:
    print("INVALID\n")

# 16-digit
elif count == 16:
    if card[0] == '4':
        print("VISA\n")
    elif card[0] == '5' and card[1] in ['1', '2', '3', '4', '5']:
        print("MASTERCARD\n")
    else:
        print(card[1])
        print("INVALID1\n")

# 15-digit
elif count == 15:
    if card[0] == '3' and card[1] in ['4', '7']:
        print("AMEX\n")
    else:
        print(card[0])
        print("INVALID2\n")

# 13-digit
elif count == 13:
    if card[0] == '4':
        print("VISA\n")
    else:
        print("INVALID3\n")

else:
    print("INVALID4\n")
