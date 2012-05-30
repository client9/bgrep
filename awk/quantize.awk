#!/usr/bin/awk -f
BEGIN {
  blen = bmax - bmin;
  count[""] = 0;
  for (i = 0; i < blen / binc; i++) {
     count[i] = 0;
  }
}
{
  if ($0 >= bmin && $0 <= bmax) {
      count[int(($0 - bmin) / binc)] += 1
  }
}
END {
  for (i in count) {
    if (i != "") {
      print i *binc + bmin, count[i]
    }
  }
}
