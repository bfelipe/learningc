#include <stdio.h>
#include <string.h>
#include "object.h"
#include "vm.h"

int main() {
    vm_t *vm = new_vm();
    
    // ---------------------------------------------------------
    // PHASE 1: Survival Test (Objects in a Frame)
    // ---------------------------------------------------------
    frame_t *main_frame = new_frame(vm);
    push_frame(vm, main_frame);

    object_t *life_raft = new_string(vm, "I must survive");
    frame_ref(main_frame, life_raft);

    // ---------------------------------------------------------
    // PHASE 2: Garbage Test (Floating Objects)
    // ---------------------------------------------------------
    // These objects are tracked by vm->objects but NOT in a frame.
    // They should be destroyed during collect_garbage.
    for (int i = 0; i < 5; i++) {
        new_int(vm, i * 100); 
    }

    printf("--- Phase 1 & 2 ---\n");
    printf("Objects before GC: %d (Expected: 6)\n", vm->objects->count);
    collect_garbage(vm);
    printf("Objects after GC:  %d (Expected: 1)\n", vm->objects->count);

    // ---------------------------------------------------------
    // PHASE 3: Nested Reachability (Vector3 & Array)
    // ---------------------------------------------------------
    // We only frame_ref the PARENT. The GC must trace and save the children.
    printf("\n--- Phase 3 (Tracing) ---\n");
    
    object_t *x = new_int(vm, 1);
    object_t *y = new_int(vm, 2);
    object_t *z = new_int(vm, 3);
    object_t *vec = new_vector3(vm, x, y, z);
    
    // We ONLY protect the vector itself. 
    // x, y, and z should survive because they are inside vec.
    frame_ref(main_frame, vec);

    // Create a "dead" array that isn't in a frame
    object_t *dead_arr = new_array(vm, 3);
    arr_set(dead_arr, new_float(vm, 1.1), 0);
    // Note: We do NOT call frame_ref(main_frame, dead_arr)

    printf("Objects before GC: %d (Expected: 7)\n", vm->objects->count); 
    // (1 survival string + 1 vec + 3 vec_children + 1 dead_arr + 1 dead_float)
    
    collect_garbage(vm);
    
    printf("Objects after GC:  %d (Expected: 5)\n", vm->objects->count);
    // (1 survival string + 1 vec + 3 vec_children)

    // simulate exiting main frame
    frame_t *old_frame = pop(vm->frames);
    free_frame(old_frame); // remove last roots from vm

    // Now the GC has no roots to start marking from
    printf("\n--- Final Cleanup Test ---\n");
    printf("Objects before final exit GC: %d\n", vm->objects->count);
    collect_garbage(vm);
    printf("Objects after final exit GC: %d\n", vm->objects->count);

    // Clean up VM
    free_vm(vm);
    printf("\nVM shut down successfully.\n");

    return 0;
}