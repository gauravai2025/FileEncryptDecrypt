import os
import string
import random

print("Script started")

def makeFiles(path):
    os.makedirs(path, exist_ok=True)
    print("Created:", os.path.abspath(path))

    os.chdir(path)
    print("Now in:", os.getcwd())

    for i in range(10):
        filename = f"test{i+1}.txt"
        with open(filename, "w") as file:
            random_string = ''.join(
                random.choices(
                    string.ascii_uppercase + string.digits,
                    k=1000
                )
            )
            file.write(random_string)

    print("Done creating files")

if __name__ == "__main__":
    makeFiles("test")