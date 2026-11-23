package main

import (
	"bufio"
	"fmt"
	"os"

	"project/structures"
)

func task3() {
	in := bufio.NewReader(os.Stdin)

	var n int
	fmt.Fscan(in, &n)

	var positions structures.StringSet
	count := 0

	for i := 0; i < n; i++ {
		var ahead, behind int
		fmt.Fscan(in, &ahead, &behind)

		if ahead < 0 || behind < 0 || ahead+behind != n-1 {
			continue
		}

		key := fmt.Sprintf("%d:%d", ahead, behind)
		if !positions.Contains(key) {
			positions.Add(key)
			count++
		}
	}

	fmt.Println(count)
	positions.Clear()
}
