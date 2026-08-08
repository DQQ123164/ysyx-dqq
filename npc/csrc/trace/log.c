#include <stdarg.h>
#include <npc/diagnostics.h>

extern uint64_t guest_insts;
uint64_t get_sim_time();

FILE *log_fp = NULL;
static FILE *trace_streams[TRACE_CHANNEL_COUNT] = {};
static const char *trace_tags[TRACE_CHANNEL_COUNT] = { "[ITRACE] ", "[FTRACE] ", "[ETRACE] ", "[MTRACE] ", "[DTRACE] ",};

void init_log(const char *log_file) {
  log_fp = NULL;
  if (log_file != NULL) {
    log_fp = fopen(log_file, "w");
    Assert(log_fp != NULL, "Can not open '%s'", log_file);
  }
}

bool log_enable() {
  return MUXDEF(CONFIG_TRACE, (guest_insts >= CONFIG_TRACE_START) &&
         (guest_insts <= CONFIG_TRACE_END), false);
}

void init_trace_log(TraceKind kind, const char *log_file) {
  if (kind >= TRACE_CHANNEL_COUNT || log_file == NULL) return;
  trace_streams[kind] = fopen(log_file, "w");
  Assert(trace_streams[kind] != NULL, "Can not open '%s'", log_file);
}

static void write_record(FILE *stream, TraceKind kind, uint64_t time,
    const char *format, va_list args) {
  if (stream == NULL) return;
  fprintf(stream, "[%9" PRIu64 "] %s", time, trace_tags[kind]);
  vfprintf(stream, format, args);
  fflush(stream);
}

void trace_write(TraceKind kind, const char *format, ...) {
  if (!log_enable() || kind >= TRACE_CHANNEL_COUNT) return;

  va_list args;
  va_start(args, format);
  uint64_t time = get_sim_time();

  if (log_fp != NULL) {
    va_list copy;
    va_copy(copy, args);
    write_record(log_fp, kind, time, format, copy);
    va_end(copy);
  }
  if (trace_streams[kind] != NULL) {
    va_list copy;
    va_copy(copy, args);
    write_record(trace_streams[kind], kind, time, format, copy);
    va_end(copy);
  }
  va_end(args);
}
