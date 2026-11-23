package main

import (
	"bufio"
	"fmt"
	"os"

	"project/structures"
)

func task1() {
	in := bufio.NewReader(os.Stdin)

	var n, m int
	fmt.Fscan(in, &n, &m)

	graph := make([]structures.ForwardList[int], n)

	for i := 0; i < m; i++ {
		var a, b int
		fmt.Fscan(in, &a, &b)
		a--
		b--
		graph[a].FPushFront(b)
		graph[b].FPushFront(a)
	}

	var start int
	fmt.Fscan(in, &start)
	start--

	used := make([]bool, n)
	var stack structures.Stack[int]
	stack.Init()
	stack.SPush(start)

	out := bufio.NewWriter(os.Stdout)
	defer out.Flush()

	for !stack.SEmpty() {
		vertex := stack.STop()
		stack.SPop()

		if used[vertex] {
			continue
		}

		used[vertex] = true
		fmt.Fprint(out, vertex+1, " ")

		for node := graph[vertex].Head; node != nil; node = node.Next {
			if !used[node.Value] {
				stack.SPush(node.Value)
			}
		}
	}

	fmt.Fprintln(out)
	stack.Clear()

	for i := range graph {
		graph[i].Clear()
	}
}
