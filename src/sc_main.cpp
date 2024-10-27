#include <systemc.h>
#include <vector>
#include "include/test_queues.hpp"

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
