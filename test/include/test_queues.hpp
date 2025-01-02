#ifndef TEST_QUEUES_HPP_
#define TEST_QUEUES_HPP_

#include <systemc.h>
#include <string>
#include <cassert>
#include <vector>
#include "queue.hpp"

std::vector<int> range(int stop) {
    std::vector<int> vec;

    for (int i = 0; i < stop; i++) {
        vec.push_back(i);
    }
    return vec;
}

void assert_array_equal(std::vector<int> actual, std::vector<int> desired) {
    int* results;
    int mismatched = 0;

    results = new int[desired.size()];
    for (int i = 0; i < desired.size(); i++) {
        if (actual[i] == desired[i]) {
            results[i] = 0;
        } else {
            results[i] = 1;
            mismatched++;
        }
    }
    std::cout << "Mismatched elements:" << mismatched << " / " << desired.size() << std::endl;
}

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
    }

    ~TB() {
        delete q;
    }
};

#endif  // TEST_QUEUES_HPP_
