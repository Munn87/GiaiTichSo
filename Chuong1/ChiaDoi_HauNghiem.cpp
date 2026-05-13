#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

double f(double x) { return(pow(x,5)-17); }

string center(double val, int width) {
  ostringstream oss;
  oss << fixed << setprecision(8) << val;
  string s = oss.str();
  int pad = (width - s.length()) / 2;
  return string(pad, ' ') + s + string(width - s.length() - pad, ' ');
}

string center(int val, int width) {
  ostringstream oss;
  oss << val;
  string s = oss.str();
  int pad = (width - s.length()) / 2;
  return string(pad, ' ') + s + string(width - s.length() - pad, ' ');
}

string center(const string &s, int width) {
  int pad = (width - s.length()) / 2;
  return string(pad, ' ') + s + string(width - s.length() - pad, ' ');
}

void bisection(double a, double b, double epsilon) {
  int steps = 1;
  double c;
  // clang-format off
  cout << "------+---------------+---------------+---------------+----------------\n";
  cout << "|" << center("k", 5) << "|" << center("a_k", 15) << "|" << center("b_k", 15)
       << "|" << center("x_k+1", 15) << "|" << center("Dau f(x_k+1)", 15) << "|\n";
  cout << "------+---------------+---------------+---------------+----------------\n";
  while (b - a >= epsilon)
  {
    c = (a + b) / 2;
    cout << "|" << center(steps, 5)
         << "|" << center(a, 15)
         << "|" << center(b, 15)
         << "|" << center(c, 15)
         << "|" << center((f(c) > 0 ? "+" : "-"), 15) << "|\n";
    if (f(c) == 0)
      break;
    else if (f(a) * f(c) < 0)
      b = c;
    else
      a = c;
    ++steps;
  }
  cout << "------+---------------+---------------+---------------+----------------\n";
  cout << "Nghiem gan dung la: " << setprecision(10) << c << "\n";
  // clang-format on
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  double a, b, c, epsilon;
  cin >> a >> b >> epsilon;
  bisection(a, b, epsilon);
}