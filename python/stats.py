#!/usr/bin/env python
import sys

def stats(fd):
    lmin = 0
    lmax = 0
    line = fd.readline()
    val = float(line)
    lmin = lmax = val

    for line in fd:
        val = float(line)
        if val > lmax:
            lmax = val
        elif val < lmin:
            lmin = val

    print lmin, lmax


if __name__ == '__main__':
    stats(sys.stdin)
