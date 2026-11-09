package main

import (
	"bufio"
	"fmt"
	"io"
)

func task4(in io.Reader) {
	count := [256]int{}
	rd := bufio.NewReader(in)

	for {
		c, err := rd.ReadByte()
		if err != nil {
			break
		}
		if c != ' ' && c != '\n' && c != '\r' {
			count[c]++
		}
	}

	mx := 0
	for i := 0; i < 256; i++ {
		if count[i] > mx {
			mx = count[i]
		}
	}

	for row := mx; row > 0; row-- {
		for i := 0; i < 256; i++ {
			if count[i] > 0 {
				if count[i] >= row {
					fmt.Print("#")
				} else {
					fmt.Print(" ")
				}
			}
		}
		fmt.Println()
	}

	for i := 0; i < 256; i++ {
		if count[i] > 0 {
			fmt.Printf("%c", i)
		}
	}
	fmt.Println()
}
