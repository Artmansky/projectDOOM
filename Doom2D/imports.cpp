#include "imports.hpp"


bool generateRandomBool() {
	static std::default_random_engine re{ std::random_device{}() };
	using Dist = std::uniform_int_distribution<int>;
	static Dist uid{};
	return uid(re, Dist::param_type{0, 1});
}

float generateRandomFloat(const float& min, const float& max) {
	static std::default_random_engine re{ std::random_device{}() };
	using Dist = std::uniform_real_distribution<float>;
	static Dist uid{};
	return uid(re, Dist::param_type{min, max});
}