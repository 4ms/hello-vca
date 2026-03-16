#pragma once
#include "conf/console_uart_conf.hh"
#include "drivers/lazy_uart.hh"
#include "drivers/uart_conf.hh"

namespace MetaModule
{

struct UartLog {
	static inline mdrivlib::LazyUart<LogUartConfig> log_uart;

	UartLog() {
		init();
	}

	static void init();

	static void putchar(char c);

	static void log(const char *format, ...);

	static void puts(const char *ptr);

	static void write_uart(const char *ptr, size_t len);
	static void write_stdout(const char *ptr, size_t len);
};

extern "C" int puts(const char *s);

} // namespace MetaModule
