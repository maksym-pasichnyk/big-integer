#pragma once

#include <iostream>
#include <string>

template <int N> 
struct bigint {
public:
	uint8_t data[N] {0};

	bigint() = default;

	bigint(uint64_t number) {
		constexpr int size = sizeof(uint64_t) <= N ? sizeof(uint64_t) : N;

		uint8_t *bytes = (uint8_t*)&number;
		for (int i = 0; i < size; i++) {
			data[i] = bytes[i];
		}
	}

	bigint(const std::string &input) {
		std::string number = input;
		int index = 0;
		do {
			if (index == N) break;
			
			std::string res;
			number = fast_str_div(number, "16", &res);
			int byte1 = stoi(res, 0, 10);
			number = fast_str_div(number, "16", &res);
			int byte2 = stoi(res, 0, 10);

			data[index++] = (byte2 << 4) | byte1;
		} while (number != "0");
	}

	bigint<N> operator++(int) {
		auto result = *this;
		operator++();
		return result;
	}

	bigint<N> operator--(int) {
		auto result = *this;
		operator--();
		return result;
	}

	bigint<N> &operator++() {
		for (int i = 0; i < N; i++) {
			if (data[i]++ != 0xFF) break;
		}
		return *this;
	}

	bigint<N> &operator--() {
		for (int i = 0; i < N; i++) {
			if (data[i]-- != 0x00) break;
		}
		return *this; 
	}

	bigint<N> operator~() {
		bigint<N> result;
		for (int i = 0; i < N; i++) {
			result.data[i] = 0xFF - data[i];
		}
		return result;
	}

	// bool operator!() {
		
	// }

	bigint<N> operator+() {
		return *this;
	}

	bigint<N> operator-() {
		return ++(~*this);
	}

	bigint<N> operator*(const bigint<N> &other) const {
		// bigint<N> out;

		// register uint8_t carry = 0;
		// for (int i = 0; i < N; i++) {
		//  for (int j = 0; (i + j) < N; j++) {
		//	  register int k = i + j;

		//	  register int16_t i16 = (int16_t) data[j] * (int16_t) other.data[i];

		//	  register int16_t val = out.data[k] + (i16 & 0xFF) + carry;
		//	  out.data[k] = val;

		//	  carry = (i16 >> 8) + (val >> 8);
		//  }
		// }

		// return out;
		return bigint<N>(str_mul(str(), other.str()));
	}

	bigint<N> operator/(const bigint<N> &other) const {
		return bigint<N>(fast_str_div(str(), other.str()));
	}

	bigint<N> operator%(const bigint<N> &other) const {
		std::string remainder;
		fast_str_div(str(), other.str(), &remainder);
		return bigint<N>(remainder);
	}

	bigint<N> operator+(const bigint<N> &other) const {
		bigint<N> result;
		uint8_t carry = 0;
		for (int i = 0; i < N; i++) {
			short sbyte = data[i] + other.data[i] + carry;
			result.data[i] = sbyte;
			carry = sbyte >> 8;
		}
		return result;
	}

	bigint<N> operator-(const bigint<N> &other) const {
		bigint<N> result;
		uint8_t carry = 0;
		for (int i = 0; i < N; i++) {
			short sbyte = data[i] - other.data[i] - carry;
			result.data[i] = sbyte;
			carry = (0xFF - sbyte) >> 8;
		}
		return result;
	}

	// auto operator<<(const bigint<N> &other) const {
		
	// }

	// auto operator>>(const bigint<N> &other) const {
		
	// }

	bool operator<(const bigint<N> &other) const {
		for (int i = N - 1; i >= 0; i++) {
			if (data[i] > other.data[i]) return false;
		}
		return operator!=(other);
	}

	bool operator>(const bigint<N> &other) const {
		for (int i = N - 1; i >= 0; i++) {
			if (data[i] < other.data[i]) return false;
		}
		return true;
	}
	
	bool operator<=(const bigint<N> &other) const {
		return operator>(other) == false;
	}

	bool operator>=(const bigint<N> &other) const {
		return operator<(other) == false;
	}   

	bool operator==(const bigint<N> &other) const {
		for (int i = 0; i < N; i++) {
			if (data[i] != other.data[i]) return false;
		}
		return true;
	}

	bool operator!=(const bigint<N> &other) const {
		return !operator==(other);
	}

	bigint<N> operator&(const bigint<N> &other) const {
		bigint<N> result;
		for (int i = 0; i < N; i++) {
			result.data[i] = data[i] & other.data[i];
		}
		return result;
	}

	bigint<N> operator^(const bigint<N> &other) const {
		bigint<N> result;
		for (int i = 0; i < N; i++) {
			result.data[i] = data[i] ^ other.data[i];
		}
		return result;
	}

	bigint<N> operator|(const bigint<N> &other) const {
		bigint<N> result;
		for (int i = 0; i < N; i++) {
			result.data[i] = data[i] | other.data[i];
		}
		return result;
	}

	// bool operator&&(const bigint<N> &other) {
		
	// }

	// bool operator||(const bigint<N> &other) {
		
	// }

	bigint<N> &operator=(const bigint<N> &other) {
		for (int i = 0; i < N; i++) {
			data[i] = other.data[i];
		}
		return *this;
	}

	bigint<N> &operator*=(const bigint<N> &other) {
		return operator=(*this * other);
	}

	bigint<N> &operator/=(const bigint<N> &other) {
		return operator=(*this / other);
	}

	bigint<N> &operator%=(const bigint<N> &other) {
		return operator=(*this % other);
	}

	bigint<N> &operator+=(const bigint<N> &other) {
		return operator=(*this + other);
	}

	bigint<N> &operator-=(const bigint<N> &other) {
		return operator=(*this - other);
	}

	bigint<N> &operator>>=(const bigint<N> &other) {
		return operator=(*this >> other);
	}

	bigint<N> &operator<<=(const bigint<N> &other) {
		return operator=(*this << other);
	}

	bigint<N> &operator&=(const bigint<N> &other) {
		return operator=(*this & other);
	}

	bigint<N> &operator^=(const bigint<N> &other) {
		return operator=(*this ^ other);
	}

	bigint<N> &operator|=(const bigint<N> &other) {
		return operator=(*this | other);
	}

	std::string str() const {
		std::string result = "0";
		for (int i = N - 1; i >= 0; i--) {
			result = str_add(str_mul(result, "256"), std::to_string(data[i]));
		}
		return result;
	}  

	friend std::ostream &operator<<(std::ostream &out, const bigint<N> &other) {
		return out << other.str();
	}

private:
	static std::string &str_inc(std::string &n) {
		int carry = 1;

		int size = n.size();
		for (int i = size - 1; i >= 0; i--) {
			int v = n[i] - '0' + carry;
			n[i] = v % 10 + '0';

			if ((carry = v / 10) == 0) break;
		}
		if (carry == 1) n.insert(n.begin(), '1');
		return n;
	}

	static std::string str_add(const std::string &n1, const std::string &n2) {
		int carry = 0;

		int size1 = n1.size();
		int size2 = n2.size();

		int size = size1 >= size2 ? size1 : size2;
		std::string result(size, '1');

		for (int i = 0; i < size; i++) {
			int v = (i < size1 ? (n1[size1 - i - 1] - '0') : 0) + (i < size2 ? (n2[size2 - i - 1] - '0') : 0) + carry;
			result[size - i - 1] = v % 10 + '0';
			carry = v / 10;
		}
		if (carry == 1) result.insert(result.begin(), '1');
		result.erase(0, result.find_first_not_of('0'));
		return result.empty() ? "0" : result;
	}

	static std::string str_div(const std::string &n1, const std::string &n2, std::string &remainder) {
		std::string n3 = n1;
		std::string result = "0";
		int carry = 0;

		int size1 = n1.size();
		int size2 = n2.size();
		std::string n4;

		while (true) {
			int size = size1 >= size2 ? size1 : size2;

			n4 = n3;

			for (int i = 0; i < size; i++) {
				int v = 10 + (i < size1 ? (n3[size1 - i - 1] - '0') : 0) - (i < size2 ? (n2[size2 - i - 1] - '0') : 0) - carry;
				if (i < size1) n3[size1 - i - 1] = v % 10 + '0';
				carry = 1 - v / 10;
			}
			if (carry == 1) break;
			str_inc(result);
		}
		n4.erase(0, n4.find_first_not_of('0'));
		remainder = n4.empty() ? "0" : n4;
		return result;
	}

	static std::string fast_str_div(const std::string &input1, const std::string &input2, std::string *rest = 0) {
		std::string result;

		std::string remainder;
		std::string number1 = input1;

		std::string part = "";
		while ((number1 != "0") && (part.size() < number1.size())) {
			part += number1[part.size()];
			result.append(str_div(part, input2, remainder));
			number1 = remainder + number1.erase(0, part.size());
			part = remainder;
		}
		if (rest) *rest = number1;

		result.erase(0, result.find_first_not_of('0'));
		return result.empty() ? "0" : result;
	}

	static std::string str_mul(const std::string &n1, const std::string &n2) {
		std::string res;

		int size1 = n1.size();
		int size2 = n2.size();

		for (int i = 0; i < size1; i++) {
			std::string temp(size2, 0);

			int v1 = n1[size1 - i - 1] - '0';
			int carry = 0;

			for (int j = size2 - 1; j >= 0; j--) {
				int v = v1 * (n2[j] - '0') + carry;

				temp[j] = (char)((v % 10) + '0');
				carry = v / 10;
			}
			temp = (char) (carry + '0') + temp + std::string(i, '0');
			res = str_add(res, temp);
		}
		res.erase(0, res.find_first_not_of('0'));
		return res.empty() ? "0" : res;
	}
};
