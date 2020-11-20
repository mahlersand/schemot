/*This file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html*/

#pragma once

//Parameterpack abstraction

//TypePacks depend on data types
template<typename ...>
struct TypePack;

//ValuePacks depend on integral values
template<auto ...>
struct ValuePack;

//Abstract away from that shit
template<typename ...T>
TypePack<T ...> __pp();

template<auto ...I>
ValuePack<I ...> __pp();

#define ParameterPack(...) decltype(__pp<__VA_ARGS__>())


/*
 * We have base versions of the packs for the stuff that's identical in both 0-value base cases and the general case
 */

//Base TypePack
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

//Base ValuePack
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

//0-type base case
template<>
struct TypePack<> :
        TypePackBase<>
{
    using Reverse = TypePack<>;
};

//General case
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


//0-value base case
template<>
struct ValuePack<> :
        ValuePackBase<>
{
    using Reverse = ValuePack<>;
};

//General case
template<auto ValueHead, auto ...ValueTail>
struct ValuePack<ValueHead, ValueTail ...> :
        ValuePackBase<ValueHead, ValueTail ...>
{
    using Reverse = typename ValuePack<ValueTail ...>::Reverse::template Append<ValueHead>;

    static auto const Head = ValueHead;

    using Tail = ValuePack<ValueTail ...>;

    using Init = typename ValuePack<ValueHead, ValueTail ...>::Reverse::Tail::Reverse;

    static auto const Last = ValuePack<ValueHead, ValueTail ...>::Reverse::Head;
};
