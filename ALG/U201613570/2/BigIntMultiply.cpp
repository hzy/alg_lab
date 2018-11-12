#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

#define BASE 100000000
#define BASE_LOG_10 8
#define IntList vector<int>

class BigInt {
 private:
  IntList digits;
  bool sign;

 public:
  BigInt(string);
  BigInt(IntList, bool);
  ~BigInt();
  BigInt operator-() const;
  BigInt operator-(BigInt const&) const;
  BigInt operator+(BigInt const&) const;
  bool operator>(BigInt const&) const;
  bool operator<(BigInt const&) const;
  bool operator==(BigInt const&) const;
  friend std::istream& operator>>(std::istream&, BigInt&);        // Input&Output
  friend std::ostream& operator<<(std::ostream&, BigInt const&);  // Input&Output
};

BigInt::~BigInt() {}
BigInt::BigInt(string s) {
  sign = s.substr(0, 1) != "-";  // true for positive
  if (!sign) {
    s = s.substr(1);
  }
  size_t l = s.length();
  size_t i = l % BASE_LOG_10;
  digits = {stoi(s.substr(0, i))};
  for (; i < l; i += BASE_LOG_10) {
    digits.insert(digits.begin(), stoi(s.substr(i, BASE_LOG_10)));
  }

  size_t ll = digits.size();
}

BigInt::BigInt(IntList digits, bool sign)
    : digits(digits), sign(sign) {}

BigInt BigInt::operator-() const {
  return BigInt(this->digits, !this->sign);
}

bool BigInt::operator>(BigInt const& b) const {
  BigInt a = *this;
  size_t a_l = a.digits.size();
  size_t b_l = b.digits.size();
  if (a_l > b_l) {
    return true;
  } else if (a_l < b_l) {
    return false;
  }
  for (size_t i = a_l - 1; i >= 0; i++) {
    if (a.digits[i] > b.digits[i]) {
      return true;
    } else if (a.digits[i] < b.digits[i]) {
      return false;
    }
  }

  return false;
}

bool BigInt::operator==(BigInt const& b) const {
  BigInt a = *this;
  size_t a_l = a.digits.size();
  size_t b_l = b.digits.size();
  if (a_l == b_l) {
    for (size_t i = 0; i < a_l; i++) {
      if (a.digits[i] != b.digits[i]) {
        return false;
      }
    }
    return true;
  }
  return false;
}

bool BigInt::operator<(BigInt const& b) const {
  BigInt a = *this;
  if (a > b) {
    return false;
  }
  if (a == b) {
    return false;
  }

  return true;
}

BigInt BigInt::operator+(BigInt const& b) const {
  BigInt a = *this;
  if (a.sign != b.sign) {
    return a - (-b);
  }

  BigInt c({}, a.sign);
  int carry = 0;
  for (int i = 0; i < max(a.digits.size(), b.digits.size()); i++) {
    int a_d = i < a.digits.size() ? a.digits[i] : 0;
    int b_d = i < b.digits.size() ? b.digits[i] : 0;
    c.digits.push_back(a_d + b_d + carry);
    if (c.digits[i] >= BASE) {
      carry = 1;
      c.digits[i] -= BASE;
    } else {
      carry = 0;
    }
  }

  if (carry) {
    c.digits.push_back(carry);
  }

  return c;
}

BigInt BigInt::operator-(BigInt const& b) const {
  BigInt a = *this;
  if (a.sign != b.sign) {
    return a + (-b);
  }

  if (a < b) {
    return -(b - a);
  }

  BigInt c({}, a.sign);
  int borrow = 0;
  for (int i = 0; i < max(a.digits.size(), b.digits.size()); i++) {
    int a_d = i < a.digits.size() ? a.digits[i] : 0;
    int b_d = i < b.digits.size() ? b.digits[i] : 0;
    c.digits.push_back(a_d - b_d - borrow);
    if (c.digits[i] < 0) {
      borrow = 1;
      c.digits[i] += BASE;
    } else {
      borrow = 0;
    }
  }

  return c;
};

std::ostream& operator<<(std::ostream& out, BigInt const& a) {
  if (!a.sign) {
    out << "-";
  }
  for (int i = a.digits.size() - 1; i >= 0; i--) {
    string s = to_string(a.digits[i]);
    if (s.length() == BASE_LOG_10 || i == a.digits.size() - 1) {
      out << s;
    } else {
      for (int i = 0; i < BASE_LOG_10 - s.length(); i++) {
        out << "0";
      }
      out << s;
    }
  }

  return out;
}

int main() {
  ifstream infile("in.dat");
  ofstream outfile("out.dat");
  int n;
  infile >> n;
  for (int i = 0; i < n; i++) {
    int flag;
    string x, y;
    infile >> x >> y >> flag;
    BigInt a(x);
    BigInt b(y);
    if (flag == 1) {
      BigInt c(a + b);
      outfile << c << endl;
    } else if (flag == 2) {
      BigInt c(a - b);
      outfile << c << endl;
    }
    // else if (flag == 3)
    // outfile << a * b << endl;
  }
  infile.close();
  outfile.close();
  return 0;
}