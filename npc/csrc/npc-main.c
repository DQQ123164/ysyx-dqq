#include <npc/monitor.h>
void engine_start();
int is_exit_status_bad();
void delete_workspace();

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
  init_monitor(argc, argv);

  /* Start engine. */
  engine_start();

  int exit_status = is_exit_status_bad();
  delete_workspace();
  return exit_status;
}
