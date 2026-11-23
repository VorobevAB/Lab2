package main

import (
	"bufio"
	"fmt"
	"os"

	"project/structures"
)

type CacheItem struct {
	Key        int
	Value      int
	Frequency  int
	LastAccess int
}

type LFUCache struct {
	Data     structures.Array[CacheItem]
	Capacity int
	Timer    int
}

func (cache *LFUCache) Init(capacity int) {
	cache.Capacity = capacity
	cache.Timer = 0
	cache.Data.Init()
}

func (cache *LFUCache) Find(key int) int {
	for i := 0; i < cache.Data.Size; i++ {
		if cache.Data.Data[i].Key == key {
			return i
		}
	}
	return -1
}

func (cache *LFUCache) Get(key int) int {
	index := cache.Find(key)
	if index == -1 {
		return -1
	}

	cache.Data.Data[index].Frequency++
	cache.Timer++
	cache.Data.Data[index].LastAccess = cache.Timer
	return cache.Data.Data[index].Value
}

func (cache *LFUCache) Set(key int, value int) {
	index := cache.Find(key)

	if index != -1 {
		cache.Data.Data[index].Value = value
		cache.Data.Data[index].Frequency++
		cache.Timer++
		cache.Data.Data[index].LastAccess = cache.Timer
		return
	}

	if cache.Capacity <= 0 {
		return
	}

	if cache.Data.Size == cache.Capacity {
		removeIndex := 0

		for i := 1; i < cache.Data.Size; i++ {
			if cache.Data.Data[i].Frequency < cache.Data.Data[removeIndex].Frequency ||
				(cache.Data.Data[i].Frequency == cache.Data.Data[removeIndex].Frequency &&
					cache.Data.Data[i].LastAccess < cache.Data.Data[removeIndex].LastAccess) {
				removeIndex = i
			}
		}

		cache.Data.MDelInd(removeIndex)
	}

	cache.Timer++
	cache.Data.MPush(CacheItem{
		Key:        key,
		Value:      value,
		Frequency:  1,
		LastAccess: cache.Timer,
	})
}

func (cache *LFUCache) Clear() {
	cache.Data.Clear()
}

func task7() {
	in := bufio.NewReader(os.Stdin)

	var capacity, q int
	fmt.Fscan(in, &capacity, &q)

	var cache LFUCache
	cache.Init(capacity)

	for i := 0; i < q; i++ {
		var operation string
		var key int
		fmt.Fscan(in, &operation, &key)

		if operation == "SET" {
			var value int
			fmt.Fscan(in, &value)
			cache.Set(key, value)
		} else if operation == "GET" {
			fmt.Println(cache.Get(key))
		}
	}

	cache.Clear()
}
