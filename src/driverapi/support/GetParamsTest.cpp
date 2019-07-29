#include <vector>
#include <utility>
#include <array>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <tuple>
#include <functional>
#include <memory>

int test(int a, std::string b, int q) {
	std::cout << a << ", "<< b <<"," << q << std::endl;
	return 0;
}

int test2(double t, int m) {
	std::cout << t << "," << m << std::endl;
	return 1;
}

int testvoid(void * a, void * b,void * c) {
	int tmp = ((int*)&a)[0];
	double tmp2 = ((double*)&b)[0];
	int tmp3 = ((int*)&c)[0];
	std::cout << tmp << "," << tmp2 << "," << tmp3 << std::endl;
}


typedef int (*TPVoid)(void *, void *, void *);

typedef std::function<int(void *, void *)> TwoParam;

class TestClass{
public:
	std::vector<void **> values;
	std::vector<uint64_t> paramCache;
	void * funcPtr;
	TestClass(void * func, std::vector<void**> v):
		values(v), funcPtr(func) {

	}


template<typename T0>
std::tuple<T0> GetParams()
{
	return std::make_tuple((T0)(values[0]));
}

template<typename T0,typename T1>
std::tuple<T0,T1> GetParams()
{
	return std::make_tuple((T0)(values[0]),(T1)(values[1]));
}

template<typename T0,typename T1,typename T2>
std::tuple<T0,T1,T2> GetParams()
{
	return std::make_tuple((T0)(values[0]),(T1)(values[1]),(T2)(values[2]));
}

template<typename T0,typename T1,typename T2,typename T3>
std::tuple<T0,T1,T2,T3> GetParams()
{
	return std::make_tuple((T0)(values[0]),(T1)(values[1]),(T2)(values[2]),(T3)(values[3]));
}

template<typename T0,typename T1,typename T2,typename T3,typename T4>
std::tuple<T0,T1,T2,T3,T4> GetParams()
{
	return std::make_tuple((T0)(values[0]),(T1)(values[1]),(T2)(values[2]),(T3)(values[3]),(T4)(values[4]));
}

template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5>
std::tuple<T0,T1,T2,T3,T4,T5> GetParams()
{
	return std::make_tuple((T0)(values[0]),(T1)(values[1]),(T2)(values[2]),(T3)(values[3]),(T4)(values[4]),(T5)(values[5]));
}

template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6>
std::tuple<T0,T1,T2,T3,T4,T5,T6> GetParams()
{
	return std::make_tuple((T0)(values[0]),(T1)(values[1]),(T2)(values[2]),(T3)(values[3]),(T4)(values[4]),(T5)(values[5]),(T6)(values[6]));
}

template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7>
std::tuple<T0,T1,T2,T3,T4,T5,T6,T7> GetParams()
{
	return std::make_tuple((T0)(values[0]),(T1)(values[1]),(T2)(values[2]),(T3)(values[3]),(T4)(values[4]),(T5)(values[5]),(T6)(values[6]),(T7)(values[7]));
}

template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8>
std::tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8> GetParams()
{
	return std::make_tuple((T0)(values[0]),(T1)(values[1]),(T2)(values[2]),(T3)(values[3]),(T4)(values[4]),(T5)(values[5]),(T6)(values[6]),(T7)(values[7]),(T8)(values[8]));
}

template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9>
std::tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9> GetParams()
{
	return std::make_tuple((T0)(values[0]),(T1)(values[1]),(T2)(values[2]),(T3)(values[3]),(T4)(values[4]),(T5)(values[5]),(T6)(values[6]),(T7)(values[7]),(T8)(values[8]),(T9)(values[9]));
}

template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10>
std::tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10> GetParams()
{
	return std::make_tuple((T0)(values[0]),(T1)(values[1]),(T2)(values[2]),(T3)(values[3]),(T4)(values[4]),(T5)(values[5]),(T6)(values[6]),(T7)(values[7]),(T8)(values[8]),(T9)(values[9]),(T10)(values[10]));
}

template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,typename T11>
std::tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11> GetParams()
{
	return std::make_tuple((T0)(values[0]),(T1)(values[1]),(T2)(values[2]),(T3)(values[3]),(T4)(values[4]),(T5)(values[5]),(T6)(values[6]),(T7)(values[7]),(T8)(values[8]),(T9)(values[9]),(T10)(values[10]),(T11)(values[11]));
}

template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,typename T12>
std::tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12> GetParams()
{
	return std::make_tuple((T0)(values[0]),(T1)(values[1]),(T2)(values[2]),(T3)(values[3]),(T4)(values[4]),(T5)(values[5]),(T6)(values[6]),(T7)(values[7]),(T8)(values[8]),(T9)(values[9]),(T10)(values[10]),(T11)(values[11]),(T12)(values[12]));
}

template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,typename T12,typename T13>
std::tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13> GetParams()
{
	return std::make_tuple((T0)(values[0]),(T1)(values[1]),(T2)(values[2]),(T3)(values[3]),(T4)(values[4]),(T5)(values[5]),(T6)(values[6]),(T7)(values[7]),(T8)(values[8]),(T9)(values[9]),(T10)(values[10]),(T11)(values[11]),(T12)(values[12]),(T13)(values[13]));
}

template<typename T0,typename T1,typename T2,typename T3,typename T4,typename T5,typename T6,typename T7,typename T8,typename T9,typename T10,typename T11,typename T12,typename T13,typename T14>
std::tuple<T0,T1,T2,T3,T4,T5,T6,T7,T8,T9,T10,T11,T12,T13,T14> GetParams()
{
	return std::make_tuple((T0)(values[0]),(T1)(values[1]),(T2)(values[2]),(T3)(values[3]),(T4)(values[4]),(T5)(values[5]),(T6)(values[6]),(T7)(values[7]),(T8)(values[8]),(T9)(values[9]),(T10)(values[10]),(T11)(values[11]),(T12)(values[12]),(T13)(values[13]),(T14)(values[14]));
}
	int call() {
		if (values.size() == 2) {
			//((TPVoid)funcPtr)(*values[0],*values[1]);
		}
		if (values.size() == 3) {
			((TPVoid)funcPtr)(*values[0],*values[1], *values[2]);
		}
	}
};


// typedef std::function<int(int, std::string, int)> Stringly;
// typedef std::function<int(double, int)> Doubly;

// template<typename Func, typename ... Types> 
// class Parameters {
// public:
// 	std::tuple<Types...> _args;
// 	std::function<Func> _function;
// 	Parameters(std::function<Func> f, Types ... a) :
// 		_function(f), _args(std::forward<Types>(a)...) {}

// 	std::tuple<Types...> GetValues() {
// 		return _args;
// 	}

// 	void SetValues(std::tuple<Types...> t) {
// 		_args = t;
// 	}

// 	int Call() {
// 		return std::apply(_function, _args);
// 	}

// };


// int Test(Parameters a) {
// 	a.call();
// }


int main() {
	int a = 99;
	double b = 23.231;
	int c = 55555;
	std::vector<void **> testvec = {(void**)&a,(void**)&b,(void**)&c};
	TestClass t((void*)&testvoid, testvec);
	t.call();
	auto ret = t.GetParams<int*, double*, int*>();
	std::cout << std::get<0>(ret)[0] << "," << std::get<1>(ret)[0] << "," << std::get<2>(ret)[0] << std::endl;
	std::get<0>(ret)[0] = 75;
	std::get<1>(ret)[0] = 19.99;
	std::get<2>(ret)[0] = 37;
	t.call();
	// Doubly a = std::bind(test2, std::placeholders::_1, std::placeholders::_2);
	// Stringly b = std::bind(test,  std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	// Parameters prams(a, 1.5, 9);

	//std::cout << std::get<0>(t) << "," << std::get<1>(t) << std::endl;
//	Test(prams);

}
