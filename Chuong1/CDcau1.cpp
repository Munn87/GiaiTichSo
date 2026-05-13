#include <iostream>
#include <cmath>
#include <iomanip>

using namespace std;

// Định nghĩa hàm f(x) theo đề bài
double f(double x) {
    return 3 * sin(x) + pow(x, 3) - 8 * pow(x, 2) + 8 * x + 1;
}

int main() {
    // Khởi tạo các thông số
    double a = 6.0, b = 7.0;
    double pi = 3.1415927;
    double delta_pi = 0.5e-7;
    double delta_V_target = 0.5e-3;
    
    double d, V, delta_d, delta_V;
    int n = 0;

    cout << fixed << setprecision(8);
    cout << "n |      a      |      b      |      d      |    delta_V   \n";
    cout << "-----------------------------------------------------------\n";

    while (true) {
        d = (a + b) / 2.0; // Bước 3: Tính d = (a+b)/2
        delta_d = (b - a) / 2.0; // Tính sai số của d
        
        V = (pi * pow(d, 3)) / 6.0; // Tính thể tích V
        
        // Tính sai số tuyệt đối delta_V theo lý thuyết sai số tương đối
        // delta_V = V * (delta_pi / pi + 3 * delta_d / d)
        delta_V = V * (delta_pi / pi + 3 * (delta_d / d));

        cout << n << " | " << a << " | " << b << " | " << d << " | " << delta_V << endl;

        // Kiểm tra điều kiện dừng
        if (delta_V <= delta_V_target) break;

        // Cập nhật khoảng cách li nghiệm
        if (f(a) * f(d) < 0) b = d;
        else a = d;
        
        n++;
        if (n > 100) break; // Tránh vòng lặp vô hạn
    }

    cout << "-----------------------------------------------------------\n";
    cout << "Ket qua sau " << n << " lan lap:\n";
    cout << "Duong kinh d xap xi: " << d << endl;
    cout << "The tich V xap xi: " << V << endl;
    cout << "Sai so delta_V: " << delta_V << endl;

    return 0;
}