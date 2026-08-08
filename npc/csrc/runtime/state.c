#include <npc/runtime.h>

NPCState npc_state = {NPC_STOP, 0, 0};

int is_exit_status_bad(void) {
  switch (npc_state.state) {
    case NPC_QUIT:
      return 0;
    case NPC_END:
      return npc_state.halt_ret != 0;
    default:
      return 1;
  }
}
