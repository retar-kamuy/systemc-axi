#ifndef TEST_QUEUES_HPP_
#define TEST_QUEUES_HPP_

#include <systemc.h>
#include <string>
#include <cassert>
#include <vector>
#include <deque>
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

    sc_event event_start_putter;
    sc_event event_end_putter;
    sc_event event_start_getter;
    sc_event event_end_getter;

    std::vector<int> getter_list;
    std::deque<int> _getter_queue;
    std::deque<int> _putter_queue;

    Queue<int> *q;

    int NUM_PUTTERS = 20;
    int QUEUE_SIZE = 10;

    void putter(std::vector<int>* lst, int item);
    void getter(std::vector<int>* lst, int item);

    void run_queue_nonblocking_test(void);
    void test_queue_contention(void);
    void putter_process(void);
    void getter_process(void);

    SC_CTOR(TB) {
        SC_REPORT_INFO(name, "test_queues");
        q = new Queue<int>("q", QUEUE_SIZE);

        // SC_THREAD(run_queue_nonblocking_test);
        SC_THREAD(test_queue_contention);
        SC_THREAD(getter_process);
    }

    ~TB() {
        delete q;
    }
};

#endif  // TEST_QUEUES_HPP_
