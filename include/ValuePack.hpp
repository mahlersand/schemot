/*This file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html*/

#pragma once

#include <type_traits>

#include "packs_common.h"

namespace schemot {

  template<auto ...>
  struct ValuePackBase;

  template<auto ...>
  struct ValuePack;


  namespace __helpers {
    namespace __ValuePack {
      template<typename ...Packs>
      struct concatenate_helper;

      template<>
      struct concatenate_helper<>
      {
        using type = ValuePack<>;
      };

      template<auto ...Parameters>
      struct concatenate_helper<ValuePack<Parameters ...>>
      {
        using type = ValuePack<Parameters ...>;
      };

      template<auto ...Parameters1, auto ...Parameters2, typename ...OtherPacks>
      struct concatenate_helper<ValuePack<Parameters1 ...>, ValuePack<Parameters2 ...>, OtherPacks ...>
      {
        using type = typename concatenate_helper<ValuePack<Parameters1 ..., Parameters2 ...>, OtherPacks ...>::type;
      };
    }
  }

  /*! Quicksort for ValuePacks */
  template<typename Pack>
  using Sort = std::conditional_t<__helpers::__packs_common::same_helper_v<Pack>,
                    Pack,
                    typename Pack::SortLess::template AppendPacks<typename Pack::SortEquals, typename Pack::SortGreater>>;

  //! Valuecount-agnostic ValuePack
  template<auto ...MValues>
  struct ValuePackBase
  {
    using This = ValuePack<MValues ...>;

    template<auto ...Values>
    using Append = ValuePack<MValues ..., Values ...>;

    template<typename ...Packs>
    using AppendPacks = typename __helpers::__ValuePack::concatenate_helper<This, Packs ...>::type;

    template<auto ...Values>
    using Prepend = ValuePack<Values ..., MValues ...>;

    template<typename ...Packs>
    using PrependPacks = typename __helpers::__ValuePack::concatenate_helper<Packs ..., This>::type;

    template<template<auto ...> typename Container>
    using Typeify = Container<MValues ...>;

    static constexpr bool same = __helpers::__packs_common::same_helper_v<This>;
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
    using SortEquals  = FilterEquals<Head>; //Already sorted! Cannot call sort, lest recursion breaketh thine algorithm
    using SortGreater = Sort<FilterGreater<Head>>;
  };
}
