#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

// 1. Định nghĩa hàm số f(x)
double f(double x) {
    return pow(x, 3) - 3 * x + 1; // Ví dụ: f(x) = x^3 - 3x + 1
}

// 2. Định nghĩa đạo hàm f'(x)
double f_prime(double x) {
    return 3 * pow(x, 2) - 3; // f'(x) = 3x^2 - 3
}

// Hàm chia đôi để tìm nghiệm của f'(x) = 0 trong khoảng [low, high]
double findRoot(double low, double high, double eps = 1e-7) {
    double a = low, b = high, c;
    while ((b - a) > eps) {
        c = (a + b) / 2.0;
        if (f_prime(a) * f_prime(c) < 0) b = c;
        else a = c;
    }
    return (a + b) / 2.0;
}

int main() {
    double a, b, eps = 1e-6;
    cout << "Nhap khoang [a, b]: ";
    cin >> a >> b;

    // Bước 1 & 2: Tìm các điểm dừng x_i trong (a, b)
    vector<double> critical_points;
    double step = 0.1; 
    for (double i = a; i < b; i += step) {
        double next = min(i + step, b);
        if (f_prime(i) * f_prime(next) <= 0) {
            critical_points.push_back(findRoot(i, next));
        }
    }

    // Bước 3: Tính giá trị tại 2 đầu mút và các điểm dừng
    vector<double> values;
    values.push_back(f(a)); // f(a)
    values.push_back(f(b)); // f(b)
    
    for (double x_i : critical_points) {
        values.push_back(f(x_i)); // f(x_i)
    }

    // Bước 4: So sánh tìm M và m của f(x)
    double M = *max_element(values.begin(), values.end());
    double m = *min_element(values.begin(), values.end());

    // Tìm cực trị của |f(x)|
    // Giá trị lớn nhất M_abs
    double M_abs = max(abs(M), abs(m));

    // Giá trị nhỏ nhất m_abs dựa trên tích M.m
    double m_abs;
    if (M * m <= 0) {
        m_abs = 0; // Đồ thị cắt trục hoành
    } else {
        m_abs = min(abs(M), abs(m));
    }

    // Xuất kết quả
    cout << fixed << setprecision(6);
    cout << "-----------------------------------" << endl;
    cout << "Cuc tri cua f(x):" << endl;
    cout << "Max M = " << M << endl;
    cout << "Min m = " << m << endl;
    cout << "-----------------------------------" << endl;
    cout << "Cuc tri cua |f(x)|:" << endl;
    cout << "Max |f(x)| = " << M_abs << endl;
    cout << "Min |f(x)| = " << m_abs << endl;

    return 0;
}