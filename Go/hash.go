package main

func hashString(s string) uint64 {
	var h uint64 = 0
	for i := 0; i < len(s); i++ {
		h = h*31 + uint64(s[i])
	}
	return h
}
