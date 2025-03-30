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
}

void TB::_putter_process(void) {
    std::cout << __FILE__ << "(" << __FUNCTION__ << "): --- thread: _putter_process start ---" << std::endl;

    while (true) {
        if (!_putter_queue.empty()) {
            int item = _putter_queue.front();
            _putter_queue.pop_front();
            putter(&putter_list, item);
        }
        wait(SC_ZERO_TIME);
    }
}

void TB::getter(std::vector<int>* lst, int item) {
    int result = q->get();

    std::cout << "result=" << result << ", item=" << item << std::endl;
    assert(item == result);
    lst->push_back(item);
}

void TB::_getter_process(void) {
    std::cout << __FILE__ << "(" << __FUNCTION__ << "): --- thread: _getter_process start ---" << std::endl;

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
    wait(SC_ZERO_TIME);
    std::cout << __FILE__ << "(" << __FUNCTION__ << "): --- thread: test_queue_contention start ---" << std::endl;

    // test put contention
    for (int k = 0; k < NUM_PUTTERS; k++) {
        _putter_queue.push_back(k);
    }

    std::cout << __FILE__ << "(" << __FUNCTION__ << "): _putter_queue { ";
    std::for_each(_putter_queue.begin(), _putter_queue.end(), [](int x) {
        std::cout << x << " ";
    });
    std::cout << "}" << std::endl;

    std::cout << "_putter_queue.size()=" << _putter_queue.size() << std::endl;
    while (_putter_queue.size() != 0) {
        wait(SC_ZERO_TIME);
    }

    std::cout << "_putter_queue.size()=" << _putter_queue.size() << std::endl;
    std::cout << q->qsize() << std::endl;
    assert(q->qsize() == QUEUE_SIZE);

    // test killed putter
    _putter_queue.push_back(100);
    _putter_queue.pop_back();
    _putter_queue.push_back(101);

    for (int k = 0; k < NUM_PUTTERS; k++) {
        _getter_queue.push_back(k);
    }

    _getter_queue.push_back(101);

    while (_putter_queue.size() != 0 || _getter_queue.size() != 0) {
        std::cout << "_putter_queue.size()=" << _putter_queue.size() << std::endl;
        std::cout << "_getter_queue.size()=" << _getter_queue.size() << std::endl;
        wait(SC_ZERO_TIME);
    }

    std::vector<int> list = range(NUM_PUTTERS);
    list.push_back(101);

    assert_array_equal(putter_list, list);
    assert_array_equal(getter_list, list);

    assert(q->qsize() == 0);

    _putter_queue.clear();
    _getter_queue.clear();
    putter_list.clear();
    getter_list.clear();

    // test get contension
    for (int k = 0; k < NUM_PUTTERS; k++) {
        _getter_queue.push_back(k);
    }

    // test killed getter
    _getter_queue.push_back(100);
    _getter_queue.pop_back();
    _getter_queue.push_back(101);

    for (int k = 0; k < NUM_PUTTERS; k++) {
        _putter_queue.push_back(k);
    }

    _putter_queue.push_back(101);

    while (_putter_queue.size() != 0 || _getter_queue.size() != 0) {
        std::cout << "_putter_queue.size()=" << _putter_queue.size() << std::endl;
        std::cout << "_getter_queue.size()=" << _getter_queue.size() << std::endl;
        wait(SC_ZERO_TIME);
    }

    assert_array_equal(putter_list, list);
    assert_array_equal(getter_list, list);

    assert(q->qsize() == 0);
}

void TB::test_fair_scheduling(void) {
    wait(SC_ZERO_TIME);
    std::cout << __FILE__ << "(" << __FUNCTION__ << "): --- thread: test_fair_scheduling start ---" << std::endl;

    // test put contention
    for (int k = 0; k < NUM_PUTTERS; k++) {
        _putter_queue.push_back(k);
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
