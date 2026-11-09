package main

import (
	"bufio"
	"fmt"
	"os"
	"strings"
)

const ChainCap = 101

type ChainHash struct {
	Table [ChainCap]ForwardList[string]
}

func (c *ChainHash) h(s string) int {
	return int(hashString(s) % ChainCap)
}

func (c *ChainHash) Add(s string) {
	for cur := c.Table[c.h(s)].Head; cur != nil; cur = cur.Next {
		if cur.Value == s {
			return
		}
	}
	c.Table[c.h(s)].FPushFront(s)
}

func (c *ChainHash) Del(s string) {
	c.Table[c.h(s)].FDelValue(s)
}

func (c *ChainHash) Contains(s string) bool {
	return c.Table[c.h(s)].FContains(s)
}

func (c *ChainHash) Clear() {
	for i := 0; i < ChainCap; i++ {
		c.Table[i].Clear()
	}
}

func task2(args []string) {
	var file, query string
	for i := 0; i < len(args); i++ {
		if args[i] == "--file" && i+1 < len(args) {
			file = args[i+1]
			i++
		} else if args[i] == "--query" && i+1 < len(args) {
			query = args[i+1]
			i++
		} else {
			fmt.Printf("ОШИБКА: неизвестный аргумент '%s'\n", args[i])
			return
		}
	}

	var s ChainHash

	if file != "" {
		f, err := os.Open(file)
		if err != nil {
			fmt.Printf("ОШИБКА: не удалось открыть файл %s\n", file)
			return
		}
		defer f.Close()

		sc := bufio.NewScanner(f)
		for sc.Scan() {
			line := sc.Text()
			parts := strings.Fields(line)
			if len(parts) != 2 {
				if line != "" {
					fmt.Printf("ОШИБКА: строка '%s' — ожидалось <op> <value>\n", line)
				}
				continue
			}
			op, val := parts[0], parts[1]
			if op == "SETADD" {
				s.Add(val)
			} else if op == "SETDEL" {
				s.Del(val)
			} else {
				fmt.Printf("ОШИБКА: неизвестная операция '%s'\n", op)
			}
		}
	}

	if query == "" {
		sc := bufio.NewScanner(os.Stdin)
		for sc.Scan() {
			parts := strings.Fields(sc.Text())
			if len(parts) != 2 {
				continue
			}
			op, val := parts[0], parts[1]
			if op == "SETADD" {
				s.Add(val)
			} else if op == "SETDEL" {
				s.Del(val)
			} else if op == "SET_AT" {
				if s.Contains(val) {
					fmt.Println("TRUE")
				} else {
					fmt.Println("FALSE")
				}
			} else {
				fmt.Printf("ОШИБКА: неизвестная операция '%s'\n", op)
			}
		}
	} else {
		parts := strings.Fields(query)
		if len(parts) != 2 {
			fmt.Println("ОШИБКА: запрос должен быть <op> <value>")
			return
		}
		op, val := parts[0], parts[1]
		if op == "SETADD" {
			s.Add(val)
		} else if op == "SETDEL" {
			s.Del(val)
		} else if op == "SET_AT" {
			if s.Contains(val) {
				fmt.Println("TRUE")
			} else {
				fmt.Println("FALSE")
			}
		} else {
			fmt.Printf("ОШИБКА: неизвестная операция '%s'\n", op)
			return
		}

		if file != "" {
			f, err := os.OpenFile(file, os.O_APPEND|os.O_WRONLY, 0644)
			if err == nil {
				f.WriteString(query + "\n")
				f.Close()
			}
		}
	}
}
