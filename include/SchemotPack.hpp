/*This file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html*/

#pragma once

#include "TypePack.hpp"
#include "ValuePack.hpp"

namespace schemot {
  namespace __helpers {

    namespace __SchemotPack {
      template<typename ...T>
      schemot::TypePack<T ...> __pp();

      template<auto ...I>
      schemot::ValuePack<I ...> __pp();
    } //__SchemotPack

    namespace __Packify {
      template<typename T>
      struct packify_helper;

      template<template<typename ...> typename T, typename ...Parameters>
      struct packify_helper<T<Parameters ...>>
      {
        using type = TypePack<Parameters ...>;
      };

      template<template<auto ...> typename T, auto ...Parameters>
      struct packify_helper<T<Parameters ...>>
      {
        using type = ValuePack<Parameters ...>;
      };
    } //__Packify

  } //__helpers
} //schemot

#define SchemotPack(...) decltype(schemot::__helpers::__SchemotPack::__pp<__VA_ARGS__>())
#define SchemotPackify(...) schemot::__helpers::__Packify::packify_helper<__VA_ARGS__>::type
