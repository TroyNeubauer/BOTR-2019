#pragma once

#include <stdint.h>
#include <limits.h>
#undef min
#undef max

template<typename T>
struct is_signed { enum { value = false }; };

template<>
struct is_signed<bool> { enum { value = false }; };
template<>
struct is_signed<uint8_t> { enum {  value = false }; };
template<>
struct is_signed<int8_t> { enum {  value = true }; };
template<>
struct is_signed<uint16_t> { enum { value = false }; };
template<>
struct is_signed<int16_t> { enum { value = true }; };
template<>
struct is_signed<uint32_t> { enum { value = false }; };
template<>
struct is_signed<int32_t> { enum { value = true }; };
template<>
struct is_signed<uint64_t> { enum { value = false }; };
template<>
struct is_signed<int64_t> { enum { value = true }; };
template<>
struct is_signed<float> { enum { value = true }; };
template<>
struct is_signed<double> { enum { value = true }; };


template<class T>
float max_value(T&) {
	return powf(2.0f, sizeof(T) * CHAR_BIT - (is_signed<T>::value ? 1 : 0)) - 1.0f;
}

//Maps a value from the range [0, newMax] to [0, (the max value for its type)]
template<typename T>
T SimpleMapEncode(float value, T newMax) {
	if (value > newMax) return max_value(newMax);

	if (is_signed<T>::value && value < -newMax) return -max_value(newMax);
	else if (!is_signed<T>::value && value < 0.0f) return 0;
	return static_cast<T>(value * max_value(newMax) / static_cast<float>(newMax));
}

template<typename T>
float SimpleMapDecode(T value, T newMax) {
	return static_cast<float>(value) * static_cast<float>(newMax) / max_value(newMax);
}

template<typename T, size_t MAX_VALUE>
class MyFloat {
private:
	T value;

public:
	MyFloat() { }
	MyFloat(MyFloat& copy) { value = copy.value; }
	MyFloat(T value) { this->value = value; }
	MyFloat(float value) { this->value = SimpleMapEncode(value, static_cast<T>(MAX_VALUE)); }

	T max() const { return static_cast<T>(MAX_VALUE); }
	T min() const { return (is_signed<T>::value ? -static_cast<T>(MAX_VALUE) : 0); }

	operator float() const { return SimpleMapDecode(value, static_cast<T>(MAX_VALUE)); }
	T SetInternalValue(T value);
};

/* Test code
constexpr int16_t max = 100;
	double averageError = 0.0;
	int total = 0;
	for (float i = -max - 0.5f; i <= max + 0.5f; i += 0.235f) {
		total++;
		MyFloat<int16_t, max> value(i);
		float result = static_cast<float>(value);
		double error = i - result;
		averageError += abs(error);
		GS_INFO("{} {} Error: {}", i, result, error);
	}
	GS_INFO("Average Error: {}", averageError / total);
	system("PAUSE");
	return 0;

*/