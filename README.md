# ctest

Minimalist test framework for C.

you can define your test cases anywhere like this:
```c
#include "ctest.h"

test(Addition) {
    ctest_assert(2 + 2 == 4)
}
```

and run them like this
```c
#define CTEST_IMPLEMENTATION
#include "ctest.h"

int main() {
    ctest_run();
}
```
