package main

import (
	"bufio"
	"fmt"
	"os"

	"project/structures"
)

func task4() {
	var count structures.Array[int]
	count.Init()

	for i := 0; i < 256; i++ {
		count.MPush(0)
	}

	reader := bufio.NewReader(os.Stdin)
	line, _ := reader.ReadString('\n')

	for i := 0; i < len(line); i++ {
		value := line[i]
		if value != ' ' && value != '\n' {
			count.Data[int(value)]++
		}
	}

	maximum := 0
	for i := 0; i < 256; i++ {
		if count.Data[i] > maximum {
			maximum = count.Data[i]
		}
	}

	if maximum == 0 {
		count.Clear()
		return
	}

	for row := maximum; row > 0; row-- {
		for i := 0; i < 256; i++ {
			if count.Data[i] > 0 {
				if count.Data[i] >= row {
					fmt.Print("#")
				} else {
					fmt.Print(" ")
				}
			}
		}
		fmt.Println()
	}

	for i := 0; i < 256; i++ {
		if count.Data[i] > 0 {
			fmt.Printf("%c", byte(i))
		}
	}
	fmt.Println()

	count.Clear()
}
