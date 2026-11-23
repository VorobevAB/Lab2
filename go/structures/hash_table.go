package structures

func HashString(value string) uint64 {
	var hash uint64
	for i := 0; i < len(value); i++ {
		hash = hash*31 + uint64(value[i])
	}
	return hash
}

type HashEntry[V any] struct {
	Key   string
	Value V
}

type ChainHashMap[V any] struct {
	Table    [101]ForwardList[HashEntry[V]]
	Attempts uint64
}

func (h *ChainHashMap[V]) Init() {
	h.Attempts = 0
}

func (h *ChainHashMap[V]) hash(key string) int {
	return int(HashString(key) % uint64(len(h.Table)))
}

func (h *ChainHashMap[V]) Set(key string, value V) {
	index := h.hash(key)

	for node := h.Table[index].Head; node != nil; node = node.Next {
		h.Attempts++
		if node.Value.Key == key {
			node.Value.Value = value
			return
		}
	}

	h.Table[index].FPushFront(HashEntry[V]{Key: key, Value: value})
}

func (h *ChainHashMap[V]) Get(key string) (V, bool) {
	index := h.hash(key)

	for node := h.Table[index].Head; node != nil; node = node.Next {
		h.Attempts++
		if node.Value.Key == key {
			return node.Value.Value, true
		}
	}

	var zero V
	return zero, false
}

func (h *ChainHashMap[V]) Contains(key string) bool {
	_, found := h.Get(key)
	return found
}

func (h *ChainHashMap[V]) Del(key string) {
	index := h.hash(key)
	current := h.Table[index].Head
	var previous *Node[HashEntry[V]]

	for current != nil && current.Value.Key != key {
		previous = current
		current = current.Next
	}

	if current == nil {
		return
	}

	if previous != nil {
		previous.Next = current.Next
	} else {
		h.Table[index].Head = current.Next
	}
}

func (h *ChainHashMap[V]) Clear() {
	for i := range h.Table {
		h.Table[i].Clear()
	}
}

type StringSet struct {
	Items ChainHashMap[int]
}

func (s *StringSet) Add(key string) {
	s.Items.Set(key, 1)
}

func (s *StringSet) Del(key string) {
	s.Items.Del(key)
}

func (s *StringSet) Contains(key string) bool {
	return s.Items.Contains(key)
}

func (s *StringSet) Clear() {
	s.Items.Clear()
}

type CuckooEntry struct {
	Key   string
	Value string
	Used  bool
}

type CuckooHashMap struct {
	First  Array[CuckooEntry]
	Second Array[CuckooEntry]

	Seed1 uint64
	Seed2 uint64

	Attempts        uint64
	Displacements   uint64
	RebuildAttempts uint64
	LookupChecks    uint64
}

func (h *CuckooHashMap) Init(capacity int) {
	if capacity < 1 {
		capacity = 16
	}

	h.First.Init()
	h.Second.Init()

	for i := 0; i < capacity; i++ {
		h.First.MPush(CuckooEntry{})
		h.Second.MPush(CuckooEntry{})
	}
}

func (h *CuckooHashMap) h1(key string) int {
	return int((HashString(key) + h.Seed1) % uint64(h.First.Size))
}

func (h *CuckooHashMap) h2(key string) int {
	return int((HashString(key)*33 + h.Seed2) % uint64(h.Second.Size))
}

func (h *CuckooHashMap) Get(key string) (string, bool) {
	h.LookupChecks++
	a := h.h1(key)

	if h.First.Data[a].Used && h.First.Data[a].Key == key {
		return h.First.Data[a].Value, true
	}

	h.LookupChecks++
	b := h.h2(key)

	if h.Second.Data[b].Used && h.Second.Data[b].Key == key {
		return h.Second.Data[b].Value, true
	}

	return "", false
}

func (h *CuckooHashMap) Place(entry CuckooEntry) bool {
	current := entry
	inFirst := true
	limit := h.First.Size * 2

	for i := 0; i < limit; i++ {
		h.Attempts++

		var slot *CuckooEntry
		if inFirst {
			slot = &h.First.Data[h.h1(current.Key)]
		} else {
			slot = &h.Second.Data[h.h2(current.Key)]
		}

		if !slot.Used {
			*slot = current
			return true
		}

		h.Displacements++
		*slot, current = current, *slot
		inFirst = !inFirst
	}

	return false
}

func (h *CuckooHashMap) Rebuild(entries []CuckooEntry) {
	capacity := h.First.Size
	failures := 0

	for {
		h.First.Clear()
		h.Second.Clear()
		h.Init(capacity)

		ok := true
		for i := 0; i < len(entries); i++ {
			if !h.Place(entries[i]) {
				ok = false
				break
			}
		}

		if ok {
			return
		}

		h.RebuildAttempts++
		h.Seed1++
		h.Seed2 += 2
		failures++

		if failures%5 == 0 {
			capacity *= 2
		}
	}
}

func (h *CuckooHashMap) Set(key string, value string) {
	if _, found := h.Get(key); found {
		a := h.h1(key)
		b := h.h2(key)

		if h.First.Data[a].Used && h.First.Data[a].Key == key {
			h.First.Data[a].Value = value
		} else {
			h.Second.Data[b].Value = value
		}
		return
	}

	entries := make([]CuckooEntry, 0, h.First.Size+h.Second.Size+1)

	for i := 0; i < h.First.Size; i++ {
		if h.First.Data[i].Used {
			entries = append(entries, h.First.Data[i])
		}
	}

	for i := 0; i < h.Second.Size; i++ {
		if h.Second.Data[i].Used {
			entries = append(entries, h.Second.Data[i])
		}
	}

	entries = append(entries, CuckooEntry{
		Key: key, Value: value, Used: true,
	})

	h.Rebuild(entries)
}

func (h *CuckooHashMap) Contains(key string) bool {
	_, found := h.Get(key)
	return found
}

func (h *CuckooHashMap) Clear() {
	h.First.Clear()
	h.Second.Clear()
}
