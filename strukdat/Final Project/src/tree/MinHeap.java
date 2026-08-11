package tree;

/**
 * Min-Heap (Priority Queue) implemented from scratch using OOP.
 * Stores (nodeId, priority) pairs.
 * Used by Dijkstra to always process the node with lowest cost next.
 *
 * Binary Heap property: parent.priority <= children.priority
 * Array representation: for node at index i,
 * left child = 2*i + 1
 * right child = 2*i + 2
 * parent = (i - 1) / 2
 */
public class MinHeap {

    public static class HeapNode {
        public int nodeId;
        public int priority;

        public HeapNode(int nodeId, int priority) {
            this.nodeId = nodeId;
            this.priority = priority;
        }
    }

    private HeapNode[] data;
    private int size;
    private int capacity;

    // position[nodeId] = index in heap array (-1 if not present)
    private int[] position;

    public MinHeap(int capacity) {
        this.capacity = capacity;
        this.data = new HeapNode[capacity];
        this.position = new int[capacity];
        this.size = 0;
        for (int i = 0; i < capacity; i++)
            position[i] = -1;
    }

    public boolean isEmpty() {
        return size == 0;
    }

    public int getSize() {
        return size;
    }

    /**
     * Insert a new (nodeId, priority) into the heap.
     */
    public void insert(int nodeId, int priority) {
        if (size == capacity) {
            resizeHeap(capacity * 2);
        }
        if (nodeId >= position.length)
            resizePositionArray(nodeId + 10);
        data[size] = new HeapNode(nodeId, priority);
        position[nodeId] = size;
        size++;
        bubbleUp(size - 1);
    }

    /**
     * Remove and return the HeapNode with minimum priority.
     */
    public HeapNode extractMin() {
        if (isEmpty())
            return null;
        HeapNode min = data[0];
        position[min.nodeId] = -1;

        // Move last element to root
        size--;
        if (size > 0) {
            data[0] = data[size];
            position[data[0].nodeId] = 0;
            data[size] = null;
            bubbleDown(0);
        } else {
            data[0] = null;
        }
        return min;
    }

    /**
     * Decrease priority of an existing node (used by Dijkstra relaxation).
     */
    public void decreaseKey(int nodeId, int newPriority) {
        int idx = position[nodeId];
        if (idx == -1)
            return; // not in heap
        if (newPriority >= data[idx].priority)
            return; // not a decrease
        data[idx].priority = newPriority;
        bubbleUp(idx);
    }

    /**
     * Check if nodeId is currently in the heap.
     */
    public boolean contains(int nodeId) {
        if (nodeId < 0 || nodeId >= capacity)
            return false;
        return position[nodeId] != -1;
    }

    /**
     * Move element at index up until heap property is restored.
     */
    private void bubbleUp(int idx) {
        while (idx > 0) {
            int parent = (idx - 1) / 2;
            if (data[parent].priority <= data[idx].priority)
                break;
            swap(idx, parent);
            idx = parent;
        }
    }

    /**
     * Move element at index down until heap property is restored.
     */
    private void bubbleDown(int idx) {
        while (true) {
            int left = 2 * idx + 1;
            int right = 2 * idx + 2;
            int smallest = idx;

            if (left < size && data[left].priority < data[smallest].priority)
                smallest = left;
            if (right < size && data[right].priority < data[smallest].priority)
                smallest = right;

            if (smallest == idx)
                break;
            swap(idx, smallest);
            idx = smallest;
        }
    }

    private void swap(int i, int j) {
        // Update position map
        position[data[i].nodeId] = j;
        position[data[j].nodeId] = i;
        // Swap data
        HeapNode tmp = data[i];
        data[i] = data[j];
        data[j] = tmp;
    }

    private void resizeHeap(int newCapacity) {
        HeapNode[] newData = new HeapNode[newCapacity];
        System.arraycopy(data, 0, newData, 0, size);
        this.data = newData;
        this.capacity = newCapacity;
    }

    private void resizePositionArray(int newLength) {
        int[] newPosition = new int[newLength];
        for (int i = 0; i < newLength; i++) {
            if (i < position.length) {
                newPosition[i] = position[i];
            } else {
                newPosition[i] = -1;
            }
        }
        this.position = newPosition;
    }

    public void printHeap() {
        System.out.print("Heap[");
        for (int i = 0; i < size; i++) {
            System.out.print("(" + data[i].nodeId + ":" + data[i].priority + ")");
            if (i < size - 1)
                System.out.print(", ");
        }
        System.out.println("]");
    }
}
