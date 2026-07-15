#ifndef CASMJIT_LABEL_H
#define CASMJIT_LABEL_H

#include <stddef.h>
#include <stdint.h>

#define OPKIND_LABEL 1
#define LABEL_MARK_TBL_SIZE 256

typedef uint32_t labelid_t;

typedef struct {
    size_t index;
    labelid_t labelid;
} label_mark_t;

labelid_t create_label(void);
void add_label_mark(size_t index, labelid_t labelid);
size_t get_label_index(labelid_t labelid);

#endif // CASMJIT_LABEL_H
