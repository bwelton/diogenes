#pragma once
#include <tuple>
#include <memory>
#include "Parameters.h"

template<typename T0>
std::tuple<T0> GetParams(std::shared_ptr<Parameters> p)
{
	return std::make_tuple((T0)(p.get()->values[0]));
}

template<typename T0,typename T1>
std::tuple<T0,T1> GetParams(std::shared_ptr<Parameters> p)
{
	return std::make_tuple((T0)(p.get()->values[0]),(T1)(p.get()->values[1]));
}

template<typename T0,typename T1,typename T2>
std::tuple<T0,T1,T2> GetParams(std::shared_ptr<Parameters> p)
{
	return std::make_tuple((T0)(p.get()->values[0]),(T1)(p.get()->values[1]),(T2)(p.get()->values[2]));
}

template<typename T0,typename T1,typename T2,typename T3>
std::tuple<T0,T1,T2,T3> GetParams(std::shared_ptr<Parameters> p)
{
	return std::make_tuple((T0)(p.get()->values[0]),(T1)(p.get()->values[1]),(T2)(p.get()->values[2]),(T3)(p.get()->values[3]));
}

template<typename T0,typename T1,typename T2,typename T3,typename T4>
std::tuple<T0,T1,T2,T3,T4> GetParams(std::shared_ptr<Parameters> p)
{
	return std::make_tuple((T0)(p.get()->values[0]),(T1)(p.get()->values[1]),(T2)(p.get()->values[2]),(T3)(p.get()->values[3]),(T4)(p.get()->values[4]));
}

template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
std::tuple<T0,T1,T2,T3,T4,T5> GetParams(std::shared_ptr<Parameters> p)
{
	return std::make_tuple((T0)(p.get()->values[0]),(T1)(p.get()->values[1]),(T2)(p.get()->values[2]),(T3)(p.get()->values[3]),(T4)(p.get()->values[4]),(T5)(p.get()->values[5]));
}

template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
std::tuple<T0,T1,T2,T3,T4,T5,T6> GetParams(std::shared_ptr<Parameters> p)
{
	return std::make_tuple((T0)(p.get()->values[0]),(T1)(p.get()->values[1]),(T2)(p.get()->values[2]),(T3)(p.get()->values[3]),(T4)(p.get()->values[4]),(T5)(p.get()->values[5]),(T6)(p.get()->values[6]));
}

template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
std::tuple<T0,T1,T2,T3,T4,T5,T6,T7> GetParams(std::shared_ptr<Parameters> p)
{
	return std::make_tuple((T0)(p.get()->values[0]),(T1)(p.get()->values[1]),(T2)(p.get()->values[2]),(T3)(p.get()->values[3]),(T4)(p.get()->values[4]),(T5)(p.get()->values[5]),(T6)(p.get()->values[6]),(T7)(p.get()->values[7]));
}

template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
std::tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8> GetParams(std::shared_ptr<Parameters> p)
{
	return std::make_tuple((T0)(p.get()->values[0]),(T1)(p.get()->values[1]),(T2)(p.get()->values[2]),(T3)(p.get()->values[3]),(T4)(p.get()->values[4]),(T5)(p.get()->values[5]),(T6)(p.get()->values[6]),(T7)(p.get()->values[7]),(T8)(p.get()->values[8]));
}

template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
std::tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9> GetParams(std::shared_ptr<Parameters> p)
{
	return std::make_tuple((T0)(p.get()->values[0]),(T1)(p.get()->values[1]),(T2)(p.get()->values[2]),(T3)(p.get()->values[3]),(T4)(p.get()->values[4]),(T5)(p.get()->values[5]),(T6)(p.get()->values[6]),(T7)(p.get()->values[7]),(T8)(p.get()->values[8]),(T9)(p.get()->values[9]));
}

template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10>
std::tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10> GetParams(std::shared_ptr<Parameters> p)
{
	return std::make_tuple((T0)(p.get()->values[0]),(T1)(p.get()->values[1]),(T2)(p.get()->values[2]),(T3)(p.get()->values[3]),(T4)(p.get()->values[4]),(T5)(p.get()->values[5]),(T6)(p.get()->values[6]),(T7)(p.get()->values[7]),(T8)(p.get()->values[8]),(T9)(p.get()->values[9]),(T10)(p.get()->values[10]));
}

template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,typename T11>
std::tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11> GetParams(std::shared_ptr<Parameters> p)
{
	return std::make_tuple((T0)(p.get()->values[0]),(T1)(p.get()->values[1]),(T2)(p.get()->values[2]),(T3)(p.get()->values[3]),(T4)(p.get()->values[4]),(T5)(p.get()->values[5]),(T6)(p.get()->values[6]),(T7)(p.get()->values[7]),(T8)(p.get()->values[8]),(T9)(p.get()->values[9]),(T10)(p.get()->values[10]),(T11)(p.get()->values[11]));
}

template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,typename T12>
std::tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12> GetParams(std::shared_ptr<Parameters> p)
{
	return std::make_tuple((T0)(p.get()->values[0]),(T1)(p.get()->values[1]),(T2)(p.get()->values[2]),(T3)(p.get()->values[3]),(T4)(p.get()->values[4]),(T5)(p.get()->values[5]),(T6)(p.get()->values[6]),(T7)(p.get()->values[7]),(T8)(p.get()->values[8]),(T9)(p.get()->values[9]),(T10)(p.get()->values[10]),(T11)(p.get()->values[11]),(T12)(p.get()->values[12]));
}

template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,typename T12,typename T13>
std::tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13> GetParams(std::shared_ptr<Parameters> p)
{
	return std::make_tuple((T0)(p.get()->values[0]),(T1)(p.get()->values[1]),(T2)(p.get()->values[2]),(T3)(p.get()->values[3]),(T4)(p.get()->values[4]),(T5)(p.get()->values[5]),(T6)(p.get()->values[6]),(T7)(p.get()->values[7]),(T8)(p.get()->values[8]),(T9)(p.get()->values[9]),(T10)(p.get()->values[10]),(T11)(p.get()->values[11]),(T12)(p.get()->values[12]),(T13)(p.get()->values[13]));
}

template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,typename T12,typename T13,typename T14>
std::tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14> GetParams(std::shared_ptr<Parameters> p)
{
	return std::make_tuple((T0)(p.get()->values[0]),(T1)(p.get()->values[1]),(T2)(p.get()->values[2]),(T3)(p.get()->values[3]),(T4)(p.get()->values[4]),(T5)(p.get()->values[5]),(T6)(p.get()->values[6]),(T7)(p.get()->values[7]),(T8)(p.get()->values[8]),(T9)(p.get()->values[9]),(T10)(p.get()->values[10]),(T11)(p.get()->values[11]),(T12)(p.get()->values[12]),(T13)(p.get()->values[13]),(T14)(p.get()->values[14]));
}
