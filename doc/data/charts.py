#*-* coding: utf-8 *-*
import sys
import matplotlib.pyplot as plt

data = []
data.append((
        (5, 165451, 35136),
        (6, 456523, 100507),
        (7, 3913744, 505596),
        (8, 13590827, 1359498)
        ))
data.append((
        (5, 139515, 71089),
        (6, 549378, 189203),
        (7, 3337096, 714068),
        (8, 13610961, 1589901)
        ))
data.append((
        (5, 156947, 78131),
        (6, 864731, 267493),
        (7, 3516925, 842535),
        (8, 19280966, 2423382)
        ))

def wl(f, s):
    f.write(s.encode("utf8") + "\n")

def draw_table(data, f):
    wl(f, r"\begin{tabular}{|c|c|c|}")
    wl(f, r"\hline")
    wl(f, u"Głębokość &" + 
       r"\multicolumn{2}{|c|}{" +
       u"Liczba odwiedzonych wierzchołków" + r"}\\")
    wl(f, r"\hline")
    wl(f, r"& $\alpha - \beta$ &" +
       r"$\alpha - \beta + \text{TT}$ \\ ")
    wl(f, r"\hline")
    for (depth, a, b) in data:
        wl(f, "%d & %d & %d \\\\" % (depth, a, b))
        wl(f, r"\hline")
    wl(f, r"\end{tabular}")

def draw_chart(data, fname):
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.plot([d[0] for d in data], [d[1] * 0.000001 for d in data], 'o-',
            label=u"\u237a - \u03b2")
    ax.plot([d[0] for d in data], [d[2] * 0.000001 for d in data], 'x-',
            label=u"\u237a - \u03b2 + TT")
    plt.grid(True, which='both')
    plt.legend(loc='upper left')
    plt.xticks([d[0] for d in data])
    plt.xlabel(u"Głębokość przeszukiwania")
    plt.ylabel(u"Liczba odwiedzonych wierzchołków [mln]")
    fig.savefig(fname)

def main():
    for i in range(len(data)):
        dat = data[i]
        filename = "results%d" % i
        tex_filename = filename + ".tex"
        pdf_filename = filename + ".pdf"

        with open(tex_filename, 'w') as f:
            draw_table(dat, f)
        draw_chart(dat, pdf_filename)

if __name__ == "__main__":
    main()

