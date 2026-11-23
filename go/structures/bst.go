package structures

type BSTNode struct {
	Value int
	Left  *BSTNode
	Right *BSTNode
}

type BST struct {
	Root *BSTNode
}

func (tree *BST) Insert(value int) {
	current := &tree.Root

	for *current != nil {
		if value < (*current).Value {
			current = &(*current).Left
		} else if value > (*current).Value {
			current = &(*current).Right
		} else {
			return
		}
	}

	*current = &BSTNode{Value: value}
}

func (tree *BST) PrintLeaves(node *BSTNode) {
	if node == nil {
		return
	}

	tree.PrintLeaves(node.Left)

	if node.Left == nil && node.Right == nil {
		print(node.Value, " ")
	}

	tree.PrintLeaves(node.Right)
}

func (tree *BST) ClearNode(node *BSTNode) {
	if node == nil {
		return
	}
	tree.ClearNode(node.Left)
	tree.ClearNode(node.Right)
}

func (tree *BST) Clear() {
	tree.ClearNode(tree.Root)
	tree.Root = nil
}
