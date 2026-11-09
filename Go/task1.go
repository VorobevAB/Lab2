package main

import (
	"bufio"
	"fmt"
)

func task1(in *bufio.Scanner) {
	var n, m int
	if !in.Scan() {
		fmt.Println("ОШИБКА: ожидалось n m")
		return
	}
	fmt.Sscan(in.Text(), &n, &m)

	if n <= 0 || m < 0 {
		fmt.Println("ОШИБКА: n > 0, m >= 0")
		return
	}

	g := make([]ForwardList[int], n)
	for i := 0; i < m; i++ {
		var a, b int
		if !in.Scan() {
			fmt.Println("ОШИБКА: ожидалось ребро a b")
			return
		}
		fmt.Sscan(in.Text(), &a, &b)

		if a < 1 || a > n || b < 1 || b > n {
			fmt.Printf("ОШИБКА: вершина вне [1..%d]\n", n)
			return
		}
		a--
		b--
		g[a].FPushFront(b)
		g[b].FPushFront(a)
	}

	var start int
	if !in.Scan() {
		fmt.Println("ОШИБКА: ожидалась стартовая вершина")
		return
	}
	fmt.Sscan(in.Text(), &start)

	if start < 1 || start > n {
		fmt.Printf("ОШИБКА: старт вне [1..%d]\n", n)
		return
	}
	start--

	used := make([]bool, n)
	st := &Stack[int]{}
	st.SPush(start)

	for !st.SEmpty() {
		v, _ := st.STop()
		st.SPop()

		if used[v] {
			continue
		}

		used[v] = true
		fmt.Printf("%d ", v+1)

		for cur := g[v].Head; cur != nil; cur = cur.Next {
			if !used[cur.Value] {
				st.SPush(cur.Value)
			}
		}
	}
	fmt.Println()
}
