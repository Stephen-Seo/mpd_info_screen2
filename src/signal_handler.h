#ifndef SEODISPARATE_COM_MPD_INFO_SCREEN_2_SIGNAL_HANDLER_H_
#define SEODISPARATE_COM_MPD_INFO_SCREEN_2_SIGNAL_HANDLER_H_

// Standard library includes
#include <atomic>

extern std::atomic_bool IS_SIGNAL_HANDLED;

void handle_signal_fn(int sig);

extern void register_signals();

#endif
