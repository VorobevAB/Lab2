package main

import (
	"bufio"
	"fmt"
	"os"

	"project/structures"
)

func task5() {
	in := bufio.NewReader(os.Stdin)
	var tree structures.BST

	for {
		var value int
		_, err := fmt.Fscan(in, &value)
		if err != nil || value == 0 {
			break
		}
		tree.Insert(value)
	}

	tree.PrintLeaves(tree.Root)
	fmt.Println()
	tree.Clear()
}
