#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

// Lưu ý: coeffs[0] là hệ số x^n, coeffs[n] là hệ số tự do 
double evaluate(const vector<double>& coeffs, double x) {
    double result = 0;
    for (double c : coeffs) {
        result = result * x + c; // Thuật toán Horner chuẩn
    }
    return result;
}

vector<double> get_derivative(const vector<double>& coeffs) {
    int n = coeffs.size() - 1;
    if (n == 0) return {0};
    vector<double> deriv;
    for (int i = 0; i < n; ++i) {
        deriv.push_back(coeffs[i] * (n - i));
    }
    return deriv;
}

double get_R0(const vector<double>& coeffs) {
    double max_ak = 0;
    int n = coeffs.size() - 1;
    for (int i = 1; i <= n; ++i) {
        max_ak = max(max_ak, abs(coeffs[i]));
    }
    return 1.0 + max_ak / abs(coeffs[0]); // [cite: 347, 348]
}

// Hàm đệ quy tìm nghiệm (để tìm cực trị)
vector<double> find_roots_recursive(vector<double> coeffs, double eps) {
    int n = coeffs.size() - 1;
    if (n == 1) return {-coeffs[1] / coeffs[0]};

    vector<double> extrema = find_roots_recursive(get_derivative(coeffs), eps);
    double R = get_R0(coeffs);

    vector<double> points = {-R};
    for (double x : extrema) if (x > -R && x < R) points.push_back(x);
    points.push_back(R);
    sort(points.begin(), points.end());

    vector<double> roots;
    for (size_t i = 0; i < points.size() - 1; ++i) {
        double a = points[i], b = points[i+1];
        if (evaluate(coeffs, a) * evaluate(coeffs, b) <= 0) {
            // Chia đôi nhanh để lấy điểm cực trị chính xác cho bậc trên
            for(int j=0; j<50; j++) {
                double c = (a+b)/2.0;
                if (evaluate(coeffs, a) * evaluate(coeffs, c) <= 0) b = c;
                else a = c;
            }
            roots.push_back((a + b) / 2.0);
        }
    }
    return roots;
}

int main() {
    int n;
    cout << "Nhap bac cua da thuc n: "; cin >> n;
    
    vector<double> coeffs(n + 1);
    cout << "Nhap cac he so tu a[" << n << "] den a[0] (Tu bac cao den thap):" << endl;
    for (int i = 0; i <= n; ++i) {
        cout << "a[" << n - i << "] = "; cin >> coeffs[i];
    }

    double eps;
    cout << "Nhap sai so epsilon: "; cin >> eps;

    // 1. Xác định miền chứa nghiệm R0 [cite: 347]
    double R0 = get_R0(coeffs);
    
    // 2. Tìm cực trị (nghiệm của đạo hàm) [cite: 312]
    vector<double> extrema = find_roots_recursive(get_derivative(coeffs), eps);
    
    // 3. Thiết lập các điểm chốt để tìm khoảng cách ly [cite: 310, 312]
    vector<double> points = {-R0};
    for (double x : extrema) points.push_back(x);
    points.push_back(R0);
    sort(points.begin(), points.end());

    cout << "\n--- CAC KHOANG CACH LY NGHIEM ---" << endl;
    vector<pair<double, double>> intervals;
    for (size_t i = 0; i < points.size() - 1; ++i) {
        double a = points[i], b = points[i+1];
        if (evaluate(coeffs, a) * evaluate(coeffs, b) < 0) {
            intervals.push_back({a, b});
            cout << "Khoang " << intervals.size() << ": (" << fixed << setprecision(4) << a << ", " << b << ")" << endl;
        }
    }

    cout << "\n--- KET QUA NGHIEM (CHIA DOI) ---" << endl;
    for (auto& range : intervals) {
        double a = range.first, b = range.second;
        // Thuật toán Chia đôi [cite: 321-332]
        while ((b - a) > eps) {
            double c = (a + b) / 2.0;
            if (evaluate(coeffs, a) * evaluate(coeffs, c) <= 0) b = c;
            else a = c;
        }
        cout << "x = " << fixed << setprecision(10) << (a + b) / 2.0 << endl;
    }

    return 0;
}