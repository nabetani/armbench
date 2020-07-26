import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from matplotlib.backends.backend_pdf import PdfPages


def read_csv(k):
    df = pd.read_csv('../data/'+k+'.csv', index_col=0)
    return df


def graph(fn, vals, title, t):
    pdf = PdfPages(fn+'.pdf')
    plt.figure()
    plt.yscale('log')
    plt.title(title)
    for name, v0 in vals:
        v = [c for c in v0.reset_index().values.tolist() if c[0] == t]
        x = [c[1] for c in v][5:]
        y = [c[2] for c in v][5:]
        plt.plot(x, y, label=name)
    plt.legend(bbox_to_anchor=(0, 1), loc='upper left',
               borderaxespad=0, fontsize=18)
    plt.xlabel('load')
    plt.ylabel('processing time [ms]')
    pdf.savefig()
    pdf.close()


def main():
    keys = ["amd64-go1.14.5",
            "arm7-go1.11.6",
            "arm64-go1.11.6",
            "arm7-go1.14.5",
            "arm64-go1.14.5"]
    values = [[k, read_csv(k)] for k in keys]

    g114Keys = ["amd64-go1.14.5", "arm7-go1.14.5", "arm64-go1.14.5"]
    g114 = [x for x in values if x[0] in g114Keys]
    graph("u64-go1.14", g114, "go 1.14, u64", "u64")
    graph("f64-go1.14", g114, "go 1.14, f64", "f64")
    graph("f32-go1.14", g114, "go 1.14, f32", "f32")

    armU64Keys = [k for k in keys if "arm" in k]
    armU64 = [x for x in values if x[0] in armU64Keys]

    for t in ["u64", "f64", "f32"]:
        graph("arm-"+t, armU64, "arm, "+t, t)


main()
