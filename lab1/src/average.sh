#! /bin/sh
count=0
sum=0
for an_arg in "$@" ; do
  sum=$(($sum + ${an_arg}))
  count=$(($count+1))
done
expr $sum / $count
expr $count

