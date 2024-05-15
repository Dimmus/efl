#include <stdio.h>

#include <Efl_Eo.h>

#include "eo_suite.h"
#include "eo_test_class_simple.h"
#include "eo_test_reflection_complex_class_structure.h"


TEST(eo_test_reflection_invalid)
{
   Eina_Value numb_val = eina_value_int_init(1337);
   Eo *simple = efl_new(SIMPLE_CLASS);

   simple_a_set(simple, 22);
   efl_property_reflection_set(simple, "simple_a_asdf", numb_val);
   efl_assert_fail_if(efl_property_reflection_get(simple, "simple_a_invalid").type != EINA_VALUE_TYPE_ERROR);
}

TEST(eo_test_reflection_inherited)
{
   const int numb = 42;
   int number_ref;
   Eina_Value numb_val = eina_value_int_init(numb);
   Eo *simple = efl_new(SIMPLE3_CLASS);

   simple_a_set(simple, 22);
   efl_property_reflection_set(simple, "simple_a", numb_val);
   efl_assert_int_eq(simple_a_get(simple), numb);

   simple_a_set(simple, 22);
   Eina_Value res = efl_property_reflection_get(simple, "simple_a");
   eina_value_int_convert(&res, &number_ref);
   efl_assert_int_eq(number_ref, 22);

}

TEST(eo_test_reflection_simple)
{
   const int numb = 42;
   int number_ref;
   Eina_Value numb_val = eina_value_int_init(numb);
   Eina_Value useless_val = eina_value_int_init(7);
   Eo *simple = efl_new(SIMPLE_CLASS);

   simple_a_set(simple, 22);
   efl_property_reflection_set(simple, "simple_a", numb_val);
   efl_assert_int_eq(simple_a_get(simple), numb);

   efl_assert_int_eq(efl_property_reflection_exist(simple, "simple_a"), EFL_TRUE);

   efl_assert_int_eq(efl_property_reflection_set(simple, "should_fail", useless_val),
                    EINA_ERROR_NOT_IMPLEMENTED);

   efl_assert_int_eq(efl_property_reflection_exist(simple, "should_fail"), EFL_FALSE);

   simple_a_set(simple, 22);
   Eina_Value res = efl_property_reflection_get(simple, "simple_a");
   eina_value_int_convert(&res, &number_ref);
   efl_assert_int_eq(number_ref, 22);
}

TEST(eo_test_reflection_complex_class_structure)
{
   const int numb = 42;
   Eina_Value numb_val = eina_value_int_init(numb);
   Eo *simple = efl_new(COMPLEX_CLASS_CLASS);

   efl_property_reflection_set(simple, "m_test", numb_val);
   efl_property_reflection_set(simple, "i_test", numb_val);

   efl_assert_int_eq(complex_mixin_m_test_get(simple), numb);
   efl_assert_int_eq(complex_interface_i_test_get(simple), numb);
}

#include "eo_test_reflection_complex_class_structure.c"
