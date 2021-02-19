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

  //! SchemotPack containing compile-time values
  template<auto ...>
  struct ValuePack;


  /*! Concatenation of ValuePacks */
  template<auto ...PackL, auto ...PackR>
  ValuePack<PackL ..., PackR ...> __concat_helper(ValuePack<PackL ...>, ValuePack<PackR ...>);

  template<typename Pack1, typename Pack2, typename ...PackTail>
  decltype (__concat_helper(Pack1(), __concat_helper(Pack2(), PackTail() ...))) __concat_helper(Pack1, Pack2, PackTail ...Tail);

  template<typename ...Packs>
  using ValuePackConcat = decltype (__concat_helper(Packs() ...));
  /* template<typename  ...Packs>
  using ValuePackConcat = std::conditional_t<sizeof... (Packs) == 0,
                            ValuePack<>,
                            std::conditional_t<sizeof... (Packs) == 1,
                              schemot::First<Packs ...>,
                              ValuePackConcat<ValuePackConcat2>
                              >>*/


  /*! Quicksort for ValuePacks */
  template<typename Pack>
  using Sort = std::conditional_t<Pack::same(),
                    Pack,
                    ValuePackConcat<typename Pack::SortLess, typename Pack::SortEquals, typename Pack::SortGreater>>;


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

    static bool consteval same()
    requires (sizeof... (MValues) < 2)
    {
      return true;
    }

    static bool consteval same()
    requires (sizeof... (MValues) >= 2)
    {
      return (ValuePack<MValues ...>::Head == ValuePack<MValues ...>::Tail::Head)
          && (ValuePack<MValues ...>::Tail::same());
    }
  };

  //! 0-value TypePack Case
  template<>
  struct ValuePack<> :
      ValuePackBase<>
  {
    //! list reversing
    using Reverse = ValuePack<>;

    //! list filtering
    template<auto>
    using FilterLess = ValuePack<>;

    template<auto>
    using FilterEquals = ValuePack<>;

    template<auto>
    using FilterGreater = ValuePack<>;

    //! sorting filtered list
    using SortLess = ValuePack<>;

    using SortEquals = ValuePack<>;

    using SortGreater = ValuePack<>;
  };

  //! General case
  template<auto MValueHead, auto ...MValueTail>
  struct ValuePack<MValueHead, MValueTail ...> :
      ValuePackBase<MValueHead, MValueTail ...>
  {
    //! list reversing
    using Reverse = typename ValuePack<MValueTail ...>::Reverse::template Append<MValueHead>;

    //! list selection
    static auto const Head = MValueHead;

    using Tail = ValuePack<MValueTail ...>;

    using Init = typename ValuePack<MValueHead, MValueTail ...>::Reverse::Tail::Reverse;

    static auto const Last = ValuePack<MValueHead, MValueTail ...>::Reverse::Head;

    //! list filtering
    template<auto Value>
    using FilterEquals = std::conditional_t< (MValueHead == Value),
                            typename ValuePack<MValueTail ...>::template FilterEquals<Value>::template Prepend<MValueHead>,
                            typename ValuePack<MValueTail ...>::template FilterEquals<Value>>;

    template<auto Value>
    using FilterGreater = std::conditional_t< (MValueHead > Value),
                            typename ValuePack<MValueTail ...>::template FilterGreater<Value>::template Prepend<MValueHead>,
                            typename ValuePack<MValueTail ...>::template FilterGreater<Value>>;

    template<auto Value>
    using FilterLess = std::conditional_t< (MValueHead < Value),
                            typename ValuePack<MValueTail ...>::template FilterLess<Value>::template Prepend<MValueHead>,
                            typename ValuePack<MValueTail ...>::template FilterLess<Value>>;

    //! sorting filtered list
    using SortLess    = Sort<FilterLess<Head>>;
    using SortEquals  = FilterEquals<Head>;
    using SortGreater = Sort<FilterGreater<Head>>;
  };
}
