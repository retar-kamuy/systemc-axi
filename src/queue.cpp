#include "include/queue.hpp"

template <typename T>
void Queue<T>::_at_all(void) {
    std::cout << __FILE__ << "(" << __FUNCTION__ << "): { ";
    std::for_each(_queue.begin(), _queue.end(), [](int x) {
        std::cout << x << " ";
    });
    std::cout << "}" << std::endl;
}

template void Queue<int>::_at_all(void);

template <typename T>
void Queue<T>::_put(T const& item) {
    _queue.push_back(item);
}

template void Queue<int>::_put(int const& item);

template <typename T>
T Queue<T>::_get(void) {
    T popleft = _queue.front();
    _queue.pop_front();
    std::cout << __FILE__ << "(" << __FUNCTION__ << "): get item = " << popleft << std::endl;
    return popleft;
}

template <typename T>
void Queue<T>::_wakeup_next(std::mutex* waiters, int* ns) {
    while (*ns >= 0) {
        std::cout << __FILE__ << "(" << __FUNCTION__ << "): ns=" << *ns << std::endl;
        waiters->unlock();
        *ns -= 1;
    }
}

template void Queue<int>::_wakeup_next(std::mutex* waiters, int* ns);

template <typename T>
int Queue<T>::qsize(void) {
    /**
     * Number of items in the queue.
     */
    return _queue.size();
}

template int Queue<int>::qsize(void);

template <typename T>
int Queue<T>::maxsize(void) {
    /**
     * Number of items allowed in the queue.
     */
    return _maxsize;
}

template int Queue<int>::maxsize(void);

template <typename T>
bool Queue<T>::empty(void) {
    /**
     * Return ``True`` if the queue is empty, ``False`` otherwise.
     */
    return _queue.empty();
}

template bool Queue<int>::empty(void);

template <typename T>
bool Queue<T>::full(void) {
    /**
     * Return ``True`` if there are :meth:`maxsize` items in the queue.
     * 
     * .. note::
     *     If the Queue was initialized with ``maxsize=0`` (the default), then
     *     :meth:`full` is never ``True``.
     */
    if (_maxsize <= 0) {
        return false;
    } else {
        return qsize() >= _maxsize;
    }
}

template bool Queue<int>::full(void);

template <typename T>
void Queue<T>::put(T const& item) {
    /**
     * Put an *item* into the queue.
     * 
     * If the queue is full, wait until a free
     * slot is available before adding the item.
     */
    _putters_ns++;
    while (full()) {
        _putters.lock();
        std::cout <<  __FILE__ << "(" << __FUNCTION__ << "): put" << std::endl;
    }
    put_nowait(item);
}

template void Queue<int>::put(int const& item);

template <typename T>
void Queue<T>::put_nowait(T const& item) {
    /**
     * Put an *item* into the queue without blocking.
     * 
     * If no free slot is immediately available, raise :exc:`asyncio.QueueFull`.
     */
    try {
        if (full()) {
            throw QueueFull();
        }
    } catch (const QueueFull& e) {
        std::cout << __FILE__ << "(" << __FUNCTION__ << "): " << e.what() << std::endl;
    }
    std::cout << __FILE__ << "(" << __FUNCTION__ << "): Before { ";
    std::for_each(_queue.begin(), _queue.end(), [](int x) {
        std::cout << x << " ";
    });
    std::cout << "}" << std::endl;
    _put(item);
    std::cout << __FILE__ << "(" << __FUNCTION__ << "): After { ";
    std::for_each(_queue.begin(), _queue.end(), [](int x) {
        std::cout << x << " ";
    });
    std::cout << "}" << std::endl;
    _wakeup_next(&_getters, &_getters_ns);
}

template void Queue<int>::put_nowait(int const& item);

template <typename T>
T Queue<T>::get(void) {
    /**
     * Remove and return an item from the queue.
     * 
     * If the queue is empty, wait until an item is available.
     */
    std::cout << __FILE__ << "(" << __FUNCTION__ << "): qsize= " << qsize() << std::endl;
    std::cout << __FILE__ << "(" << __FUNCTION__ << "): empty is '" << empty() << "'" << std::endl;
    _getters_ns++;
    while (empty()) {
        std::cout << __FILE__ << "(" << __FUNCTION__ << "): get" << std::endl;
        _getters.lock();
    }
    T item = get_nowait();
    return item;
}

template int Queue<int>::get(void);

template <typename T>
T Queue<T>::get_nowait(void) {
    /**
     * Remove and return an item from the queue.
     * 
     * Return an item if one is immediately available, else raise
     * :exc:`asyncio.QueueEmpty`.
     */
    try {
        if (empty()) {
            throw QueueEmpty();
        }
    } catch (const QueueEmpty& e) {
        std::cout << __FILE__ << "(" << __FUNCTION__ << "): " << e.what() << std::endl;
    }
    T item = _get();
    _wakeup_next(&_putters, &_putters_ns);
    return item;
}

template int Queue<int>::get_nowait(void);
