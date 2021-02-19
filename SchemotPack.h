/*This file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html*/

#pragma once

#include "TypePack.h"
#include "ValuePack.h"

namespace schemot {
  template<typename ...T>
  schemot::TypePack<T ...> __pp();

  template<auto ...I>
  schemot::ValuePack<I ...> __pp();

  #define SchemotPack(...) decltype(schemot::__pp<__VA_ARGS__>())
}

