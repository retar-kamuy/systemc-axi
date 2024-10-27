#ifndef QUEUE_HPP_
#define QUEUE_HPP_

#include <systemc.h>
#include <deque>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <exception>
#include <future>

// class QueueInitiator : public tlm::tlm_bw_transport_if<> {
//  public:
//     InitiatorSocket(const sc_module_name& name, Queue* queue) {}
//  private:
//     tlm::tlm_sync_enum nb_transport_bw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_time& time) {
//         return tlm::TLM_COMPLETED;
//     }
//     void invalidate_direct_mem_ptr(sc_dt::unit64 start, sc_dt::uint64 end) {
//         return 0;
//     }
//
//     Queue parent;
// }

// class QueueTarget : public tlm::tlm_fw_transportif<> {
//  public:
//     TargetSocket(const sc_module_name& name, Queue* queue):
//     parent(queue) {}
//  private:
//     void b_transport(tlm::tlm_generic_payload& trans, sc_time& time);
//     unsigned int transport_dbg(tlm::tlm_generic_payload& trans) {
//         return 0;
//     }
//     tlm::tlm_tlm_sync_enum nb_transport_fw(tlm::tlm_generic_payload& trans, tlm::tlm_phase& phase, sc_time& time) {
//         return tlm::TLM_COMPLETED;
//     }
//     bool get_direct_mon_ptr(tlm::tlm_generic_payload& trans, tlm::tlm_dmi& dmi) {
//         return 0;
//     }
//
//     enum { SIZE = 256 };
//
//     Queue *parent;
// };

class QueueFull : public std::exception {
    /**
     * Raised when the Queue.put_nowait() method is called on a full Queue.
     */
 public:
    const char* what() const throw() {
        return "カスタムエラーが発生しました";
    }
};

class QueueEmpty : public std::exception {
    /**
     * Raised when the Queue.get_nowait() method is called on a empty Queue.
     */
 public:
    const char* what() const throw() {
        return "カスタムエラーが発生しました";
    }
};

template <typename T>
class Queue : public sc_module {
    /**
     * A queue, useful for coordinating producer and consumer coroutines.
     * 
     * If *maxsize* is less than or equal to 0, the queue size is infinite. If it
     * is an integer greater than 0, then :meth:`put` will block when the queue
     * reaches *maxsize*, until an item is removed by :meth:`get`.
     */
 private:
    int _maxsize;

    std::mutex _getters;
    std::mutex _putters;

    int _getters_ns = 0;
    int _putters_ns = 0;

    std::deque<T> _queue;

 public:
    void _at_all(void);

    void _put(T const& item);

    T _get(void);

    void _wakeup_next(std::mutex* waiters, int* ns);

    int qsize(void);

    int maxsize(void);

    bool empty(void);

    bool full(void);

    void put(T const& item);

    void put_nowait(T const& item);

    T get(void);

    T get_nowait(void);

    SC_CTOR(Queue, int maxsize) {
        _maxsize = maxsize;
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
