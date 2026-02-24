#include "iaq_internal.h"

void iaq_buffer_push(struct iaq_sample *sample)
{
    spin_lock(&iaq->buf_lock);

    iaq->buffer[iaq->head++] = *sample;
    iaq->head %= IAQ_BUF_SIZE;

    spin_unlock(&iaq->buf_lock);
}

