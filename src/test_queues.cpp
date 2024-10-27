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
    std::lock_guard<std::mutex> lock(_putters);
    q->put(item);
    lst->push_back(item);
}

void TB::getter(std::vector<int>* lst, int item) {
    std::lock_guard<std::mutex> lock(_getters);
    int temp = q->get();
    std::cout << __FILE__ << "(" << __FUNCTION__ << "): item=" << item << ",q.get()=" << temp << std::endl;
    // assert(item == temp);
    lst->push_back(item);
    std::cout << __FILE__ << "(" << __FUNCTION__ << "): { ";
    std::for_each(lst->begin(), lst->end(), [](int x) {
        std::cout << x << " ";
    });
    std::cout << "}" << std::endl;
}

void TB::test_queue_contention(void) {
    std::cout << __FILE__ << "(" << __FUNCTION__ << "): --- thread: test_queue_contention start ---" << std::endl;
    std::vector<std::thread> coro_list;
    std::vector<int> putter_list;
    std::vector<int> getter_list;

    // test put contention
    for (int k = 0; k < NUM_PUTTERS; k++) {
        coro_list.push_back(std::thread([&]{ putter(&putter_list, k); }));
    }

    q->_at_all();
    assert(q->qsize() == QUEUE_SIZE);

    // test killed putter
    std::cout << "putter_list.size()=" << putter_list.size() << std::endl;
    std::thread coro([&]{ putter(&putter_list, 100); });
    std::cout << "putter_list.size()=" << putter_list.size() << std::endl;
    q->_at_all();
    coro.detach();
    q->_at_all();
    std::cout << "putter_list.size()=" << putter_list.size() << std::endl;
    coro_list.push_back(std::thread([&]{ putter(&putter_list, 101); }));
    std::cout << "putter_list.size()=" << putter_list.size() << std::endl;
    q->_at_all();

    for (int k = 0; k < NUM_PUTTERS; k++) {
        std::cout << "get k= " << k << std::endl;
        coro_list.push_back(std::thread([&]{ getter(&getter_list, k); }));
    }

    std::cout <<  "putter_list = { ";
    std::for_each(putter_list.begin(), putter_list.end(), [](int x) {
        std::cout << x << " ";
    });
    std::cout << "}" << std::endl;

    std::cout <<  "getter_list = { ";
    std::for_each(getter_list.begin(), getter_list.end(), [](int x) {
        std::cout << x << " ";
    });
    std::cout << "}" << std::endl;

    for (std::thread& th : coro_list) {
        th.join();
    }

    /*
    for (int k = 0; k < NUM_PUTTERS; k++) {
        std::cout << "k= " << k << std::endl;
        coro_list.push_back(std::thread([this, getter_list, k]{ getter(getter_list, k); }));
    }

    coro_list.push_back(std::thread([this, getter_list]{ getter(getter_list, 101); }));

    for (int p : putter_list) {
        std::cout << p << ", ";
    }
    std::cout << std::endl;

    std::cout << "test1" << std::endl;
    for (std::thread &th : coro_list) {
        th.join();
    }
    std::cout << "test2" << std::endl;*/
}

void TB::thread_putter(void) {
    std::cout << __FILE__ << "(" << __FUNCTION__ << "): --- thread: thread_putter start ---" << std::endl;
    std::vector<int> putter_list;

    // while (true) {
        std::cout << __FILE__ << "(" << __FUNCTION__ << "): --- thread: thread_putter while(true) ---" << std::endl;
        // wait(event_start_putter);

        std::cout << sc_time_stamp() << " : thread_putter" << std::endl;
        for (int k = 0; k < NUM_PUTTERS; k++) {
            putter(&putter_list, k);
            // wait(SC_ZERO_TIME);
        }

        // q->_at_all();
        // assert(q->qsize() == QUEUE_SIZE);

        event_end_putter.notify();
        // wait(SC_ZERO_TIME);
    // }
}

void TB::thread_getter(void) {
    std::cout << __FILE__ << "(" << __FUNCTION__ << "): --- thread: thread_getter start ---" << std::endl;
    std::vector<int> getter_list;

    while (true) {
        wait(event_start_getter);

        std::cout << sc_time_stamp() << " : thread_getter" << std::endl;
        for (int k = 0; k < NUM_PUTTERS; k++) {
            std::cout << sc_time_stamp() << " k=" << k << std::endl;
            getter(&getter_list, k);
            wait(SC_ZERO_TIME);
        }

        event_end_getter.notify();
    }
}
