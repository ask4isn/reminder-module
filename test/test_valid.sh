#!/bin/bash
set -e

echo "Тест на корректную загрузку"

sudo dmesg -C
sudo insmod reminder.ko event_times=2,4,6 event_msgs="one","two","three"
sleep 7

echo "Вывод:"
dmesg | grep "reminder"

echo "Выгрузка модуля"
sudo rmmod reminder