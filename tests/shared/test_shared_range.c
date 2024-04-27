/*
 * Copyright (C) 2020 Ali Alzyod
 */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <stdio.h>

#include <Efl_Shared.h>

#include "efl-test-runner.h"

TEST(range_intersect_union_test)
{
   Eina_Range r1 = EINA_RANGE(0, 10);
   Eina_Range r2 = EINA_RANGE(5, 15);

   Eina_Range r_intersect = eina_range_intersect(&r1, &r2);
   ck_assert_uint_eq(r_intersect.start, 5);
   ck_assert_uint_eq(r_intersect.length, 5);

   Eina_Range r_union = eina_range_union(&r1, &r2);
   ck_assert_uint_eq(r_union.start, 0);
   ck_assert_uint_eq(r_union.length, 20);
}

TEST(range_contains_test)
{
   Eina_Range r1 = EINA_RANGE(0, 10);

   ck_assert(eina_range_contains(&r1,0));
   ck_assert(eina_range_contains(&r1,9));
   ck_assert(!eina_range_contains(&r1,10));
}

TEST(range_equal_test)
{
   Eina_Range r1 = EINA_RANGE(0, 10);
   Eina_Range r2 = EINA_RANGE(0, 10);
   Eina_Range r3 = EINA_RANGE(0, 9);

   ck_assert(eina_range_equal(&r1, &r2));
   ck_assert(!eina_range_equal(&r1, &r3));
}


void
eina_test_range(TCase *tc)
{
   tcase_add_test(tc, eina_range_intersect_union_test);
   tcase_add_test(tc, eina_range_contains_test);
   tcase_add_test(tc, eina_range_equal_test);
}
