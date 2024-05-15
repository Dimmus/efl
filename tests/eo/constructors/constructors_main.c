#include "Efl_Eo.h"
#include "constructors_simple.h"
#include "constructors_simple2.h"
#include "constructors_simple3.h"
#include "constructors_simple4.h"
#include "constructors_simple5.h"
#include "constructors_simple6.h"
#include "constructors_simple7.h"
#include "constructors_mixin.h"

#include "../eunit_tests.h"

#include "efl-test-runner.h"

int my_init_count = 0;

static enum test_result_code
fixture_setup(struct efl_test_harness *harness)
{
   efl_object_init();

   return efl_test_harness_execute_standalone(harness);
}

DECLARE_FIXTURE_SETUP(fixture_setup);

TEST(constructors)
{
   efl_object_init();

   Eo *obj = efl_add_ref(SIMPLE_CLASS, NULL);

   efl_assert_fail_if(my_init_count != 2);

   simple_a_set(obj, 1);
   simple_b_set(obj, 2);

   int a = 0, b = 0;
   a = simple_a_get(obj);
   b = simple_b_get(obj);
   mixin_add_and_print(obj, 5);
   efl_assert_fail_if(a != 1);
   efl_assert_fail_if(b != 2);

   efl_unref(obj);

   efl_assert_fail_if(my_init_count != 0);

   obj = efl_add_ref(SIMPLE2_CLASS, NULL);
   efl_assert_fail_if(obj);

   obj = efl_add_ref(SIMPLE3_CLASS, NULL);
   efl_assert_fail_if(obj);

   my_init_count = 0;
   obj = efl_add_ref(SIMPLE4_CLASS, NULL);

   efl_assert_fail_if(my_init_count != 2);

   efl_unref(obj);

   efl_assert_fail_if(my_init_count != 0);

   obj = efl_add_ref(SIMPLE5_CLASS, NULL);
   efl_assert_fail_if(!obj);
   efl_unref(obj);

   obj = efl_add_ref(SIMPLE6_CLASS, NULL);
   efl_assert_fail_if(!obj);
   efl_unref(obj);

   obj = efl_add_ref(SIMPLE7_CLASS, NULL);
   efl_assert_fail_if(obj);

   my_init_count = 0;
   obj = efl_add_ref(SIMPLE_CLASS, NULL);
   efl_assert_fail_if(!obj);
   efl_assert_fail_if(my_init_count != 2);
   a = simple_a_get(obj);
   efl_assert_fail_if(a != 0);

   my_init_count = 0;
   obj = efl_add_ref(SIMPLE_CLASS, NULL, simple_a_set(efl_added, 7));
   efl_assert_fail_if(!obj);
   efl_assert_fail_if(my_init_count != 2);
   a = simple_a_get(obj);
   efl_assert_fail_if(a != 7);

   my_init_count = 0;
   obj = efl_add_ref(SIMPLE_CLASS, NULL, simple_b_set(efl_added, 6), simple_a_set(efl_added, -1), b = simple_b_get(efl_added));
   efl_assert_fail_if(obj);
   efl_assert_fail_if(b != 6);
   efl_assert_fail_if(my_init_count != 0);

   efl_unref(obj);

   efl_object_shutdown();
}

