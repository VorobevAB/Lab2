package main

import (
	"bufio"
	"fmt"
	"strconv"
)

type BSTNode struct {
	Value int
	Left  *BSTNode
	Right *BSTNode
}

func insertBST(root **BSTNode, v int) {
	if *root == nil {
		*root = &BSTNode{Value: v}
		return
	}
	if v < (*root).Value {
		insertBST(&(*root).Left, v)
	} else if v > (*root).Value {
		insertBST(&(*root).Right, v)
	}
}

func printLeaves(root *BSTNode) {
	if root == nil {
		return
	}
	printLeaves(root.Left)
	if root.Left == nil && root.Right == nil {
		fmt.Printf("%d ", root.Value)
	}
	printLeaves(root.Right)
}

func task5(in *bufio.Scanner) {
	var root *BSTNode

	for in.Scan() {
		token := in.Text()
		x, err := strconv.Atoi(token)
		if err != nil {
			fmt.Printf("ОШИБКА: '%s' — не целое число\n", token)
			return
		}
		if x == 0 {
			break
		}
		insertBST(&root, x)
	}

	printLeaves(root)
	fmt.Println()
}
