package structures

type ForwardList[T any] struct {
	Head *Node[T]
}

type Node[T any] struct {
	Value T
	Next  *Node[T]
}

func (list *ForwardList[T]) FPushFront(value T) {
	list.Head = &Node[T]{
		Value: value,
		Next:  list.Head,
	}
}

func (list *ForwardList[T]) FPushBack(value T) {
	node := &Node[T]{Value: value}

	if list.Head == nil {
		list.Head = node
		return
	}

	current := list.Head
	for current.Next != nil {
		current = current.Next
	}
	current.Next = node
}

func (list *ForwardList[T]) FDelValue(value T, equal func(T, T) bool) {
	current := list.Head
	var previous *Node[T]

	for current != nil && !equal(current.Value, value) {
		previous = current
		current = current.Next
	}

	if current == nil {
		return
	}

	if previous != nil {
		previous.Next = current.Next
	} else {
		list.Head = current.Next
	}
}

func (list *ForwardList[T]) Clear() {
	for list.Head != nil {
		list.Head = list.Head.Next
	}
}
