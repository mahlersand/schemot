#pragma once

#include "SchemotPack.h"

namespace schemot {

  template<typename Head, typename ...Tail>
  using First = Head;

  //template<typename ...Types>
  //using Last = typename schemot::TypePack<Types ...>::Last;
}
