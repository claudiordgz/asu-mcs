public static <T> ListQueue<T> _shuffle(ListQueue<T> list) {
    if (list.size() <= 1) {
        return list;
    }

    ListQueue<T> left = new ListQueue<T>();
    ListQueue<T> right = new ListQueue<T>();
    while (!list.isEmpty()) {
        left.enqueue(list.dequeue());
        if (!list.isEmpty()) {
            right.enqueue(list.dequeue());
        }
    }
    left = _shuffle(left);
    right = _shuffle(right);

    return _shuffleMerge(left, right);
}

public static <T> ListQueue<T> _shuffleMerge(ListQueue<T> a, ListQueue<T> b) {
    ListQueue<T> shuffled = new ListQueue<T>();
    Random random = new Random();
    while (a.size() != 0 && b.size() != 0) {
        if (random.nextBoolean()) {
            shuffled.enqueue(a.dequeue());
        } else {
            shuffled.enqueue(b.dequeue());
        }
    }

    while(!a.isEmpty())
    {
        shuffled.enqueue(a.dequeue());
    }
    while(!b.isEmpty())
    {
        shuffled.enqueue(b.dequeue());
    }
    return shuffled;
}

@Override
public void shuffle(Object[] a) {
    if (a.length <= 1) {
        return;
    }
    ListQueue list = new ListQueue();
    for (int i = 0; i != a.length; i++) {
        list.enqueue(a[i]);
    }
    ListQueue shuffled = _shuffle(list);
    for (int i = a.length - 1; i >= 0; i--) {
        a[i] = shuffled.dequeue();
    }
}