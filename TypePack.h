/*This file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html*/

#pragma once

namespace schemot {
  template<typename ...>
  struct TypePack;

  //! Typecount-agnostic TypePack
  template<typename ...MTypes>
  struct TypePackBase
  {
    template<typename ...Types>
    using Append = TypePack<MTypes ..., Types ...>;

    template<typename ...Types>
    using Prepend = TypePack<Types ..., MTypes ...>;

    template<template<typename ...> typename Container>
    using Typeify = Container<MTypes ...>;
  };

  //! 0-type TypePack Case
  template<>
  struct TypePack<> :
      TypePackBase<>
  {
    using Reverse = TypePack<>;
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
  };
}
