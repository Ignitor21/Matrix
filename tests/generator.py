import numpy as np
import os

size = int(input("Input square matrix size: "))
lower_bound, upper_bound = map(int, input("Input upper bound and lower bound respectively: ").split())    
# matrix = np.random.randint(lower_bound, upper_bound, (size, size))
matrix = np.random.random((size, size))

all_files = os.listdir()
max_test_number = 0
for file in all_files:
    if file.endswith('.in'):
        test_number = int(file[:-3])
        if (test_number > max_test_number):
            max_test_number = test_number

answer = np.linalg.det(matrix)

with open(str(max_test_number + 1).zfill(3) + ".in", "w") as f:
    f.write(str(size) + "\n")
    for row in matrix:
        for num in row:
            f.write(str(num) + ' ')
        f.write("\n")

os.chdir("answers")
with open(str(max_test_number + 1).zfill(3) + ".ans", "w") as ans:
    ans.write("%.10g" % answer)

