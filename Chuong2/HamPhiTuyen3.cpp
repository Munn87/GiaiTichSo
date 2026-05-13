#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

// Định nghĩa các hàm lặp phi(x) từ ảnh của bạn
double phi1(double x1, double x2, double x3) {
    return (1.0/3.0)*cos(x2*x3) + 1.0/6.0 ;
}

double phi2(double x1, double x2, double x3) {
    return (-1.0/9.0)*pow(x1*x1 + sin(x3)+1.06, 1.0/2.0) - 0.1;
}

double phi3(double x1, double x2, double x3) {
    return (-1.0/2.0)*exp(-1*x1*x2) - (10.0*M_PI - 3.0)/60.0;
}

int main() {
    // Khởi tạo giá trị ban đầu X0 = (0, 0, 0)
    double x1 = 0.0, x2 = 0.0, x3 = 0.0;
    double x1_next, x2_next, x3_next;
    
    double q ;
    cout << "He so co : " ;
    cin >> q;
    double epsilon;
    cout << "epsilon :";
    cin >> epsilon;
    
    int iteration = 0;
    double error;

    cout << fixed << setprecision(10);
    cout << "Iteration |      x1      |      x2      |      x3      |    Error\n";
    cout << "----------------------------------------------------------------------\n";

    do {
        iteration++;
        
        // Tính bước lặp tiếp theo
        x1_next = phi1(x1, x2, x3);
        x2_next = phi2(x1, x2, x3);
        x3_next = phi3(x1, x2, x3);

        // Tính chuẩn vô cùng của (X_n - X_{n-1})
        double diff = max({abs(x1_next - x1), abs(x2_next - x2), abs(x3_next - x3)});
        
        // Đánh giá sai số theo công thức hậu nghiệm: diff * q / (1 - q)
        error = diff * (q / (1.0 - q));

        // Cập nhật giá trị cho vòng lặp sau
        x1 = x1_next;
        x2 = x2_next;
        x3 = x3_next;

        cout << setw(9) << iteration << " | " 
             << setw(12) << x1 << " | " 
             << setw(12) << x2 << " | " 
             << setw(12) << x3 << " | " 
             << error << endl;

    } while (error > epsilon);

    cout << "----------------------------------------------------------------------\n";
    cout << "Nghiem xap xi cuoi cung sau " << iteration << " buoc lap:\n";
    cout << "x1 = " << x1 << "\nx2 = " << x2 << "\nx3 = " << x3 << endl;

    return 0;
}