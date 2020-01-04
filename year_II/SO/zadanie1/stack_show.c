#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>


#define CURRENT_STACK_FRAME ({ char __csf; (void*)&__csf; })
#define CALL_OPCODE ((uint8_t) 0xe8)


extern void *__libc_stack_end;
extern uint8_t __executable_start;
extern uint8_t __etext;

uint8_t* data_beginning = (uint8_t*) &__executable_start;
uint8_t* data_end = (uint8_t*) &__etext;


void stack_show() {
  void *current_stack_address = CURRENT_STACK_FRAME;

  while (current_stack_address < __libc_stack_end) {
    uint64_t curr_addr = *(uint64_t*) current_stack_address;
    uint8_t *possible_opcode = (uint8_t*) (curr_addr - 5 * sizeof(char));

    if (possible_opcode < data_end && data_beginning <= possible_opcode
        && *possible_opcode == CALL_OPCODE) {
      int32_t rel_addr = *(int32_t*) (curr_addr - 4 * sizeof(char));
      printf("%.16lx\n", curr_addr + (uint64_t) rel_addr);
    }
    
    current_stack_address++;
  }  
}

