# Reminder Module
Модуль ядра, который реализует напоминания о событиях.

## Требования
- Операционная система: Linux
- Установка пакетов:
```bash
sudo apt update
sudo apt install build-essential linux-headers-$(uname -r)
```
- Права: root

## Параметры модуля
- **event_times** - задержки в секундах
- **event_msgs** - сообщения (максимум 127 символов, остальное обрезается)

### Пример загрузки модуля:
```bash
sudo insmod reminder.ko \
event_times=5,10,15 \
event_msgs="test1","test2","test3"
```
### Вывод логов виден в **dmesg**:
reminder #0: "test1" через 5 секунд
reminder #1: "test2" через 10 секунд
reminder #2: "test3" через 15 секунд

## Сборка модуля
```bash
make
```

## Тестирование
Подробнее в папке test: README.md

## Выгрузка модуля
```bash
sudo rmmod reminder
```
### Вывод логов через:
```bash
dmesg | tail -n 5
```

## Очистка
```bash
make clean
```