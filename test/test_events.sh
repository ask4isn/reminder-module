#!/bin/bash
set -e

echo "Тест на много событии"

TIMES=$(yes 1 | head -n 17 | paste -sd, -)
MSGS=$(yes "msg" | head -n 17 | paste -sd, -)

if sudo insmod reminder.ko event_times="$TIMES" event_msgs="$MSGS"; then
    echo "Тест не пройден"
    sudo rmmod reminder
    exit 1
else
    echo "Модуль отказался загружаться с ошибкой"
fi