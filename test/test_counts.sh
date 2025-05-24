#!/bin/bash
set -e

echo "Тест на параметры"

if sudo insmod reminder.ko event_times=1,2 event_msgs="a"; then
    echo "Тест не пройден"
    sudo rmmod reminder
    exit 1
else
    echo "Модуль отказался загружаться с ошибкой"
fi