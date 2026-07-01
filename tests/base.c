#include "kritic.h"

#include "main.h"

#define ARRAY_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))
typedef struct {
    int a, b;
} Pair;

static Pair pairs[] = { { 1, 2 }, { 3, 4 }, { -5, 5 } };
static int expected[] = { 3, 7, 0 };
KRITIC_TEST(
    base,
    add,
    KRITIC_PARAMETERIZED(pair, Pair, pairs, ARRAY_LEN(pairs)),
    KRITIC_PARAMETERIZED(result, int, expected, ARRAY_LEN(expected))
) {
    Pair p = KRITIC_GET_PARAMETER(Pair, pair);
    int res = add(p.a, p.b);
    KRITIC_ASSERT_EQ(res, KRITIC_GET_PARAMETER(int, result));
}
