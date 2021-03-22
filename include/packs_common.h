/*This file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html*/

#pragma once

#include <type_traits>

namespace schemot {

  template<auto ...>
  struct ValuePack;

  template<typename ...>
  struct TypePack;

  namespace __helpers {
    namespace __packs_common {
      using std::size_t;

      //Helper struct for same-ness
      template<typename>
      struct same_helper;

      template<auto ParHead, auto ...Pars>
      struct same_helper<ValuePack<ParHead, Pars ...>>
      {
        static auto constexpr value = ((ParHead == Pars) && ...);
      };

      template<auto Par>
      struct same_helper<ValuePack<Par>>
      {
        static auto constexpr value = true;
      };

      template<>
      struct same_helper<ValuePack<>>
      {
        static auto constexpr value = true;
      };

      template<typename ParHead, typename ...Pars>
      struct same_helper<TypePack<ParHead, Pars ...>>
      {
        static auto constexpr value = (std::is_same_v<ParHead, Pars> && ...);
      };

      template<typename Par>
      struct same_helper<TypePack<Par>>
      {
        static auto constexpr value = true;
      };

      template<>
      struct same_helper<TypePack<>>
      {
        static auto constexpr value = true;
      };

      template<typename T>
      static auto constexpr same_helper_v = same_helper<T>::value;




      //Helper struct for slices
      template<typename Pack, size_t begin, size_t end>
      requires (begin <= end)
      struct slice_helper;

      template<typename ParHead, typename ...Pars, size_t begin, size_t end>
      requires (begin > 0 && end > 0)
      struct slice_helper<TypePack<ParHead, Pars ...>, begin, end>
      {
        using type = slice_helper<TypePack<Pars ...>, begin - 1, end - 1>;
      };

      template<typename ParHead, typename ...Pars, size_t end>
      requires (end > 0)
      struct slice_helper<TypePack<ParHead, Pars ...>, 0, end>
      {
        using type = void;
      };

      template<typename ParHead, typename ...Pars>
      struct slice_helper<TypePack<ParHead, Pars ...>, 0, 0>
      {
        using type = void;
      };
    }
  }
}
