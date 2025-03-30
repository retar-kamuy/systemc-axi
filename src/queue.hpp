#ifndef QUEUE_HPP_
#define QUEUE_HPP_

#include <systemc.h>
#include <deque>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <exception>
#include <future>

class QueueFull : public std::exception {
    /**
     * @brief Raised when the Queue.put_nowait() method is called on a full Queue.
     */
 public:
    const char* what() const throw() {
        return "カスタムエラーが発生しました";
    }
};

class QueueEmpty : public std::exception {
    /**
     * @brief Raised when the Queue.get_nowait() method is called on a empty Queue.
     */
 public:
    const char* what() const throw() {
        return "カスタムエラーが発生しました";
    }
};

template <typename T>
class Queue : public sc_module {
    /**
     * @brief A queue, useful for coordinating producer and consumer coroutines.
     * 
     * If *maxsize* is less than or equal to 0, the queue size is infinite. If it
     * is an integer greater than 0, then :meth:`put` will block when the queue
     * reaches *maxsize*, until an item is removed by :meth:`get`.
     */
 private:
     const char* name = "Queue";
    int _maxsize;

    sc_event _putters_event;
    sc_event _getters_event;

    std::deque<T> _queue;
    std::deque<T> _putters;
    std::deque<T> _getters;

 public:
    void _at_all(void);

    void _put(T const& item);

    T _get(void);

    void _wakeup_next(std::deque<T> waiters, sc_event* waiters_event);

    int qsize(void);

    int maxsize(void);

    bool empty(void);

    bool full(void);

    void put(T const& item);

    void _put_process(void);

    void put_nowait(T const& item);

    T get(void);

    T get_nowait(void);

    SC_CTOR(Queue, int maxsize) {
        SC_REPORT_INFO(name, "queue");
        _maxsize = maxsize;
        SC_THREAD(_put_process);
    }

    ~Queue() {
    }
};

// class PriorityQueue : public Queue {
       /**
        * A subclass of :class:`Queue`; retrieves entries in priority order (smallest item first).
        * 
        * Entries are typically tuples of the form ``(priority number, data)``.
        */
//  public:
//     template <typename T>
//     void _put(T item) {
//     }
// }

// template <typename T> class LifoQueue : public Queue {
//     /**
//      * A subclass of :class:`Queue`; retrieves most recently added entries first.
//     */
//  public:
//     void _put(T item) {
//         _queue.push_back(item);
//     }
//
//     T _get(void) {
//         return std::make_heap(_queue.front(), _queue.back());
//     }
// }

#endif  // QUEUE_HPP_
