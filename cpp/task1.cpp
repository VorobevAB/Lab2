#include "task1.h"
#include "structures/forward_list.h"
#include "structures/stack.h"
#include <iostream>
using namespace std;

void task1() {
    int n, m;
    cin >> n >> m;
    ForwardList<int>* graph = new ForwardList<int>[n];
    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        --a; --b;
        graph[a].FPushFront(b);
        graph[b].FPushFront(a);
    }

    int start;
    cin >> start;
    --start;

    bool* used = new bool[n]{};
    Stack<int> stack;
    stack.Init();
    stack.SPush(start);

    while (!stack.SEmpty()) {
        int vertex = stack.STop();
        stack.SPop();
        if (used[vertex])
            continue;
        used[vertex] = true;
        cout << vertex + 1 << " ";
        for (ForwardList<int>::Node* node = graph[vertex].head; node; node = node->next)
            if (!used[node->value])
                stack.SPush(node->value);
    }
    cout << endl;

    stack.clear();
    for (int i = 0; i < n; ++i)
        graph[i].clear();
    delete[] graph;
    delete[] used;
}
