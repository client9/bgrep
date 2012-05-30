#!/usr/bin/env python
import sys
from collections import defaultdict

def freqcount(fd):
    parts = defaultdict(int)

    for line in fd:
        parts[line.strip()] += 1

    for k,v in parts.iteritems():
        print k,v

if __name__ == '__main__':
    freqcount(sys.stdin)
