#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

// 1. Định nghĩa hàm f(x)
double f(double x) { 
    return pow(x,5)-81.0*x-243.0  ; 
}

// Đạo hàm bậc nhất f'(x) để tính m1
double f_prime(double x) {
    return 5.0*pow(x,4) - 81.0;
}

// Đạo hàm bậc hai f''(x) để kiểm tra điều kiện Fourier
double f_double_prime(double x) {
    return 20.0*pow(x,3);
}

// Tìm m1 = min |f'(x)| trên [a, b] (xấp xỉ bằng cách chia nhỏ đoạn)
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

// 3. Hàm thực hiện phương pháp Dây cung
void chordMethod(double a, double b, double epsilon) {
    double x_old, x_new, d;
    int steps = 0;

    // Xác định m1 để tính sai số mục tiêu 
    double m1 = get_m1(a, b);

    // Kiểm tra điều kiện Fourier để chọn điểm cố định d [cite: 16, 17]
    // M(d, f(d)) là điểm Fourier nếu f(d)*f''(d) > 0
    if (f(a) * f_double_prime(a) > 0) {
        d = a;
        x_old = b;
    } else {
        d = b;
        x_old = a;
    }

    cout << "\nDiem Fourier (co dinh): d = " << d << "\n";
    cout << "Gia tri m1 = min|f'(x)| tren [" << a << ", " << b << "] la: " << m1 << "\n\n";
    
    cout << "------+---------------+---------------+--------------- \n";
    cout << "|" << center("k", 5) << "|" << center("x_k", 15) << "|" << center("x_k+1", 15)
         << "|" << center("Sai so MT", 15) << "|\n";
    cout << "------+---------------+---------------+--------------- \n";

    while (true) {
        // Công thức phương pháp dây cung 
        x_new = x_old - (f(x_old) * (x_old - d)) / (f(x_old) - f(d));
        
        // Tính sai số mục tiêu: |f(x_n)| / m1 
        double target_error = abs(f(x_new)) / m1;

        cout << "|" << center(steps, 5)
             << "|" << center(x_old, 15)
             << "|" << center(x_new, 15)
             << "|" << center(target_error, 15, 10) << "|\n";

        // Kiểm tra điều kiện dừng theo sai số mục tiêu
        if (target_error < epsilon) {
            break;
        }

        x_old = x_new;
        steps++;
        if (steps > 100) break;
    }

    cout << "------+---------------+---------------+--------------- \n";
    cout << "Nghiem gan dung tim duoc la: " << setprecision(10) << x_new << "\n";
}

int main() {
    double a, b, epsilon;
    cout << "--- PHUONG PHAP DAY CUNG ---\n";
    cout << "Nhap khoang cach ly nghiem [a, b]:\n";
    cout << "a = "; cin >> a;
    cout << "b = "; cin >> b;
    cout << "Nhap sai so epsilon: "; cin >> epsilon;

    // Kiểm tra điều kiện cách ly nghiệm [cite: 53]
    if (f(a) * f(b) >= 0) {
        cout << "Loi: Ham so khong doi dau tren doan [" << a << ", " << b << "]\n";
    } else {
        chordMethod(a, b, epsilon);
    }

    return 0;
}