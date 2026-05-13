#include <iostream>
#include <vector>
#include <cmath>
#include <iomanip>
#include <functional>
#include <string>

using namespace std;

const double PI = acos(-1.0);

// =========================================================
// PHẦN 1: ĐỊNH NGHĨA BÀI TOÁN CỤ THỂ
// =========================================================

vector<double> G_function(const vector<double>& x) {
    vector<double> x_new(x.size());
    
    x_new[0] = (cos(x[1] * x[2]) + 0.5) / 3.0;
    x_new[1] = (1.0 / 25.0) * sqrt(x[0] * x[0] + 0.3125) - 0.03;
    
    double term = (10.0 * PI - 3.0) / 60.0;
    x_new[2] = -(1.0 / 20.0) * exp(-x[0] * x[1]) - term;
    
    return x_new;
}

// =========================================================
// PHẦN 2: THUẬT TOÁN LẶP ĐƠN TỔNG QUÁT
// =========================================================

double calculate_error(const vector<double>& v1, const vector<double>& v2) {
    double max_err = 0.0;
    for (size_t i = 0; i < v1.size(); ++i) {
        max_err = max(max_err, abs(v1[i] - v2[i]));
    }
    return max_err;
}

// Đã đổi tham số: Nhận vào q và epsilon thay vì tol cứng
void he_lap_don(function<vector<double>(const vector<double>&)> G, 
                vector<double> x0, 
                double q, 
                double epsilon, 
                int max_iter = 100) {
    
    // Tự động tính toán ngưỡng dừng (tolerance) dựa trên q và epsilon
    double tol = epsilon * (1.0 - q) / q;
    
    int n_vars = x0.size();
    vector<double> x = x0;
    vector<double> x_new(n_vars);
    double diff = 1.0; // ||X_k - X_{k-1}||
    int iter = 0;

    // --- Cài đặt hiển thị ---
    cout << fixed << setprecision(8);
    int line_width = 10 + n_vars * 15 + 15; 
    cout << string(line_width, '=') << "\n";
    cout << "  He so co q = " << q << " | Sai so yeu cau e = " << epsilon << "\n";
    cout << "  => Nguong dung dieu kien (tol) = " << tol << "\n";
    cout << string(line_width, '=') << "\n";
    
    cout << setw(3) << "k";
    for (int i = 0; i < n_vars; ++i) {
        cout << setw(15) << "x" + to_string(i + 1);
    }
    cout << setw(15) << "Hieu so" << "\n";
    cout << string(line_width, '=') << "\n";

    // --- Vòng lặp chính ---
    while (iter < max_iter) {
        iter++;
        
        // 1. Tính X mới
        x_new = G(x);
        
        // 2. Tính hiệu số giữa 2 lần lặp liên tiếp ||X_new - X_old||
        diff = calculate_error(x_new, x);

        // 3. In kết quả
        cout << setw(5) << iter;
        for (int i = 0; i < n_vars; ++i) {
            cout << setw(15) << x_new[i];
        }
        cout << setw(15) << diff << "\n";

        // 4. Kiểm tra điều kiện dừng với ngưỡng tol đã tính ở trên
        if (diff <= tol) {
            cout << string(line_width, '=') << "\n";
            cout << "\n[+] Thuat toan HOI TU sau " << iter << " lan lap.\n";
            
            // Tính lại sai số thực tế đạt được ở bước cuối
            double actual_error = (q / (1.0 - q)) * diff;
            cout << "[-] Sai so hau nghiem thuc te: " << actual_error << " <= " << epsilon << "\n";
            cout << "[-] Nghiem gan dung la:\n";
            for(int i = 0; i < n_vars; ++i) {
                cout << "    x" << i + 1 << " = " << x_new[i] << "\n";
            }
            return;
        }

        // 5. Cập nhật
        x = x_new;
    }

    cout << string(line_width, '=') << "\n";
    cout << "\n[!] CANH BAO: Thuat toan KHONG HOI TU sau " << max_iter << " lan lap!\n";
}

// =========================================================
// PHẦN 3: HÀM MAIN THỰC THI
// =========================================================

int main() {
    // 1. Giá trị khởi tạo
    vector<double> x_init = {0.0, 0.0, 0.0}; 
    
    // 2. Cấu hình bài toán (Thay đổi trực tiếp tại đây)
    double q = 0.5609;        // Hệ số co (tự tính bằng tay từ chuẩn ma trận Jacobi)
    double epsilon = 1e-6;    // Sai số yêu cầu (ví dụ 10^-6)
    int max_iter = 100;

    // 3. Chạy thuật toán
    he_lap_don(G_function, x_init, q, epsilon);

    return 0;
}
