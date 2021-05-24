from cs50 import get_float

dollars = 0
coins = 0

while dollars <= 0:
    dollars = get_float("Change owed: ")

cents = round(dollars * 100)

if cents >= 25:
    coins += cents // 25
    # print(coins)
    cents %= 25

if cents >= 10:
    coins += cents // 10
    # print(coins)
    cents %= 10

if cents >= 5:
    coins += cents // 5
    # print(coins)
    cents %= 5

if cents > 0:
    coins += cents
    # print(coins)


print(coins)