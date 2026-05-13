#include <iostream>
#include <cmath>
#include <iomanip>
#include <algorithm>
#include <functional>

using namespace std;

// Định nghĩa hàm phi(x) và đạo hàm phi'(x)
double phi(double x) {
    return log(10.0 * x - 7.0);
}

double phi_derivative(double x) {
    return 10.0 / (10.0 * x - 7.0);
}

// Hàm tính hệ số co q trên khoảng [a, b] bằng cách lấy mẫu
double calculate_q(double a, double b) {
    double max_q = 0;
    int samples = 1000;
    double step = (b - a) / samples;
    for (int i = 0; i <= samples; ++i) {
        max_q = max(max_q, abs(phi_derivative(a + i * step)));
    }
    return max_q;
}

int main() {
    double a, b, eps;

    // Nhập đầu vào
    cout << "Nhap khoang cach ly nghiem [a, b]: ";
    cin >> a >> b;
    cout << "Nhap sai so cho phep (eps): ";
    cin >> eps;

    // 1. Tính hệ số co q 
    double q = calculate_q(a, b);
    if (q >= 1) {
        cout << "He so co q = " << q << " >= 1. Phuong phap co the khong hoi tu!" << endl;
        return 1;
    }

    // 2. Điểm bắt đầu x0 = (a + b) / 2
    double x0 = (a + b) / 2.0;
    double x1 = phi(x0);

    // Xuất thông tin ban đầu
    cout << fixed << setprecision(10);
    cout << "\n--- THONG TIN BAN DAU ---" << endl;
    cout << "He so co q: " << q << endl;
    cout << "Diem bat dau x0: " << x0 << endl;

    // 4. Bảng vòng lặp
    cout << "\n--- BANG VONG LAP ---" << endl;
    cout << setw(5) << "k" << setw(15) << "x_k" << setw(20) << "Sai so hau nghiem" << endl;
    cout << "----------------------------------------------------------------" << endl;

    double x_curr = x1;
    double x_prev = x0;
    double error = (q / (1.0 - q)) * abs(x_curr - x_prev); // 
    int k = 1;

    cout << setw(5) << 0 << setw(15) << x0 << setw(20) << "-" << endl;

    while (true) {
        // Tính sai số hậu nghiệm 
        error = (q / (1.0 - q)) * abs(x_curr - x_prev);
        
        cout << setw(5) << k << setw(15) << x_curr << setw(20) << error << endl;

        if (error < eps) break;

        // Cập nhật cho bước tiếp theo
        x_prev = x_curr;
        x_curr = phi(x_prev);
        k++;

        // Tránh vòng lặp vô hạn
        if (k > 1000) break;
    }

    cout << "------------------------------------------------------------------" << endl;
    cout << "Nghiem xap xi cuoi cung: " << x_curr << endl;

    return 0;
}