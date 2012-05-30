#!/usr/bin/awk -f
BEGIN {
    # get first line to set initial min and max
    getline;
    ll = $0 + 0.0;
    min = ll;
    max = ll;
}
{
 ll = $0 + 0.0;
 if (ll > max) {
   max = ll
 } else if (ll < min) {
      min = ll
 }
}
END {
  print min, max
}
