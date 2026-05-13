#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// 1. Định nghĩa hàm f(x)
double f(double x) { return (pow(x, 5) - 17); }

// Các hàm bổ trợ căn lề 
string center(double val, int width) {
    ostringstream oss;
    oss << fixed << setprecision(10) << val;
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

// 2. Thuật toán chia đôi dùng sai số TIÊN NGHIỆM
void bisection_priori(double a, double b, double epsilon) {
    // Bước tính n dựa trên công thức sai số tiên nghiệm 
    int n = ceil(log2((b - a) / epsilon));
    
    cout << "Khoang cach li nghiem: [" << a << ", " << b << "]\n";
    cout << "Sai so yeu cau epsilon = " << epsilon << "\n";
    cout << "So buoc lap can thiet tinh truoc: n = " << n << "\n\n";

    double c;
    cout << "------+---------------+---------------+---------------+----------------\n";
    cout << "|" << center("k", 5) << "|" << center("a_k", 15) << "|" << center("b_k", 15)
         << "|" << center("x_k", 15) << "|" << center("Dau f(x_k)", 15) << "|\n";
    cout << "------+---------------+---------------+---------------+----------------\n";

    // Vòng lặp chạy đúng n bước đã tính toán
    for (int k = 1; k <= n; ++k) {
        c = (a + b) / 2.0;

        cout << "|" << center(k, 5)
             << "|" << center(a, 15)
             << "|" << center(b, 15)
             << "|" << center(c, 15)
             << "|" << center((f(c) > 0 ? "+" : "-"), 15) << "|\n";

        // Nếu f(c) = 0 thì dừng sớm vì đã tìm thấy nghiệm chính xác
        if (abs(f(c)) < 1e-15) break;

        if (f(a) * f(c) < 0)
            b = c;
        else
            a = c;
    }

    cout << "------+---------------+---------------+---------------+----------------\n";
    cout << "Nghiem gan dung tim duoc: " << setprecision(10) << c << "\n";
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    double a, b, epsilon;
    if (!(cin >> a >> b >> epsilon)) return 0;

    // Kiểm tra điều kiện đầu vào trái dấu [cite: 77]
    if (f(a) * f(b) >= 0) {
        cout << "Loi: Ham so khong trai dau tai 2 dau mut [a, b]!\n";
    } else {
        bisection_priori(a, b, epsilon);
    }

    return 0;
}