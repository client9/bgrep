#!/bin/sh
awk '
BEGIN {
  count[""] = 0
}
{
  count[$0]++
}
END {
  for (i in count) {
    if (i != "") {
      print i, count[i]
    }
  }
}' |
sort -g