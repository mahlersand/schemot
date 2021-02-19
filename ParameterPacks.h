/*This file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html*/

#pragma once

#include "SchemotPack.h"

namespace schemot {

  template<typename Head, typename ...Tail>
  using First = Head;

  //template<typename ...Types>
  //using Last = typename schemot::TypePack<Types ...>::Last;
}
