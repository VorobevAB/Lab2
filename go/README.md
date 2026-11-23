# Реализация заданий на Go

Проект содержит задания 1–7 и собственные реализации структур данных.

## Сборка

```bash
go build -o program .
```

## Запуск

```bash
./program 1
./program 2
./program 3
./program 4
./program 5
./program 6
./program 7
```

Для второго задания:

```bash
./program 2 --file data.txt --query "SETADD apple"
```

Модуль называется `project`; если изменить имя модуля в `go.mod`, необходимо также изменить импорты `project/structures`.
