#ifndef TEST_QUEUES_HPP_
#define TEST_QUEUES_HPP_

#include <systemc.h>
#include <string>
#include <cassert>
#include <vector>
#include <queue.hpp>

class TB : public sc_module {
 private:
    const char* name = "TB";

 public:
    sc_in<bool> clk;
    // sc_out<bool> rst;

    std::mutex _getters;
    std::mutex _putters;

    sc_event event_start_putter;
    sc_event event_end_putter;
    sc_event event_start_getter;
    sc_event event_end_getter;

    Queue<int> *q;

    int NUM_PUTTERS = 20;
    int QUEUE_SIZE = 10;

    void putter(std::vector<int>* lst, int item);
    void getter(std::vector<int>* lst, int item);

    void run_queue_nonblocking_test(void);
    void test_queue_contention(void);
    void thread_putter(void);
    void thread_getter(void);

    SC_CTOR(TB) {
        SC_REPORT_INFO(name, "test_queues");
        q = new Queue<int>("q", QUEUE_SIZE);

        // SC_THREAD(run_queue_nonblocking_test);
        SC_THREAD(test_queue_contention);
        SC_THREAD(thread_putter);
        SC_THREAD(thread_getter);
    }

    ~TB() {
        delete q;
    }
};

#endif  // TEST_QUEUES_HPP_
