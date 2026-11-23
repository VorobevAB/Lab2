package main

import (
	"fmt"
	"os"
	"strconv"
)

func main() {
	if len(os.Args) < 2 {
		fmt.Println("Укажите номер задания: 1-7")
		return
	}

	task, err := strconv.Atoi(os.Args[1])
	if err != nil {
		fmt.Println("Нет такого задания")
		return
	}

	switch task {
	case 1:
		task1()
	case 2:
		task2(os.Args)
	case 3:
		task3()
	case 4:
		task4()
	case 5:
		task5()
	case 6:
		task6()
	case 7:
		task7()
	default:
		fmt.Println("Нет такого задания")
	}
}
