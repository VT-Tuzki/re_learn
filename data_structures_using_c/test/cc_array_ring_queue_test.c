#include "cc_array.h"
#include "cc_array_ring_queue.h"
#include <assert.h>

#include "cc_dbg.h"
#define TEST_LEN 8

int main(void) {
    cc_array_ring_queue_t *ring;
    char i, tmp;

    assert(!cc_array_ring_queue_new(&ring, TEST_LEN, sizeof(char)));
    assert(cc_ring_queue_size(ring) == 0);
    assert(cc_ring_queue_capacity(ring) == TEST_LEN - 1);
    assert(cc_ring_queue_dequeue(ring, (void **)&tmp) == ERR_CC_QUEUE_EMPTY);

    for (i = 0; i < TEST_LEN - 1; i++) {
        // assert(!cc_ring_queue_enqueue(ring, &i));
        assert(!cc_ring_queue_enqueue(ring, &i));
        //log_info("i:%d, size:%d,cap:%d,space:%d",i,cc_ring_queue_size(ring),cc_ring_queue_capacity(ring),cc_ring_queue_space(ring));

        assert(cc_ring_queue_size(ring) == (cc_size_t)i + 1);
        assert(cc_ring_queue_space(ring) == (cc_size_t)(TEST_LEN - i - 2));
    }
    //log_info("i:%d, size:%d,cap:%d,space:%d",i,cc_ring_queue_size(ring),cc_ring_queue_capacity(ring),cc_ring_queue_space(ring));

    assert(cc_ring_queue_space(ring) == 0);
    assert(cc_ring_queue_enqueue(ring, &i) == ERR_CC_QUEUE_IS_FULL);

    assert(!cc_ring_queue_peek(ring, (void **)&i));
    assert(i == 0);
    assert(!cc_ring_queue_peek(ring, (void **)&i));
    assert(i == 0);
    assert(!cc_ring_queue_peek(ring, (void **)&i));
    assert(i == 0);

    for (i = 0; i < TEST_LEN - 1; i++) {
        // assert(!cc_ring_queue_dequeue(ring, &tmp));
        assert(!cc_ring_queue_dequeue(ring, (void **)&tmp));
        assert(tmp == i);
        assert(cc_ring_queue_size(ring) == (cc_size_t)(TEST_LEN - i - 2));
        assert(cc_ring_queue_space(ring) == (cc_size_t)i + 1);
    }

    assert(cc_ring_queue_size(ring) == 0);
    assert(cc_ring_queue_space(ring) == TEST_LEN - 1);
    assert(cc_ring_queue_dequeue(ring, (void **)&tmp) == ERR_CC_QUEUE_EMPTY);

    assert(!cc_array_ring_queue_delete(ring, NULL));

    return 0;
}
