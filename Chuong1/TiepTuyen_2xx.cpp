// Phương pháp tiếp tuyến (Newton - Raphson) 
// theo công thức sai số 2 lần xấp xỉ liên tiếp
// | x_k - x* | <= ( M2 / 2.m1 ) * | x_k - x_k-1 |^2 

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

// Đạo hàm bậc nhất f'(x)
double f_prime(double x) {
    double t = tan(x / 4.0);
    return 0.25 * (1.0 + t * t);
}

// Đạo hàm bậc hai f''(x)
double f_double_prime(double x) {
    double t = tan(x / 4.0);
    return 0.125 * t * (1.0 + t * t);
}

// Tìm m1 = min |f'(x)| trên [a, b] 
double get_m1(double a, double b) {
    double m1 = abs(f_prime(a));
    for (double x = min(a, b); x <= max(a, b); x += 0.001) {
        m1 = min(m1, abs(f_prime(x)));
    }
    return m1;
}

// Tìm M2 = max |f''(x)| trên [a, b] 
double get_M2(double a, double b) {
    double M2 = abs(f_double_prime(a));
    for (double x = min(a, b); x <= max(a, b); x += 0.001) {
        M2 = max(M2, abs(f_double_prime(x)));
    }
    return M2;
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

// 2. Hàm thực hiện phương pháp Tiếp tuyến (Newton) dùng sai số 2 xấp xỉ
void newtonMethod(double a, double b, double epsilon) {
    double x_old, x_new;
    int steps = 1;

    // Tính m1 và M2 để đánh giá sai số [cite: 56, 74]
    double m1 = get_m1(a, b);
    double M2 = get_M2(a, b);
    double error_factor = M2 / (2 * m1);

    // Chọn điểm xuất phát x0 thỏa mãn điều kiện Fourier f(x0)*f''(x) > 0 [cite: 72]
    if (f(a) * f_double_prime(a) > 0) {
        x_old = a;
    } else {
        x_old = b;
    }

    cout << "\nDiem Fourier (x0): " << x_old << "\n";
    cout << "He so sai so M2/(2*m1) = " << error_factor << "\n\n";
    
    cout << "------+---------------+---------------+--------------- \n";
    cout << "|" << center("k", 5) << "|" << center("x_k", 15) << "|" << center("x_k+1", 15)
         << "|" << center("Sai so 2XX", 15) << "|\n";
    cout << "------+---------------+---------------+--------------- \n";

    while (true) {
        // Công thức lặp Newton: x = x - f(x)/f'(x) [cite: 66]
        x_new = x_old - f(x_old) / f_prime(x_old);
        
        // Công thức sai số 2 xấp xỉ liên tiếp: (M2 / 2m1) * |x_new - x_old|^2 
        double current_error = error_factor * pow(abs(x_new - x_old), 2);

        cout << "|" << center(steps, 5)
             << "|" << center(x_old, 15)
             << "|" << center(x_new, 15)
             << "|" << center(current_error, 15, 10) << "|\n";

        // Kiểm tra điều kiện dừng 
        if (current_error <= epsilon) {
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
    cout << "--- PHUONG PHAP TIEP TUYEN (SAI SO 2 XAP XI) ---\n";
    cout << "Nhap khoang cach ly nghiem [a, b]:\n";
    cout << "a = "; cin >> a;
    cout << "b = "; cin >> b;
    cout << "Nhap sai so epsilon: "; cin >> epsilon;

    if (f(a) * f(b) >= 0) {
        cout << "Loi: Ham so khong doi dau tren doan [" << a << ", " << b << "]\n";
    } else {
        newtonMethod(a, b, epsilon);
    }

    return 0;
}