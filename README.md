# Reminder Module
Модуль ядра, который реализует напоминания о событиях.

## Сборка
```bash
make
```

## Загрузка модуля с передачей параметров:
```bash
sudo insmod reminder.ko \
event_times=5,10,15 \
event_msgs="test1","test2","test3"
```

## Выгрузка модуля:
```bash
sudo rmmod reminder
```
Вывод через:
```bash
dmesg | tail -n 5
```