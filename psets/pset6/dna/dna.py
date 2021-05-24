# import necessary dependencies
from sys import argv, exit
from csv import reader, DictReader

# validate the input args
if len(argv) != 3:
    print("Usage: python dna.py data.csv sequence.txt")
    exit()

# read the dna sample
with open(argv[2]) as dna_file:
    dna_reader = reader(dna_file)
    for i in dna_reader:
        dna_list = i

# store it in a string
dna = str(dna_list[0])


# Get the database
with open(argv[1]) as database:
    people = reader(database)
    for person in people:
        dnaSequences = person
        dnaSequences.pop(0)
        break

# store the database sequences in a dictionary
sequences = {}

# dictionary (key = genes)
for item in dnaSequences:
    sequences[item] = 1

# iterate trough the dna sequence
for key in sequences:
    temp_max = 0
    temp = 0
    for i in range(len(dna)):
        while temp > 0:
            temp -= 1
            continue

        l = len(key)
        if dna[i: i + l] == key:
            while dna[i - l: i] == dna[i: i + l]:
                temp += 1
                i += l

            if temp > temp_max:
                temp_max = temp

    sequences[key] += temp_max


# iterate through the database
with open(argv[1], newline='') as database:
    people = DictReader(database)
    for person in people:
        match = 0
        for dna in sequences:
            if sequences[dna] == int(person[dna]):
                match += 1
        # printing the name if match found
        if match == len(sequences):
            print(person['name'])
            exit()

    print("No match")
