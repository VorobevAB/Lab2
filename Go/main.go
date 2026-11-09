package main

import (
	"bufio"
	"fmt"
	"os"
)

func main() {
	if len(os.Args) < 2 {
		fmt.Println("Укажите номер задания: 1-7")
		return
	}

	task := os.Args[1]
	scanner := bufio.NewScanner(os.Stdin)

	switch task {
	case "1":
		task1(scanner)
	case "2":
		task2(os.Args[2:])
	case "3":
		task3(scanner)
	case "4":
		task4(os.Stdin)
	case "5":
		task5(scanner)
	case "6":
		task6(scanner)
	case "7":
		task7(scanner)
	default:
		fmt.Println("Нет такого задания")
	}
}
