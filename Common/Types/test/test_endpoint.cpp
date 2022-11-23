#include "endpoint.h"

#include <gmock/gmock.h>

#include <iostream>

TEST(TestEndpoint, serialize)
{
    Endpoint endpoint("127.0.0.1",5000);

    auto bytes = endpoint.serialize();

    Endpoint endpoint2;
    endpoint2.deserialize(bytes);

    EXPECT_EQ(endpoint,endpoint2);
}