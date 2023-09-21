#!/bin/bash
clear
# Вывести текущий путь (рабочий каталог)
current_path=$(pwd)
echo "Current path: $current_path"

# Вывести текущую дату и время
current_date_time=$(date)
echo "Current date and time: $current_date_time"

# Вывести содержимое переменной окружения PATH
echo "PATH:"
echo $PATH
