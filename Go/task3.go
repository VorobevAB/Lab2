package main

import (
	"bufio"
	"fmt"
)

type Position struct {
	A int
	B int
}

func task3(in *bufio.Scanner) {
	var n int
	if !in.Scan() {
		fmt.Println("ОШИБКА: ожидалось n")
		return
	}
	fmt.Sscan(in.Text(), &n)

	if n <= 0 {
		fmt.Println("ОШИБКА: n > 0")
		return
	}

	var positions Array[Position]
	positions.Init()

	for i := 0; i < n; i++ {
		var p Position
		if !in.Scan() {
			fmt.Printf("ОШИБКА: ожидались a b для черепахи %d\n", i+1)
			positions.Clear()
			return
		}
		fmt.Sscan(in.Text(), &p.A, &p.B)

		if p.A < 0 || p.B < 0 || p.A > n-1 || p.B > n-1 || p.A+p.B != n-1 {
			continue
		}

		found := false
		for j := 0; j < positions.Size(); j++ {
			if positions.Data[j].A == p.A && positions.Data[j].B == p.B {
				found = true
				break
			}
		}

		if !found {
			positions.MPush(p)
		}
	}

	fmt.Println(positions.Size())
	positions.Clear()
}
