package graph;

/**
 * Simple FIFO Queue using circular array.
 * Used by BFS algorithm — no java.util.Queue dependency.
 */
public class SimpleQueue {

    private int[] data;
    private int head, tail, size, capacity;

    public SimpleQueue(int capacity) {
        this.capacity = capacity;
        this.data = new int[capacity];
        this.head = 0;
        this.tail = 0;
        this.size = 0;
    }

    public boolean isEmpty() {
        return size == 0;
    }

    public int getSize() {
        return size;
    }

    public void enqueue(int val) {
        if (size == capacity)
            resize(capacity * 2);
        data[tail] = val;
        tail = (tail + 1) % capacity;
        size++;
    }

    public int dequeue() {
        if (isEmpty())
            return -1;
        int val = data[head];
        head = (head + 1) % capacity;
        size--;
        return val;
    }

    public int peek() {
        if (isEmpty())
            return -1;
        return data[head];
    }

    private void resize(int newCapacity) {
        int[] newData = new int[newCapacity];
        for (int i = 0; i < size; i++) {
            newData[i] = data[(head + i) % capacity];
        }
        this.data = newData;
        this.head = 0;
        this.tail = size;
        this.capacity = newCapacity;
    }
}
