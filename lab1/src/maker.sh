#!/bin/bash

# Разделяем аргументы в массив
IFS=$'\n' read -d '' -r -a arg_array < numbers.txt
count=0
sum=0

# Выполняем операции над аргументами
for an_arg in "${arg_array[@]}"; do
  sum=$((sum + an_arg))
  count=$((count + 1))
done

# Выводим результаты
result=$(expr "$sum" / "$count")
echo "Average sum: $result"
echo "Count: $count"
