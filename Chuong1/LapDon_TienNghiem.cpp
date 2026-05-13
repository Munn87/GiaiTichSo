#include <iostream>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;

// 1. Hàm lặp phi(x) biến đổi từ f(x) = x^3 + 3x^2 - 1 = 0
// x = cbrt(1 - 3x^2)
double phi(double x) {
    return cbrt(1.0 - 3.0 * x * x);
}

// 2. Đạo hàm phi'(x) để xác định hệ số co q
double phi_prime(double x) {
    double inner = 1.0 - 3.0 * x * x;
    double mau = pow(cbrt(inner), 2); 
    if (abs(mau) < 1e-9) return 999999; 
    return (-2.0 * x) / mau;
}

// 3. Tìm hệ số co q = max |phi'(x)| trên [a, b]
double get_q(double a, double b) {
    double q = 0;
    for (double x = min(a, b); x <= max(a, b); x += 0.001) {
        q = max(q, abs(phi_prime(x)));
    }
    return q;
}

int main() {
    double a, b, epsilon, x0, x1;
    
    cout << "--- PHUONG PHAP LAP DON (SAI SO TIEN NGHIEM) ---" << endl;
    cout << "Nhap khoang cach li nghiem [a, b]: "; cin >> a >> b;
    cout << "Nhap sai so epsilon: "; cin >> epsilon;
    
    // Chọn điểm xuất phát x0
    x0 = (a + b) / 2.0; 
    double q = get_q(a, b);
    
    // Kiểm tra điều kiện hội tụ q < 1
    if (q >= 1) {
        cout << "Loi: He so co q = " << q << " >= 1. Phuong phap khong chac chan hoi tu!" << endl;
        return 1;
    }

    // Tính x1 để xác định khoảng cách ban đầu d0 = |x1 - x0|
    x1 = phi(x0);
    double d0 = abs(x1 - x0);

    // --- XÁC ĐỊNH SỐ BƯỚC LẶP n THEO TIÊN NGHIỆM ---
    // Công thức: q^n / (1-q) * |x1 - x0| <= epsilon
    // => n >= (ln(epsilon * (1-q)) - ln(|x1 - x0|)) / ln(q)
    int n = ceil((log(epsilon * (1.0 - q)) - log(d0)) / log(q));
    if (n < 0) n = 0;

    cout << fixed << setprecision(10);
    cout << "He so co q: " << q << endl;
    cout << "Khoang cach |x1 - x0|: " << d0 << endl;
    cout << "So buoc lap can thiet n = " << n << " (Tinh theo sai so tien nghiem)" << endl;
    cout << "------------------------------------------------------------" << endl;
    cout << setw(5) << "k" << setw(20) << "x_k" << setw(25) << "Sai so tien nghiem" << endl;
    cout << "------------------------------------------------------------" << endl;

    double current_x = x0;
    
    // Vòng lặp chạy đúng n bước đã tính toán
    for (int k = 0; k <= n; ++k) {
        // Tính sai số tiên nghiệm tại bước k: (q^k / (1-q)) * |x1 - x0|
        double error_priori = (pow(q, k) / (1.0 - q)) * d0;
        
        cout << setw(5) << k << setw(20) << current_x << setw(25) << error_priori << endl;
        
        if (k < n) {
            current_x = phi(current_x);
        }
    }

    cout << "------------------------------------------------------------" << endl;
    cout << "Nghiem gan dung sau " << n << " buoc: " << current_x << endl;

    return 0;
}
