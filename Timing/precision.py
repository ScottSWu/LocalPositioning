# Don't run while running tmux =(
# Don't run while running anything really

import sys,time

def main():
    rows = 200
    cols = 50
    area = rows * cols
    index = 0
    result = [0] * (4 * area)

    i = 0
    ts = time.time()
    te = time.time()
    while i < area:
        ts = te
        while te - ts < 0.001:
            te = time.time()
        result[index] = te - ts
        index = index + 1
        i = i + 1

    i = 0
    ts = time.time()
    te = time.time()
    while i < area:
        ts = te
        while te - ts < 0.0001:
            te = time.time()
        result[index] = te - ts
        index = index + 1
        i = i + 1

    i = 0
    ts = time.time()
    te = time.time()
    while i < area:
        ts = te
        while te - ts < 0.00001:
            te = time.time()
        result[index] = te - ts
        index = index + 1
        i = i + 1

    i = 0
    ts = time.time()
    te = time.time()
    while i < area:
        ts < te
        while te - ts < 0.000001:
            te = time.time()
        result[index] = te - ts
        index = index + 1
        i = i + 1
    
    index = 0
    amt = 100
    for sf in range(3,7):
        amt = amt * 10
        bad = 0
        sys.stdout.write(str(sf) + "\n")
        for r in range(rows):
            for c in range(cols):
                d = int(result[index] * amt)
                sys.stdout.write(str(d) + " ")
                index = index + 1
                if d > 1:
                    bad = bad + 1
            sys.stdout.write("\n")
        sys.stdout.write("Bad: " + str(bad) + "\n\n")

if __name__=="__main__":
    main()

