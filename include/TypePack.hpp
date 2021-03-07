/*This file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html*/

#pragma once

#include <type_traits>
#include <utility>

namespace schemot {

  template<typename ...>
  struct TypePackBase;

  template<typename ...>
  struct TypePack;

  namespace __helpers {
    namespace __TypePack {
      template<typename Container>
      struct packify_helper;

      template<template<typename ...> typename Container, typename ...Parameters>
      struct packify_helper<Container<Parameters ...>>
      {
        using type = TypePack<Parameters ...>;
      };

      template<typename ...Packs>
      struct concatenate_helper;

      template<>
      struct concatenate_helper<>
      {
        using type = TypePack<>;
      };

      template<typename ...Parameters>
      struct concatenate_helper<TypePack<Parameters ...>>
      {
        using type = TypePack<Parameters ...>;
      };

      template<typename ...Parameters1, typename ...Parameters2, typename ...OtherPacks>
      struct concatenate_helper<TypePack<Parameters1 ...>, TypePack<Parameters2 ...>, OtherPacks ...>
      {
        using type = typename concatenate_helper<TypePack<Parameters1 ..., Parameters2 ...>, OtherPacks ...>::type;
      };
    }
  }




  //! Typecount-agnostic TypePack
  template<typename ...MTypes>
  struct TypePackBase
  {
    using This = TypePack<MTypes ...>;

    template<typename ...Types>
    using Append = TypePack<MTypes ..., Types ...>;

    template<typename ...Packs>
    using AppendPacks = typename __helpers::__TypePack::concatenate_helper<This, Packs ...>::type;

    template<typename ...Types>
    using Prepend = TypePack<Types ..., MTypes ...>;

    template<typename ...Packs>
    using PrependPacks = typename __helpers::__TypePack::concatenate_helper<Packs ..., This>::type;

    template<template<typename ...> typename Container>
    using Typeify = Container<MTypes ...>;
  };

  //! 0-type TypePack Case
  template<>
  struct TypePack<> :
      TypePackBase<>
  {
    using Reverse = TypePack<>;

    template<typename T>
    struct unsafe {
      using UHead = T;
      using UTail = T;
      using UInit = T;
      using ULast = T;
    };
  };

  //! General case
  template<typename TypeHead, typename ...TypeTail>
  struct TypePack<TypeHead, TypeTail ...> :
      TypePackBase<TypeHead, TypeTail ...>
  {
    using Reverse = typename TypePack<TypeTail ...>::Reverse::template Append<TypeHead>;

    using Head = TypeHead;

    using Tail = TypePack<TypeTail ...>;

    using Init = typename Reverse::Tail::Reverse;

    using Last = typename Reverse::Head;

    template<typename T>
    struct unsafe {
      using UHead = Head;
      using UTail = Tail;
      using UInit = Init;
      using ULast = Last;
    };
  };
}
