package structures

type Array[T any] struct {
	Data     []T
	Size     int
	Capacity int
}

func (a *Array[T]) Init() {
	a.Capacity = 1
	a.Size = 0
	a.Data = make([]T, 0, a.Capacity)
}

func (a *Array[T]) MPush(value T) {
	if a.Size == a.Capacity {
		a.Capacity *= 2
		next := make([]T, a.Size, a.Capacity)
		copy(next, a.Data)
		a.Data = next
	}

	a.Data = append(a.Data, value)
	a.Size++
}

func (a *Array[T]) MDelInd(index int) {
	if index < 0 || index >= a.Size {
		return
	}

	copy(a.Data[index:], a.Data[index+1:])
	var zero T
	a.Data[a.Size-1] = zero
	a.Data = a.Data[:a.Size-1]
	a.Size--
}

func (a *Array[T]) Clear() {
	a.Data = nil
	a.Size = 0
	a.Capacity = 0
}
