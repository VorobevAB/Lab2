package main

import (
	"bufio"
	"fmt"
	"os"
	"time"

	"project/structures"
)

func isomorphicChain(a string, b string) bool {
	if len(a) != len(b) {
		return false
	}

	var forward structures.ChainHashMap[string]
	var backward structures.ChainHashMap[string]

	for i := 0; i < len(a); i++ {
		x, y := string(a[i]), string(b[i])

		mapped, found := forward.Get(x)
		if found {
			if mapped != y {
				forward.Clear()
				backward.Clear()
				return false
			}
		} else {
			forward.Set(x, y)
		}

		mapped, found = backward.Get(y)
		if found {
			if mapped != x {
				forward.Clear()
				backward.Clear()
				return false
			}
		} else {
			backward.Set(y, x)
		}
	}

	forward.Clear()
	backward.Clear()
	return true
}

func isomorphicCuckoo(a string, b string) bool {
	if len(a) != len(b) {
		return false
	}

	var forward, backward structures.CuckooHashMap
	forward.Init(16)
	backward.Init(16)

	for i := 0; i < len(a); i++ {
		x, y := string(a[i]), string(b[i])

		mapped, found := forward.Get(x)
		if found {
			if mapped != y {
				forward.Clear()
				backward.Clear()
				return false
			}
		} else {
			forward.Set(x, y)
		}

		mapped, found = backward.Get(y)
		if found {
			if mapped != x {
				forward.Clear()
				backward.Clear()
				return false
			}
		} else {
			backward.Set(y, x)
		}
	}

	forward.Clear()
	backward.Clear()
	return true
}

func task6() {
	in := bufio.NewReader(os.Stdin)

	var n int
	fmt.Fscan(in, &n)

	var chain structures.ChainHashMap[int]
	var cuckoo structures.CuckooHashMap
	cuckoo.Init(max(16, n))

	for i := 0; i < n; i++ {
		var key string
		fmt.Fscan(in, &key)
		chain.Set(key, i)
		cuckoo.Set(key, fmt.Sprintf("%d", i))
	}

	var check string
	fmt.Fscan(in, &check)

	start := time.Now()
	_, chainFound := chain.Get(check)
	chainTime := time.Since(start).Nanoseconds()

	start = time.Now()
	_, cuckooFound := cuckoo.Get(check)
	cuckooTime := time.Since(start).Nanoseconds()

	if chainFound {
		fmt.Println("Chain: FOUND")
	} else {
		fmt.Println("Chain: NOT FOUND")
	}

	if cuckooFound {
		fmt.Println("Cuckoo: FOUND")
	} else {
		fmt.Println("Cuckoo: NOT FOUND")
	}

	var a, b string
	fmt.Fscan(in, &a, &b)

	if isomorphicChain(a, b) {
		fmt.Println("Chain isomorphic: YES")
	} else {
		fmt.Println("Chain isomorphic: NO")
	}

	if isomorphicCuckoo(a, b) {
		fmt.Println("Cuckoo isomorphic: YES")
	} else {
		fmt.Println("Cuckoo isomorphic: NO")
	}

	fmt.Printf("Chain find: %d ns; comparisons: %d\n", chainTime, chain.Attempts)
	fmt.Printf("Cuckoo find: %d ns; cell checks: %d\n", cuckooTime, cuckoo.LookupChecks)

	chain.Clear()
	cuckoo.Clear()
}
