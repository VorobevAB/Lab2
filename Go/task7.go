package main

import (
	"bufio"
	"fmt"
	"strconv"
	"strings"
)

type CacheItem struct {
	Key   int
	Value int
	Freq  int
	Last  int
}

type LFUCache struct {
	Data     Array[CacheItem]
	Capacity int
	Timer    int
}

func NewLFUCache(cap int) *LFUCache {
	c := &LFUCache{Capacity: cap}
	c.Data.Init()
	return c
}

func (c *LFUCache) find(key int) int {
	for i := 0; i < c.Data.Size(); i++ {
		if c.Data.Data[i].Key == key {
			return i
		}
	}
	return -1
}

func (c *LFUCache) Get(key int) int {
	idx := c.find(key)
	if idx == -1 {
		return -1
	}
	c.Timer++
	c.Data.Data[idx].Freq++
	c.Data.Data[idx].Last = c.Timer
	return c.Data.Data[idx].Value
}

func (c *LFUCache) Set(key, value int) {
	if c.Capacity <= 0 {
		return
	}
	idx := c.find(key)
	if idx != -1 {
		c.Timer++
		c.Data.Data[idx].Value = value
		c.Data.Data[idx].Freq++
		c.Data.Data[idx].Last = c.Timer
		return
	}
	if c.Data.Size() == c.Capacity {
		del := 0
		for i := 1; i < c.Data.Size(); i++ {
			if c.Data.Data[i].Freq < c.Data.Data[del].Freq ||
				(c.Data.Data[i].Freq == c.Data.Data[del].Freq && c.Data.Data[i].Last < c.Data.Data[del].Last) {
				del = i
			}
		}
		c.Data.MDelInd(del)
	}
	c.Timer++
	c.Data.MPush(CacheItem{Key: key, Value: value, Freq: 1, Last: c.Timer})
}

func task7(in *bufio.Scanner) {
	if !in.Scan() {
		fmt.Println("ОШИБКА: ожидалось cap q")
		return
	}
	parts := strings.Fields(in.Text())
	if len(parts) != 2 {
		fmt.Println("ОШИБКА: ожидалось cap q")
		return
	}
	cap, _ := strconv.Atoi(parts[0])
	q, _ := strconv.Atoi(parts[1])

	if cap < 0 || q < 0 {
		fmt.Println("ОШИБКА: cap >= 0, q >= 0")
		return
	}

	cache := NewLFUCache(cap)
	for i := 0; i < q; i++ {
		if !in.Scan() {
			fmt.Printf("ОШИБКА: ожидалось <op> <key> в запросе #%d\n", i+1)
			return
		}
		parts := strings.Fields(in.Text())
		if len(parts) < 2 {
			fmt.Printf("ОШИБКА: ожидалось <op> <key> в запросе #%d\n", i+1)
			return
		}
		op := parts[0]
		x, _ := strconv.Atoi(parts[1])

		if op == "SET" {
			if len(parts) < 3 {
				fmt.Println("ОШИБКА: SET требует ключ и значение")
				return
			}
			y, _ := strconv.Atoi(parts[2])
			cache.Set(x, y)
		} else if op == "GET" {
			fmt.Println(cache.Get(x))
		} else {
			fmt.Printf("ОШИБКА: неизвестная операция '%s'\n", op)
			return
		}
	}
}
