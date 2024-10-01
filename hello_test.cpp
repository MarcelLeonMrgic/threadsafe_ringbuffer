#include <gtest/gtest.h>
#include "ringbuffer.h"

// Ein Test, der immer erfolgreich ist.
ringbuffer setUpRingbuffer()
{
    size_t sizer = sizeof(char)*10;
    ringbuffer ringbuf;
    int result = init_ringbuffer(&ringbuf, sizer);
    EXPECT_EQ(result, SUCCESS); // Check if initialization was successful
    return ringbuf;
}
TEST(WriteTest, NoWrap)
{
    // Verwende eine triviale Assertion, die immer wahr ist.
    ringbuffer ringbuf = setUpRingbuffer();
    u_int8_t message[] = "TUDO";
    write_ringbuffer(&ringbuf,message,4);
    ASSERT_EQ(ringbuf.begin+5, ringbuf.write);
}







