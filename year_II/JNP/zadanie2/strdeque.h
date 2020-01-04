#ifndef STRDEQUE_H
#define STRDEQUE_H

#ifdef __cplusplus
#include <cstddef>
#else
#include <stddef.h>
#endif

#ifdef __cplusplus
namespace jnp1 {
    extern "C" {
#endif

unsigned long strdeque_new();
void strdeque_delete(unsigned long id);
size_t strdeque_size(unsigned long id);
void strdeque_insert_at(unsigned long id, size_t pos, const char* value);
void strdeque_remove_at(unsigned long id, size_t pos);
const char* strdeque_get_at(unsigned long id, size_t pos);
void strdeque_clear(unsigned long id);
int strdeque_comp(unsigned long id1, unsigned long id2);

#ifdef __cplusplus
    }
}
#endif

#endif
