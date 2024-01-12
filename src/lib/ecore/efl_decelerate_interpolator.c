#ifdef HAVE_CONFIG_H
# include "efl_config.h"
#endif

#include "Ecore.h"
#include "ecore_private.h"

#define MY_CLASS EFL_DECELERATE_INTERPOLATOR_CLASS

typedef struct _Efl_Decelerate_Interpolator_Data Efl_Decelerate_Interpolator_Data;

struct _Efl_Decelerate_Interpolator_Data
{
   double slope;
};

EOLIAN static double
_efl_decelerate_interpolator_efl_interpolator_interpolate(Eo *eo_obj EINA_UNUSED,
                                                          Efl_Decelerate_Interpolator_Data *pd EINA_UNUSED,
                                                          double progress)
{
   if ((progress < 0.0) || (progress > 1.0))
     return progress;

   return ecore_animator_pos_map(progress, ECORE_POS_MAP_DECELERATE_FACTOR,
                                 pd->slope, 0);
}

EOLIAN static void
_efl_decelerate_interpolator_slope_set(Eo *eo_obj EINA_UNUSED,
                                        Efl_Decelerate_Interpolator_Data *pd,
                                        double slope)
{
   pd->slope = slope;
}

EOLIAN static double
_efl_decelerate_interpolator_slope_get(const Eo *eo_obj EINA_UNUSED,
                                        Efl_Decelerate_Interpolator_Data *pd EINA_UNUSED)
{
   return pd->slope;
}

EOLIAN static Efl_Object *
_efl_decelerate_interpolator_efl_object_constructor(Eo *eo_obj,
                                                    Efl_Decelerate_Interpolator_Data *pd)
{
   eo_obj = efl_constructor(efl_super(eo_obj, MY_CLASS));

   pd->slope = 1.0;

   return eo_obj;
}

#include "efl_decelerate_interpolator.eo.c"
