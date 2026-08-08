#include <isa.h>
#include <string.h>

void init_isa() {
  memset(&cpu, 0, sizeof(cpu));
  cpu.mstatus = 3u << 11;
  cpu.mtvec = 1;
}
