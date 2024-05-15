#ifdef HAVE_CONFIG_H
# include <efl_config.h>
#endif

#include "Efl_Eo.h"
#include "interface_simple.h"
#include "interface_interface.h"
#include "interface_interface2.h"

#include "../eunit_tests.h"

static enum test_result_code
fixture_setup(struct efl_test_harness *harness)
{
   efl_object_init();

   return efl_test_harness_execute_standalone(harness);
}

DECLARE_FIXTURE_SETUP(fixture_setup);

TEST(interface)
{
   efl_object_init();

   Eo *obj = efl_add_ref(SIMPLE_CLASS, NULL);

   simple_a_set(obj, 1);
   simple_b_set(obj, 2);

   int a = 0, b = 0, sum = 0;
   a = simple_a_get(obj);
   b = simple_b_get(obj);
   sum = interface_ab_sum_get(obj);
   efl_assert_fail_if(sum != a + b);

   sum = 0;
   sum = interface_ab_sum_get(obj);
   sum = interface_ab_sum_get(obj);
   efl_assert_fail_if(sum != a + b);
   sum = interface2_ab_sum_get2(obj);
   sum = interface2_ab_sum_get2(obj);
   efl_assert_fail_if(sum != a + b + 1);

   efl_assert_fail_if(!efl_isa(obj, INTERFACE_CLASS));
   efl_assert_fail_if(!efl_isa(obj, INTERFACE2_CLASS));
   efl_assert_fail_if(!efl_isa(SIMPLE_CLASS, INTERFACE_CLASS));
   efl_assert_fail_if(!efl_isa(SIMPLE_CLASS, INTERFACE2_CLASS));
   efl_assert_fail_if(efl_isa(INTERFACE_CLASS, INTERFACE2_CLASS));

   efl_assert_fail_if(efl_class_type_get(INTERFACE_CLASS) != EFL_CLASS_TYPE_INTERFACE);

   efl_unref(obj);
   efl_object_shutdown();
}

