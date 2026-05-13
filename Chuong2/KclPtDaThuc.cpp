#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iomanip>

using namespace std;

// Cấu trúc lưu trữ khoảng cách ly [a, b]
struct Interval {
    double a, b;
};

// 1. Tính giá trị đa thức P(x) theo thuật toán Horner
double evaluate(const vector<double>& coeffs, double x) {
    double result = 0;
    for (double c : coeffs) {
        result = result * x + c;
    }
    return result;
}

// 2. Tính đạo hàm bậc nhất P'(x)
vector<double> get_derivative(const vector<double>& coeffs) {
    int n = coeffs.size() - 1;
    if (n == 0) return {0};
    vector<double> deriv;
    for (int i = 0; i < n; ++i) {
        deriv.push_back(coeffs[i] * (n - i));
    }
    return deriv;
}

// 3. Xác định bán kính miền chứa nghiệm R0 [cite: 215]
double get_R0(const vector<double>& coeffs) {
    double max_ak = 0;
    int n = coeffs.size() - 1;
    for (int i = 1; i <= n; ++i) {
        max_ak = max(max_ak, abs(coeffs[i]));
    }
    // Công thức: R0 = 1 + max|ak| / |an|
    return 1.0 + max_ak / abs(coeffs[0]);
}

// Hàm đệ quy để tìm nghiệm thực (phục vụ việc tìm cực trị cho bậc cao hơn)
vector<double> find_roots_recursive(vector<double> coeffs) {
    int n = coeffs.size() - 1;
    if (n == 1) return {-coeffs[1] / coeffs[0]};

    vector<double> extrema = find_roots_recursive(get_derivative(coeffs));
    double R = get_R0(coeffs);

    vector<double> points = {-R};
    for (double x : extrema) if (x > -R && x < R) points.push_back(x);
    points.push_back(R);
    sort(points.begin(), points.end());

    vector<double> roots;
    for (size_t i = 0; i < points.size() - 1; ++i) {
        double a = points[i], b = points[i+1];
        if (evaluate(coeffs, a) * evaluate(coeffs, b) <= 0) {
            // Chia đôi nhanh để xác định vị trí nghiệm chính xác làm cực trị cho bậc sau
            for(int j=0; j<64; j++) {
                double c = (a + b) / 2.0;
                if (evaluate(coeffs, a) * evaluate(coeffs, c) <= 0) b = c;
                else a = c;
            }
            roots.push_back((a + b) / 2.0);
        }
    }
    return roots;
}

// 4. THUẬT TOÁN XÁC ĐỊNH CÁC KHOẢNG CÁCH LY (CÂU 15)
vector<Interval> find_isolation_intervals(const vector<double>& coeffs) {
    // Bước 1: Tính đạo hàm P'(x)
    vector<double> derivative_coeffs = get_derivative(coeffs);
    
    // Bước 2: Tìm các điểm cực trị (nghiệm của P'(x)) bằng đệ quy
    vector<double> extrema = find_roots_recursive(derivative_coeffs);

    // Bước 3: Xác định bán kính R0 [cite: 215]
    double R0 = get_R0(coeffs);

    // Bước 4: Thiết lập danh sách các điểm chốt {-R0, c1, c2, ..., cm, R0}
    vector<double> points;
    points.push_back(-R0);
    for (double x : extrema) {
        if (x > -R0 && x < R0) points.push_back(x);
    }
    points.push_back(R0);
    sort(points.begin(), points.end());

    // Bước 5: Xét dấu f(a)*f(b) trên từng khoảng con để tìm khoảng cách ly
    vector<Interval> isolation_list;
    for (size_t i = 0; i < points.size() - 1; ++i) {
        double a = points[i];
        double b = points[i+1];
        if (evaluate(coeffs, a) * evaluate(coeffs, b) < 0) {
            isolation_list.push_back({a, b});
        }
    }
    return isolation_list;
}

int main() {
    int n;
    cout << "Nhap bac cua da thuc n: "; cin >> n;
    vector<double> coeffs(n + 1);
    cout << "Nhap cac he so tu a[n] den a[0]:" << endl;
    for (int i = 0; i <= n; ++i) {
        cout << "a[" << n - i << "] = "; cin >> coeffs[i];
    }

    // Xuất kết quả các khoảng cách ly
    vector<Interval> intervals = find_isolation_intervals(coeffs);

    cout << "\n--- KET QUA KHOANG CACH LY ---" << endl;
    if (intervals.empty()) {
        cout << "Khong co khoang cach ly nao (Phuong trinh vo nghiem thuc)." << endl;
    } else {
        cout << "Tim thay " << intervals.size() << " khoang cach ly nghiem:" << endl;
        cout << fixed << setprecision(6);
        for (size_t i = 0; i < intervals.size(); ++i) {
            cout << "Khoang " << i + 1 << ": (" << intervals[i].a << ", " << intervals[i].b << ")" << endl;
            cout << "   Kiem tra: f(" << intervals[i].a << ") = " << evaluate(coeffs, intervals[i].a) 
                 << " | f(" << intervals[i].b << ") = " << evaluate(coeffs, intervals[i].b) << endl;
        }
    }

    return 0;
}