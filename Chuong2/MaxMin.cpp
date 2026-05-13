#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

// Định nghĩa hàm số f(x) = x^3 - 3x + 1
double f(double x) {
    return pow(x, 3) - 3 * x + 1;
}

// Đạo hàm f'(x) = 3x^2 - 3
double f_prime(double x) {
    return 3 * pow(x, 2) - 3;
}

int main() {
    double a, b;
    cout << "Nhap khoang [a, b]: ";
    cin >> a >> b;

    vector<double> values;
    values.push_back(f(a));
    values.push_back(f(b));

    // Tìm điểm dừng (f'(x) = 0 => 3x^2 - 3 = 0 => x = 1 hoặc x = -1)
    double points[] = {-1.0, 1.0};
    for (double x_i : points) {
        if (x_i > a && x_i < b) {
            values.push_back(f(x_i));
        }
    }

    // Tìm M và m của f(x)
    double M = *max_element(values.begin(), values.end());
    double m = *min_element(values.begin(), values.end());

    // Tính cực trị của |f(x)| theo logic trong ảnh
    double M_abs = max(abs(M), abs(m));
    double m_abs = (M * m <= 0) ? 0 : min(abs(M), abs(m));

    cout << fixed << setprecision(6);
    cout << "Max f(x) = " << M << ", Min f(x) = " << m << endl;
    cout << "Max |f(x)| = " << M_abs << ", Min |f(x)| = " << m_abs << endl;

    return 0;
}