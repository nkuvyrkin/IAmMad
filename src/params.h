#pragma once

#include "diff.h"

namespace Params
{
	template<char Family, int Index, typename Vec>
	void AddValue (Vec& vec, double value)
	{
		vec [BuildIndex<Family, Index> ()] = value;
	}

	template<typename Tree>
	struct MaxSize;

	template<char Family, int Index>
	struct MaxSize<Node<Variable<Family, Index>>>
	{
		static constexpr size_t Value = BuildIndex<Family, Index> ();
	};

	template<UnaryFunction UF, typename Child>
	struct MaxSize<Node<UnaryFunctionWrapper<UF>, Child>>
	{
		static constexpr size_t Value = MaxSize<Child>::Value;
	};

	template<BinaryFunction BF, typename Left, typename Right>
	struct MaxSize<Node<BinaryFunctionWrapper<BF>, Left, Right>>
	{
		static constexpr size_t Value = MaxSize<Left>::Value > MaxSize<Right>::Value ? MaxSize<Left>::Value : MaxSize<Right>::Value;
	};

	template<typename... Args>
	struct MaxSize<Args...>
	{
		static constexpr size_t Value = 0;
	};

	namespace detail
	{
		template<typename Vec>
		void BuildValuesImpl (Vec&)
		{
		}

		template<char Family, int Index, typename Vec, typename... Tail>
		void BuildValuesImpl (Vec& vec, Node<Variable<Family, Index>>, double value, Tail&&... tail)
		{
			AddValue<Family, Index> (vec, value);
			BuildValuesImpl (vec, std::forward<Tail> (tail)...);
		}
	}

	template<typename FT, typename... Tail>
	auto BuildValues (Tail&&... tail)
	{
		std::array<double, MaxSize<FT>::Value + 1> vec {{ 0 }};
		detail::BuildValuesImpl (vec, std::forward<Tail> (tail)...);
		return vec;
	}
}