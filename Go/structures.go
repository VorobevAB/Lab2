package main

type Array[T any] struct {
	Data     []T
	Capacity int
}

func (a *Array[T]) Init() {
	a.Data = make([]T, 0, 1)
	a.Capacity = 1
}

func (a *Array[T]) MPush(n T) {
	if len(a.Data) == a.Capacity {
		nd := make([]T, len(a.Data), a.Capacity*2)
		copy(nd, a.Data)
		a.Data = nd
		a.Capacity *= 2
	}
	a.Data = append(a.Data, n)
}

func (a *Array[T]) MDelInd(idx int) {
	if idx < 0 || idx >= len(a.Data) {
		return
	}
	a.Data = append(a.Data[:idx], a.Data[idx+1:]...)
}

func (a *Array[T]) Clear() {
	a.Data = nil
	a.Capacity = 0
}

func (a *Array[T]) Size() int {
	return len(a.Data)
}

type Node[T any] struct {
	Value T
	Next  *Node[T]
}

type ForwardList[T comparable] struct {
	Head *Node[T]
}

func (l *ForwardList[T]) FPushFront(v T) {
	l.Head = &Node[T]{Value: v, Next: l.Head}
}

func (l *ForwardList[T]) FPushBack(v T) {
	n := &Node[T]{Value: v, Next: nil}
	if l.Head == nil {
		l.Head = n
		return
	}
	cur := l.Head
	for cur.Next != nil {
		cur = cur.Next
	}
	cur.Next = n
}

func (l *ForwardList[T]) FDelValue(v T) {
	cur := l.Head
	var prev *Node[T]
	for cur != nil && cur.Value != v {
		prev = cur
		cur = cur.Next
	}
	if cur == nil {
		return
	}
	if prev != nil {
		prev.Next = cur.Next
	} else {
		l.Head = cur.Next
	}
}

func (l *ForwardList[T]) FContains(v T) bool {
	for cur := l.Head; cur != nil; cur = cur.Next {
		if cur.Value == v {
			return true
		}
	}
	return false
}

func (l *ForwardList[T]) Clear() {
	l.Head = nil
}

type Stack[T any] struct {
	Head *Node[T]
}

func (s *Stack[T]) SEmpty() bool {
	return s.Head == nil
}

func (s *Stack[T]) SPush(v T) {
	s.Head = &Node[T]{Value: v, Next: s.Head}
}

func (s *Stack[T]) STop() (T, bool) {
	if s.SEmpty() {
		var zero T
		return zero, false
	}
	return s.Head.Value, true
}

func (s *Stack[T]) SPop() {
	if s.SEmpty() {
		return
	}
	s.Head = s.Head.Next
}

func (s *Stack[T]) Clear() {
	s.Head = nil
}
