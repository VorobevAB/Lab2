package structures

type Stack[T any] struct {
	Head *StackNode[T]
}

type StackNode[T any] struct {
	Value T
	Next  *StackNode[T]
}

func (s *Stack[T]) Init() {
	s.Clear()
}

func (s *Stack[T]) SEmpty() bool {
	return s.Head == nil
}

func (s *Stack[T]) SPush(value T) {
	s.Head = &StackNode[T]{
		Value: value,
		Next:  s.Head,
	}
}

func (s *Stack[T]) STop() T {
	if s.SEmpty() {
		var zero T
		return zero
	}
	return s.Head.Value
}

func (s *Stack[T]) SPop() {
	if !s.SEmpty() {
		s.Head = s.Head.Next
	}
}

func (s *Stack[T]) Clear() {
	for s.Head != nil {
		s.Head = s.Head.Next
	}
}
