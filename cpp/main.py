import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from matplotlib.backends.backend_pdf import PdfPages
import csv
import statistics


def read_csv(k):
    r = []
    with open(k+'.csv', newline='') as f:
        reader = csv.reader(f)
        for row in reader:
            r.append([row[0], float(row[1])])
    return r


def graph(c, vals):
    pdf = PdfPages(c+'.pdf')
    plt.figure(figsize=(8, 2))
    plt.title(c)

    v = [[v[0], statistics.median(
        [row[1] for row in v[1] if row[0] == c])] for v in vals]

    print(repr(v))
    ypos = np.arange(len(v))
    bar = plt.barh(ypos, [m[1] for m in v])
    plt.yticks(ypos, [m[0] for m in v])
    plt.xlabel('processing time [ms]')
    plt.ylabel('build settig')
    plt.tight_layout()
    pdf.savefig()
    pdf.close()


def main():
    keys = reversed(["v7", "v7a-neon2", "v8", "arm64", "arm64clang"])
    values = [[k, read_csv(k)] for k in keys]
    print(values[0][1])
    conds = ["uint8", "uint16", "uint32", "uint64", "float", "double"]
    for c in conds:
        graph(c, values)


main()
