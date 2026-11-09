package main

import (
	"bufio"
	"fmt"
	"strconv"
)

type CuckooHash struct {
	T1    Array[string]
	T2    Array[string]
	Seed1 uint64
	Seed2 uint64
}

func NewCuckooHash(n int) *CuckooHash {
	c := &CuckooHash{Seed1: 0, Seed2: 1}
	c.initTables(n)
	return c
}

func (c *CuckooHash) initTables(n int) {
	c.T1.Init()
	c.T2.Init()
	for i := 0; i < n; i++ {
		c.T1.MPush("")
		c.T2.MPush("")
	}
}

func (c *CuckooHash) h1(s string) int {
	return int((hashString(s) + c.Seed1) % uint64(c.T1.Size()))
}

func (c *CuckooHash) h2(s string) int {
	return int((hashString(s)*33 + c.Seed2) % uint64(c.T2.Size()))
}

func (c *CuckooHash) Contains(s string) bool {
	return c.T1.Data[c.h1(s)] == s || c.T2.Data[c.h2(s)] == s
}

func (c *CuckooHash) place(s string) bool {
	cur := s
	inFirst := true
	maxLoop := 2 * c.T1.Size()

	for i := 0; i < maxLoop; i++ {
		if inFirst {
			p := c.h1(cur)
			cur, c.T1.Data[p] = c.T1.Data[p], cur
			if cur == "" {
				return true
			}
		} else {
			p := c.h2(cur)
			cur, c.T2.Data[p] = c.T2.Data[p], cur
			if cur == "" {
				return true
			}
		}
		inFirst = !inFirst
	}
	return false
}

func (c *CuckooHash) rebuild(keys *Array[string]) {
	cap := c.T1.Size()
	failed := 0
	for {
		c.T1.Clear()
		c.T2.Clear()
		c.initTables(cap)

		ok := true
		for i := 0; i < keys.Size(); i++ {
			if !c.place(keys.Data[i]) {
				ok = false
				break
			}
		}
		if ok {
			return
		}
		c.Seed1++
		c.Seed2 += 2
		failed++
		if failed%5 == 0 {
			cap *= 2
		}
	}
}

func (c *CuckooHash) Add(s string) {
	if s == "" || c.Contains(s) {
		return
	}
	var keys Array[string]
	keys.Init()
	for i := 0; i < c.T1.Size(); i++ {
		if c.T1.Data[i] != "" {
			keys.MPush(c.T1.Data[i])
		}
	}
	for i := 0; i < c.T2.Size(); i++ {
		if c.T2.Data[i] != "" {
			keys.MPush(c.T2.Data[i])
		}
	}
	keys.MPush(s)
	c.rebuild(&keys)
}

func isomorphic(a, b string) bool {
	if len(a) != len(b) {
		return false
	}
	var ab, ba [256]int
	for i := 0; i < 256; i++ {
		ab[i] = -1
		ba[i] = -1
	}
	for i := 0; i < len(a); i++ {
		x, y := a[i], b[i]
		if ab[x] != -1 && ab[x] != int(y) {
			return false
		}
		if ba[y] != -1 && ba[y] != int(x) {
			return false
		}
		ab[x] = int(y)
		ba[y] = int(x)
	}
	return true
}

func task6(in *bufio.Scanner) {
	if !in.Scan() {
		fmt.Println("ОШИБКА: ожидалось n")
		return
	}
	n, err := strconv.Atoi(in.Text())
	if err != nil || n <= 0 {
		fmt.Println("ОШИБКА: n > 0")
		return
	}

	var chain ChainHash
	cuckoo := NewCuckooHash(16)

	for i := 0; i < n; i++ {
		if !in.Scan() {
			fmt.Println("ОШИБКА: ожидался ключ")
			return
		}
		key := in.Text()
		chain.Add(key)
		cuckoo.Add(key)
	}

	if !in.Scan() {
		fmt.Println("ОШИБКА: ожидался ключ для поиска")
		return
	}
	check := in.Text()

	fmt.Print("Chain: ")
	if chain.Contains(check) {
		fmt.Println("FOUND")
	} else {
		fmt.Println("NOT FOUND")
	}

	fmt.Print("Cuckoo: ")
	if cuckoo.Contains(check) {
		fmt.Println("FOUND")
	} else {
		fmt.Println("NOT FOUND")
	}

	if !in.Scan() {
		fmt.Println("ОШИБКА: ожидались две строки для проверки изоморфизма")
		return
	}
	a := in.Text()
	if !in.Scan() {
		fmt.Println("ОШИБКА: ожидалась вторая строка")
		return
	}
	b := in.Text()

	if isomorphic(a, b) {
		fmt.Println("YES")
	} else {
		fmt.Println("NO")
	}
}
