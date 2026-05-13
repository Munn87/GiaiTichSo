#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

// 1. Định nghĩa hàm f(x)
double f(double x) { 
    return tan(x / 4.0) - 1.0; 
}

// Đạo hàm bậc nhất f'(x) dùng trong công thức Newton và tính m1
double f_prime(double x) {
    double t = tan(x / 4.0);
    return 0.25 * (1.0 + t * t);
}

// Đạo hàm bậc hai f''(x) để kiểm tra điều kiện Fourier
double f_double_prime(double x) {
    double t = tan(x / 4.0);
    return 0.125 * t * (1.0 + t * t);
}

// Tìm m1 = min |f'(x)| trên [a, b] để đánh giá sai số
double get_m1(double a, double b) {
    double m1 = abs(f_prime(a));
    for (double x = a; x <= b; x += 0.01) {
        m1 = min(m1, abs(f_prime(x)));
    }
    return m1;
}

// Các hàm bổ trợ căn lề cho bảng
string center(double val, int width, int prec = 8) {
    ostringstream oss;
    oss << fixed << setprecision(prec) << val;
    string s = oss.str();
    int pad = (width - s.length()) / 2;
    if (pad < 0) pad = 0;
    return string(pad, ' ') + s + string(max(0, (int)(width - s.length() - pad)), ' ');
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

// 2. Hàm thực hiện phương pháp Tiếp tuyến (Newton)
void newtonMethod(double a, double b, double epsilon) {
    double x_old, x_new;
    int steps = 0;

    // Xác định m1 để tính sai số mục tiêu 
    double m1 = get_m1(a, b);

    // Chọn điểm xuất phát x0 là điểm Fourier [f(x0)*f''(x0) > 0]
    if (f(a) * f_double_prime(a) > 0) {
        x_old = a;
    } else {
        x_old = b;
    }

    cout << "\nDiem Fourier (x0): " << x_old << "\n";
    cout << "Gia tri m1 = min|f'(x)| tren [" << a << ", " << b << "] la: " << m1 << "\n\n";
    
    cout << "------+---------------+---------------+--------------- \n";
    cout << "|" << center("k", 5) << "|" << center("x_k", 15) << "|" << center("x_k+1", 15)
         << "|" << center("Sai so MT", 15) << "|\n";
    cout << "------+---------------+---------------+--------------- \n";

    

    while (true) {
        // Công thức phương pháp tiếp tuyến (Newton): x = x - f(x)/f'(x)
        double fx = f(x_old);
        double fpx = f_prime(x_old);
        
        x_new = x_old - fx / fpx;
        
        // Tính sai số mục tiêu theo tài liệu: |f(xn)| / m1 
        double target_error = abs(f(x_new)) / m1;

        cout << "|" << center(steps, 5)
             << "|" << center(x_old, 15)
             << "|" << center(x_new, 15)
             << "|" << center(target_error, 15, 10) << "|\n";

        // Kiểm tra điều kiện dừng
        if (target_error < epsilon) {
            break;
        }

        x_old = x_new;
        steps++;
        if (steps > 100) break; // Tránh vòng lặp vô tận
    }

    cout << "------+---------------+---------------+--------------- \n";
    cout << "Nghiem gan dung tim duoc la: " << setprecision(10) << x_new << "\n";
}

int main() {
    double a, b, epsilon;
    cout << "--- PHUONG PHAP TIEP TUYEN (NEWTON) ---\n";
    cout << "Nhap khoang cach ly nghiem [a, b]:\n";
    cout << "a = "; cin >> a;
    cout << "b = "; cin >> b;
    cout << "Nhap sai so epsilon: "; cin >> epsilon;

    // Kiểm tra điều kiện cách ly nghiệm
    if (f(a) * f(b) >= 0) {
        cout << "Loi: Ham so khong doi dau tren doan [" << a << ", " << b << "]\n";
    } else {
        newtonMethod(a, b, epsilon);
    }

    return 0;
}