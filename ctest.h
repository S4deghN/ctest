// TODO:
// [ ] Add timing for each unit test.
// [ ] Add diagnostics mode where all asserts are printed with detail.
// [ ] Add ability to select spesific test by file or substring (or maybe regex).
//
#ifndef CTEST_H
#define CTEST_H

#define FMT_TITLE(str) "\033[34m"str"\033[0m"
#define FMT_RED(str) "\033[31m"str"\033[0m"
#define FMT_GREEN(str) "\033[32m"str"\033[0m"
#define FMT_DIM(str) "\033[37m"str"\033[0m"

typedef struct CtestStruct CtestStruct;
typedef void (*CtestCallback)(CtestStruct*);
struct CtestStruct {
    const char* name;
    CtestCallback cb;
    int status;
};

#define test(name)                                  \
    void _ctest_##name##_cb(CtestStruct*);          \
    __attribute__((section ("_ctest")))             \
    __attribute__((aligned (alignof(CtestStruct)))) \
    CtestStruct _ctest_##name= {                    \
         #name,                                     \
        _ctest_##name##_cb,                         \
        0                                           \
    };                                              \
    void _ctest_##name##_cb(CtestStruct* ctest)

#define ctest_assert(cond) (cond) ? \
        _ctest_assert_pass() : \
        _ctest_assert_fail(ctest, #cond, __FILE__, __LINE__)

void _ctest_assert_pass(void);
void _ctest_assert_fail(CtestStruct* ctest, const char* cond_str, const char* file, int line_number);
void ctest_run();

#ifdef CTEST_IMPLEMENTATION
#include <stdio.h>

extern CtestStruct __start__ctest;
extern CtestStruct __stop__ctest;
static int _ctest_fails;
static int _ctest_passes;

void _ctest_assert_pass()
{
    ++_ctest_passes;
}
void _ctest_assert_fail(CtestStruct* ctest, const char* cond_str, const char* file, int line_number)
{
    ++_ctest_fails;
    if (ctest->status == 0) {
        ctest->status = -1;
        printf(FMT_RED(" FAIL\n"));
    }
    printf(FMT_DIM("  %s:%d:") " "FMT_RED("%s")"\n", file, line_number, cond_str);
}

void ctest_run()
{
    for (CtestStruct *ctest = &__start__ctest; ctest < &__stop__ctest; ++ctest) {
        printf("%s", ctest->name);
        ctest->cb(ctest);
        if (ctest->status == 0) {
            printf(FMT_GREEN(" PASS\n"));
        }
    }
    printf("----------\n");
    if (_ctest_fails) {
        printf("Fails: "FMT_RED("%d")"\n", _ctest_fails);
    } else {
        printf("Fails: "FMT_GREEN("%d")"\n", _ctest_fails);
    }

    printf("Total: "FMT_TITLE("%d")"\n", _ctest_fails + _ctest_passes);
}
#endif

#endif
