#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

// 1. Định nghĩa hàm f(x)
double f(double x) { 
    return pow(2,x) - 5*x + sin(x); 
}

// Đạo hàm bậc nhất f'(x)
double f_prime(double x) {
    return log(2)*pow(2,x) - 5 + cos(x);
}

// Đạo hàm bậc hai f''(x)
double f_double_prime(double x) {
    return log(2)*log(2)*pow(2,x) - sin(x);
}

// Tìm m1 = min |f'(x)| trên [a, b] 
double get_m1(double a, double b) {
    double m1 = abs(f_prime(a));
    for (double x = a; x <= b; x += 0.001) {
        m1 = min(m1, abs(f_prime(x)));
    }
    return m1;
}

// Tìm M1 = max |f'(x)| trên [a, b] 
double get_M1(double a, double b) {
    double M1 = abs(f_prime(a));
    for (double x = a; x <= b; x += 0.001) {
        M1 = max(M1, abs(f_prime(x)));
    }
    return M1;
}

// Các hàm bổ trợ căn lề
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

// 3. Hàm thực hiện phương pháp Dây cung dùng sai số 2 xấp xỉ
void chordMethod(double a, double b, double epsilon) {
    double x_old, x_new, d;
    int steps = 1;

    // Tính m1 và M1 để phục vụ công thức sai số 2 xấp xỉ 
    double m1 = get_m1(a, b);
    double M1 = get_M1(a, b);
    double error_factor = (M1 - m1) / m1;

    // Kiểm tra điều kiện Fourier để chọn điểm cố định d [cite: 72]
    if (f(a) * f_double_prime(a) > 0) {
        d = a;
        x_old = b;
    } else {
        d = b;
        x_old = a;
    }

    cout << "\nDiem Fourier (co dinh): d = " << d << "\n";
    cout << "He so sai so (M1-m1)/m1 = " << error_factor << "\n\n";
    
    cout << "------+---------------+---------------+--------------- \n";
    cout << "|" << center("k", 5) << "|" << center("x_k", 15) << "|" << center("x_k+1", 15)
         << "|" << center("Sai so 2XX", 15) << "|\n";
    cout << "------+---------------+---------------+--------------- \n";

    while (true) {
        // Công thức phương pháp dây cung [cite: 35]
        x_new = x_old - (f(x_old) * (x_old - d)) / (f(x_old) - f(d));
        
        // Công thức sai số 2 xấp xỉ liên tiếp 
        double current_error = error_factor * abs(x_new - x_old);

        cout << "|" << center(steps, 5)
             << "|" << center(x_old, 15)
             << "|" << center(x_new, 15)
             << "|" << center(current_error, 15, 10) << "|\n";

        // Kiểm tra điều kiện dừng
        if (current_error < epsilon) {
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
    cout << "--- PHUONG PHAP DAY CUNG (SAI SO 2 XAP XI) ---\n";
    cout << "Nhap khoang cach ly nghiem [a, b]:\n";
    cout << "a = "; cin >> a;
    cout << "b = "; cin >> b;
    cout << "Nhap sai so epsilon: "; cin >> epsilon;

    if (f(a) * f(b) >= 0) {
        cout << "Loi: Ham so khong doi dau tren doan [" << a << ", " << b << "]\n";
    } else {
        chordMethod(a, b, epsilon);
    }

    return 0;
}