#include "Efl_Eo.h"

#include "../eunit_tests.h"
#include "children_simple.h"

#include "efl-test-runner.h"

#define CHECK_ITER_DATA(iter, data, expected_data) \
   efl_assert_fail_if(!eina_iterator_next(iter, &(data))); \
   efl_assert_fail_if(expected_data != data);

static enum test_result_code
fixture_setup(struct efl_test_harness *harness)
{
   efl_object_init();

   return efl_test_harness_execute_standalone(harness);
}

DECLARE_FIXTURE_SETUP(fixture_setup);

TEST(children)
{
   Eina_Iterator *iter = NULL;
   void *chld;

   efl_object_init();

   Eo *parent = efl_add_ref(SIMPLE_CLASS, NULL);

   Eo *child1 = efl_add(SIMPLE_CLASS, parent);
   Eo *child2 = efl_add(SIMPLE_CLASS, parent);
   Eo *child3 = efl_add(SIMPLE_CLASS, parent);

   iter = efl_children_iterator_new(parent);
   efl_assert_fail_if(!iter);

   CHECK_ITER_DATA(iter, chld, child1);
   CHECK_ITER_DATA(iter, chld, child2);
   CHECK_ITER_DATA(iter, chld, child3);
   efl_assert_fail_if(eina_iterator_next(iter, &chld));

   efl_assert_fail_if(eina_iterator_container_get(iter) != parent);

   eina_iterator_free(iter);

   efl_del(child2);

   iter = efl_children_iterator_new(parent);
   efl_assert_fail_if(!iter);

   CHECK_ITER_DATA(iter, chld, child1);
   CHECK_ITER_DATA(iter, chld, child3);
   efl_assert_fail_if(eina_iterator_next(iter, &chld));

   eina_iterator_free(iter);


   efl_del(child1);

   iter = efl_children_iterator_new(parent);
   efl_assert_fail_if(!iter);

   CHECK_ITER_DATA(iter, chld, child3);
   efl_assert_fail_if(eina_iterator_next(iter,  &chld));
   eina_iterator_free(iter);


   efl_del(child3);

   iter = efl_children_iterator_new(parent);
   efl_assert_fail_if(iter);

   efl_unref(parent);

   efl_object_shutdown();
}
