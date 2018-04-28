#pragma once

#include <gtest/gtest.h>

class RegressionTest : public ::testing::Test {
protected:
    RegressionTest();
    virtual ~RegressionTest();

    virtual void SetUp();

    virtual void TearDown();
};


