#pragma once
#include <tools\Common.h>

namespace
{
  class GSB_NOVTABLE Dummy GSB_ABSTRACT { };
}

template<
  typename T0 = Dummy,
  typename T1 = Dummy,
  typename T2 = Dummy,
  typename T3 = Dummy,
  typename T4 = Dummy,
  typename T5 = Dummy,
  typename T6 = Dummy,
  typename T7 = Dummy,
  typename T8 = Dummy,
  typename T9 = Dummy,
>
class GSB_NOVTABLE IPubHelp GSB_ABSTRACT : public T0, public, T1, public T2, public T3, public T4, public T5, public T6, public T7, public T8, public T9 { };