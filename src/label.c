#include <assert.h>

#include "label.h"

static labelid_t labelid = 1;

static label_mark_t marks[LABEL_MARK_TBL_SIZE] = { 0 };
static size_t marks_index = 0;

labelid_t create_label(void) {
    return labelid++;
}

void add_label_mark(size_t index, labelid_t labelid) {
    assert(
        marks_index < (LABEL_MARK_TBL_SIZE - 1) && "Label mark table filled!"
    );

    marks[marks_index++] = (label_mark_t) { .index = index,
                                            .labelid = labelid };
}

size_t get_label_index(labelid_t labelid) {
    for (size_t i = 0; i < marks_index; i++) {
        if (labelid == marks[i].labelid) {
            return marks[i].index;
        }
    }

    return 0;
}
