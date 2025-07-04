#include <stdio.h>

#define CTEST_IMPLEMENTATION
#include "ctest.h"

test(Multiplication) {
    ctest_assert( 4 == 2*2 );
}

test(Addition) {
    ctest_assert( 5 == 2+3 );
}

int main()
{
    ctest_run();
}
