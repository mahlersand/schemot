/*This file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html*/

#pragma once

#include <type_traits>
#include "ParameterPacks.h"

namespace schemot {
  template<auto ...>
  struct ValuePack;

  template<auto ...PackL, auto ...PackR>
  ValuePack<PackL ..., PackR ...> __concat_helper(ValuePack<PackL ...>, ValuePack<PackR ...>);

  template<typename PackL, typename PackR>
  using ValuePackConcat2 = decltype (__concat_helper(PackL(), PackR()));

  /*template<typename  ...Packs>
  using ValuePackConcat = std::conditional_t<sizeof... (Packs) == 0,
                            ValuePack<>,
                            std::conditional_t<sizeof... (Packs) == 1,
                              schemot::First<Packs ...>,
                              ValuePackConcat<ValuePackConcat2>
                              >>*/

  //! Valuecount-agnostic TypePack
  template<auto ...MValues>
  struct ValuePackBase
  {
    template<auto ...Values>
    using Append = ValuePack<MValues ..., Values ...>;

    template<auto ...Values>
    using Prepend = ValuePack<Values ..., MValues ...>;

    template<template<auto ...> typename Container>
    using Typeify = Container<MValues ...>;
  };

  //! 0-value TypePack Case
  template<>
  struct ValuePack<> :
      ValuePackBase<>
  {
    using Reverse = ValuePack<>;

    template<auto T>
    using FilterEquals = ValuePack<>;

    template<auto T>
    using FilterGreater = ValuePack<>;

    template<auto T>
    using FilterLess = ValuePack<>;

    using Sort = ValuePack<>;

    static bool const allsame = true;
  };

  //! General case
  template<auto MValueHead, auto ...MValueTail>
  struct ValuePack<MValueHead, MValueTail ...> :
      ValuePackBase<MValueHead, MValueTail ...>
  {
    using Reverse = typename ValuePack<MValueTail ...>::Reverse::template Append<MValueHead>;

    static auto const Head = MValueHead;

    using Tail = ValuePack<MValueTail ...>;

    using Init = typename ValuePack<MValueHead, MValueTail ...>::Reverse::Tail::Reverse;

    static auto const Last = ValuePack<MValueHead, MValueTail ...>::Reverse::Head;

    template<auto Value>
    using FilterEquals = std::conditional_t< (MValueHead == Value),
                            typename ValuePack<MValueTail ...>::template FilterEquals<Value>::template Prepend<MValueHead>,
                            typename ValuePack<MValueTail ...>::template FilterEquals<Value>>;

    template<auto Value>
    using FilterGreater = std::conditional_t< (MValueHead > Value),
                            typename ValuePack<MValueTail ...>::template FilterEquals<Value>::template Prepend<MValueHead>,
                            typename ValuePack<MValueTail ...>::template FilterEquals<Value>>;

    template<auto Value>
    using FilterLess = std::conditional_t< (Value < MValueHead),
                            typename ValuePack<MValueTail ...>::template FilterEquals<Value>::template Prepend<MValueHead>,
                            typename ValuePack<MValueTail ...>::template FilterEquals<Value>>;

    using SortLess    = typename Tail::template FilterLess<Head>::Sort;
    using SortEquals  = typename Tail::template FilterEquals<Head>::Sort;
    using SortGreater = typename Tail::template FilterGreater<Head>::Sort;

    using Sort = ValuePackConcat2<SortLess, ValuePackConcat2<SortEquals, SortGreater>>;
  };
}
