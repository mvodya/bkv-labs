#!/bin/bash

# Operation systems
# Lab 7 - Pinger monitor
# by Mark Vodyanitskiy, Arkadiy Shneider, Elena Bova, Danil Maltsev

# Отчиска экрана
printf "\033[2J\033[0;0H"

# Название файла, который будет прочитан
input="hosts.txt"

# Константы для форматирования
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

# Массив со смещениями для вывода "on" и "off"
declare -a common_offset
# Массив адресов, которые будут проверятся
declare -a common_addr

# Чтение файла по строкам
# p.s. файл должен заканчиваться пустой строкой, иначе последняя
# строка не будет прочиана
while IFS=" " read -r name addr
do
  # Выводим верхнюю строку
  printf "$name   "
  # Записываем смещение - длинна имени компьюетра в верхнем баре
  # три пробела будут учтены позже
  common_offset+=(`expr length $name`)
  # Записываем адрес
  common_addr+=($addr)
done < "$input"

printf "\n"

declare -i offset=-5
declare -i i=0

# Цикл сканирования
while true
do
  # Цикл перебора адресов (хотя на самом деле тут передираются смещения)
  for off in ${common_offset[@]}
  do
    # Хитро накапливаем смещение
    offset=$offset+$off+3
    # Проверяем доступность хоста по адресу
    ping -W 1 -c 1 ${common_addr[i]} &>/dev/null
    # Выводим "on" или  "off"
    if [ $? -eq 0 ]
    then
      printf "\033[2;$(($offset))H${GREEN} on${NC}"
    else
      printf "\033[2;$(($offset))H${RED}off${NC}"
    fi
    i=$((i+1))
  done
  # Обнуляем все временные значения назад
  offset=-5
  i=0
  # Задержка 10 секунд
  sleep 10
done