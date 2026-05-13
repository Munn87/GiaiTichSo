#include <iostream>
#include <cmath>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

// 1. Định nghĩa các hàm lặp phi1(x1, x2) và phi2(x1, x2)
// Thay đổi nội dung hàm theo hệ phương trình cụ thể của bạn [cite: 125, 136-137]
double phi1(double x1, double x2) {
    return (1.0 / 3.0) * sin(x1 * x2); 
}

double phi2(double x1, double x2) {
    return (1.0 / 4.0) * cos(x1 * x1 + x2 * x2);
}

int main() {
    // 2. Khởi tạo giá trị ban đầu X0 = (x1, x2) [cite: 129]
    double x1 = 0.1667, x2 = 0.125; 
    double x1_next, x2_next;
    
    double q; 
    cout << "Nhap he so co q (0 < q < 1): ";
    cin >> q;
    
    double epsilon;
    cout << "Nhap sai so epsilon: ";
    cin >> epsilon;
    
    int iteration = 0;
    double error;

    cout << fixed << setprecision(10);
    cout << "\nIteration |      x1      |      x2      |    Error\n";
    cout << "----------------------------------------------------------\n";

    // 3. Quá trình lặp đơn [cite: 130-142]
    do {
        iteration++;
        
        // Tính giá trị bước lặp tiếp theo [cite: 135-137]
        x1_next = phi1(x1, x2);
        x2_next = phi2(x1, x2);

        // Tính chuẩn vô cùng của hiệu hai bước lặp: max(|x_n - x_{n-1}|) [cite: 140]
        double diff = max(abs(x1_next - x1), abs(x2_next - x2));
        
        // Đánh giá sai số theo công thức hậu nghiệm: diff * q / (1 - q) [cite: 86, 94]
        error = diff * (q / (1.0 - q));

        // Cập nhật giá trị cho vòng lặp sau [cite: 133-134]
        x1 = x1_next;
        x2 = x2_next;

        cout << setw(9) << iteration << " | " 
             << setw(12) << x1 << " | " 
             << setw(12) << x2 << " | " 
             << error << endl;

        // Giới hạn an toàn để tránh vòng lặp vô hạn
        if (iteration > 100) break;

    } while (error > epsilon); // Điều kiện dừng dựa trên sai số [cite: 86, 140]

    cout << "----------------------------------------------------------\n";
    cout << "Nghiem xap xi cuoi cung sau " << iteration << " buoc lap:\n";
    cout << "x1 = " << x1 << "\nx2 = " << x2 << endl;

    return 0;
}