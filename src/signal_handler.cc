#include "signal_handler.h"

// Local includes
#include "constants.h"

// Standard library includes
#include <cstring>

// Unix includes
#include <signal.h>

std::atomic_bool IS_SIGNAL_HANDLED = std::atomic_bool(false);

void handle_signal_fn(int sig) {
  if (sig == SIGINT || sig == SIGHUP || sig == SIGTERM) {
    IS_SIGNAL_HANDLED.store(true, std::memory_order_relaxed);
    LOG_PRINT(LogLevel::DEBUG, LogLevel::DEBUG, "DEBUG: signal handled");
  }
}

void register_signals() {
  struct sigaction sa;
  std::memset(&sa, 0, sizeof(struct sigaction));
  sigemptyset(&sa.sa_mask);
  sa.sa_handler = handle_signal_fn;

  sigaction(SIGHUP, &sa, nullptr);
  sigaction(SIGINT, &sa, nullptr);
  sigaction(SIGTERM, &sa, nullptr);
}
