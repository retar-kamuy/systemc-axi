#ifndef RESET_HPP_
#define RESET_HPP_

#include <systemc.h>

SC_MODULE(Reset) {
 private:
    const char *name;
    bool _local_reset;
    bool _ext_reset;
    bool _reset_state;
 public:
    sc_signal<bool> reset_signal;
    sc_signal<bool> active_level;

    void _init_reset(bool _reset_signal, bool _active_level);
    void assert_reset(bool val);
    void _update_reset(void);
    virtual void _handle_reset(bool state) {}
    void _run_reset(bool _reset_signal, bool _active_level);
    void _run_reset_method(void);

    SC_CTOR(Reset) {
        name = "Reset";
        _local_reset = 0;
        _ext_reset = 0;
        _reset_state = 1;

        active_level = 1;

        SC_METHOD(_run_reset_method);
            sensitive << reset_signal;
            dont_initialize();
    }

    ~Reset() {
    }
};

#endif  // RESET_HPP_
