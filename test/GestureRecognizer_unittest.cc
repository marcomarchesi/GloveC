//
//  GestureRecognizer_unittest.cpp
//  GloveC
//
//  Created by Marco Marchesi on 4/13/15.
//  Copyright (c) 2015 Marco Marchesi. All rights reserved.
//

#include <stdio.h>
#include "gtest/gtest.h"
#include "GestureRecognizer.h"


// Tests factorial of negative numbers.
TEST(Gesture, Init) {
    
    GestureRecognizer  recognizer;
    
    EXPECT_EQ(1, recognizer.init());

    
    // <TechnicalDetails>
    //
    // EXPECT_EQ(expected, actual) is the same as
    //
    //   EXPECT_TRUE((expected) == (actual))
    //
    // except that it will print both the expected value and the actual
    // value when the assertion fails.  This is very helpful for
    // debugging.  Therefore in this case EXPECT_EQ is preferred.
    //
    // On the other hand, EXPECT_TRUE accepts any Boolean expression,
    // and is thus more general.
    //
    // </TechnicalDetails>
}
