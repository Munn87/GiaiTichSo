#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// 1. Định nghĩa hàm f(x)
double f(double x) { 
    return pow(x, 5) - 3 * pow(x, 3) + 2 * x * x - x + 5; 
}

// 2. Định nghĩa đạo hàm bậc hai f''(x) để kiểm tra điều kiện Fourier
double f_double_prime(double x) {
    return 20 * pow(x, 3) - 18 * x + 4;
}

// Các hàm bổ trợ căn lề cho bảng (giữ nguyên phong cách của bạn)
string center(double val, int width) {
    ostringstream oss;
    oss << fixed << setprecision(8) << val;
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
    int steps = 1;

    // Kiểm tra điều kiện Fourier để chọn điểm cố định d
    // Nếu f(a) * f''(a) > 0 thì d = a, x0 = b
    if (f(a) * f_double_prime(a) > 0) {
        d = a;
        x_old = b;
    } else {
        d = b;
        x_old = a;
    }

    cout << "\nDiem co dinh chon la d = " << d << "\n";
    cout << "------+---------------+---------------+--------------- \n";
    cout << "|" << center("k", 5) << "|" << center("x_k", 15) << "|" << center("x_k+1", 15)
         << "|" << center("f(x_k+1)", 15) << "|\n";
    cout << "------+---------------+---------------+--------------- \n";

    while (true) {
        // Công thức phương pháp dây cung
        x_new = x_old - f(x_old) * (x_old - d) / (f(x_old) - f(d));

        cout << "|" << center(steps, 5)
             << "|" << center(x_old, 15)
             << "|" << center(x_new, 15)
             << "|" << center(f(x_new), 15) << "|\n";

        // Kiểm tra sai số để dừng vòng lặp
        if (abs(x_new - x_old) < epsilon) {
            break;
        }

        x_old = x_new;
        steps++;
        
        // Tránh vòng lặp vô hạn nếu không hội tụ
        if (steps > 100) break;
    }

    cout << "------+---------------+---------------+--------------- \n";
    cout << "Nghiem gan dung la: " << setprecision(10) << x_new << "\n";
}

int main() {
    // Tối ưu nhập xuất
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    double a, b, epsilon;
    cout << "Nhap khoang cach ly nghiem [a, b]:\n";
    cout << "a = "; cin >> a;
    cout << "b = "; cin >> b;
    cout << "Nhap sai so epsilon: "; cin >> epsilon;

    if (f(a) * f(b) >= 0) {
        cout << "Loi: Ham so khong doi dau tren doan da cho!\n";
    } else {
        chordMethod(a, b, epsilon);
    }

    return 0;
}