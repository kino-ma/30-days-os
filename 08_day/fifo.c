#include "bootpack.h"

void fifo8_init(struct FIFO8 *fifo, unsigned char *buf, int size) {
    fifo->buf = buf;
    fifo->cur = 0;
    fifo->count = 0;
    fifo->size = size;
    fifo->flags = 0;
    return;
}

int fifo8_put(struct FIFO8 *fifo, unsigned char data) {
    if (fifo->count >= fifo->size) {
        fifo->flags |= FLAGS_OVERRUN;
        return -1;
    }

    fifo->buf[fifo->cur + fifo->count % fifo->size] = data;
    fifo->count += 1;

    return 0;
}

int fifo8_get(struct FIFO8 *fifo, int *data) {
    if (fifo->count <= 0) {
        return -1;
    }

    *data = fifo->buf[fifo->cur];
    fifo->cur += 1;
    fifo->cur %= fifo->size;
    fifo->count -= 1;

    return 0;
}