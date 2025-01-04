#include "include/test_queues.hpp"

void TB::run_queue_nonblocking_test(void) {
    // queue empty
    assert(q->maxsize() >= QUEUE_SIZE);
    assert(q->qsize() >= 0);
    assert(q->empty());
    assert(!q->full());

    // put one item
    q->put_nowait(0);

    assert(q->qsize() == 1);

    assert(!q->empty());
    assert(!q->full());

    // fill queue
    // if queue_type is PriorityQueue:
    //     for k in range(QUEUE_SIZE - 1, 0, -1):
    //         q.put_nowait(k)
    // else:
    for (int k = 1; k < QUEUE_SIZE; k++) {
        q->put_nowait(k);
        std::cout << q->qsize() << std::endl;
    }

    assert(q->qsize() == QUEUE_SIZE);
    assert(!q->empty());
    assert(q->full());

    // overflow
    // with pytest.raises(QueueFull):
    //     q.put_nowait(100)

    // check queue contents
    // if queue_type is LifoQueue:
    //     for k in range(QUEUE_SIZE - 1, -1, -1):
    //         assert q.get_nowait() == k
    // else:
    for (int k = 0; k < QUEUE_SIZE; k++) {
        assert(q->get_nowait() == k);
    }

    assert(q->qsize() == 0);
    assert(q->empty());
    assert(!q->full());

    // underflow
    // with pytest.raises(QueueEmpty):
    //     q.get_nowait()
}

void TB::putter(std::vector<int>* lst, int item) {
    q->put(item);
    lst->push_back(item);
    wait(SC_ZERO_TIME);
}

void TB::getter(std::vector<int>* lst, int item) {
    int temp = q->get();
    std::cout << "temp=" << temp << ", " << "item=" << item << std::endl;
    assert(item == temp);
    lst->push_back(item);
    wait(SC_ZERO_TIME);
}

void TB::getter_process(void) {
    std::cout << __FILE__ << "(" << __FUNCTION__ << "): --- thread: thread_getter start ---" << std::endl;

    while (true) {
        if (!_getter_queue.empty()) {
            int item = _getter_queue.front();
            _getter_queue.pop_front();
            getter(&getter_list, item);
        }
        wait(SC_ZERO_TIME);
    }
}

void TB::test_queue_contention(void) {
    std::cout << __FILE__ << "(" << __FUNCTION__ << "): --- thread: test_queue_contention start ---" << std::endl;
    std::vector<int> putter_list;

    // test put contention
    wait(SC_ZERO_TIME);
    for (int k = 0; k < NUM_PUTTERS; k++) {
        putter(&putter_list, k);
    }

    q->_at_all();
    assert(q->qsize() == QUEUE_SIZE);

    // test killed putter
    // coro = cocotb.start_soon(putter(putter_list, 100))
    // coro.kill()
    putter(&putter_list, 101);

    for (int k = 0; k < NUM_PUTTERS; k++) {
        // getter(&getter_list, k);
        _getter_queue.push_back(k);
    }

    // getter(&getter_list, 101);
    _getter_queue.push_back(101);

    while (_getter_queue.size() != 0) {
        wait(SC_ZERO_TIME);
    }

    std::vector<int> list = range(NUM_PUTTERS);
    list.push_back(101);

    assert_array_equal(putter_list, list);
    assert_array_equal(getter_list, list);

    std::cout << q->qsize() << std::endl;
    assert(q->qsize() == 0);

    _getter_queue.clear();
    putter_list.clear();
    getter_list.clear();

    // test get contension
    for (int k = 0; k < NUM_PUTTERS; k++) {
        _getter_queue.push_back(k);
        wait(SC_ZERO_TIME);
    }

    // test killed getter
    // coro = cocotb.start_soon(getter(getter_list, 100))
    // coro.kill()
    _getter_queue.push_back(101);

    for (int k = 0; k < NUM_PUTTERS; k++) {
        putter(&putter_list, k);
    }

    putter(&putter_list, 101);

    while (_getter_queue.size() != 0) { // BUG
        wait(SC_ZERO_TIME);
    }

    assert_array_equal(putter_list, list);
    assert_array_equal(getter_list, list);

    std::cout << q->qsize() << std::endl;
    assert(q->qsize() == 0);
}

void TB::putter_process(void) {
    std::cout << __FILE__ << "(" << __FUNCTION__ << "): --- thread: thread_putter start ---" << std::endl;
    std::vector<int> putter_list;

    while (true) {
        std::cout << __FILE__ << "(" << __FUNCTION__ << "): --- thread: thread_putter while(true) ---" << std::endl;
        wait(event_start_putter);

        std::cout << sc_time_stamp() << " : thread_putter" << std::endl;
        for (int k = 0; k < NUM_PUTTERS; k++) {
            putter(&putter_list, k);
            wait(SC_ZERO_TIME);
        }

        // q->_at_all();
        // assert(q->qsize() == QUEUE_SIZE);

        event_end_putter.notify();
        wait(SC_ZERO_TIME);
    }
}

int sc_main(int argc, char** argv) {
    sc_clock clk("clk", 10, SC_NS, 0.5, 0, SC_NS, true);
    sc_signal<bool> rst;

    TB top("top");

    top.clk(clk);

    // top.cycle_reset();
    sc_start(100, SC_NS);
    sc_start(100, SC_NS);

    return 0;
}
