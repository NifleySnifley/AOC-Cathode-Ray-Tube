import os
import sys

inp = None

if (len(sys.argv) < 2):
    print("Please specify the file containing your input in the first argument")
    exit(1)

infile = sys.argv[1]

with open(infile) as f:
    inp = [line.strip() for line in f.readlines()]

out = []
for i in inp:
    i = i.split(' ')
    if i[0] == 'noop':
        out.append(0)
    elif i[0] == 'addx':
        out.append(1)
        out.append(int(i[1]))

outstr = "{ " + ", ".join(map(str, out)) + " };"
print(f"const int8_t INPUT[{len(out)}] = {outstr}")
