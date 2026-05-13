#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>

using namespace std;

// 1. Định nghĩa hệ phương trình F(x, y) = 0
// f1 = x - (1/3)*sin(xy) = 0
// f2 = y - (1/4)*cos(x^2 + y^2) = 0
double f1(double x, double y) { return x*x + 2.0*y*y - 4.0; }
double f2(double x, double y) { return 5*y*y + x*x - 4.0; }

// 2. Các đạo hàm riêng (Ma trận Jacobi)
double df1_dx(double x, double y) { return 1.0 - (1.0/3.0)*y*cos(x*y); }
double df1_dy(double x, double y) { return -(1.0/3.0)*x*cos(x*y); }
double df2_dx(double x, double y) { return 0.5 * x * sin(x*x + y*y); }
double df2_dy(double x, double y) { return 1.0 + 0.5 * y * sin(x*x + y*y); }

int main() {
    double x = 0.1667, y = 0.125; // Xấp xỉ đầu X0
    double eps;
    cout << "Nhap sai so epsilon: "; cin >> eps;

    cout << fixed << setprecision(8);
    cout << "\n=== BUOC LAP DAU TIEN (X1) ===" << endl;

    // Tính F(X0) và J(X0)
    double F1 = f1(x, y), F2 = f2(x, y);
    double j11 = df1_dx(x, y), j12 = df1_dy(x, y);
    double j21 = df2_dx(x, y), j22 = df2_dy(x, y);

    // Tính ma trận nghịch đảo J^-1
    double det = j11 * j22 - j12 * j21;
    if (abs(det) < 1e-15) {
        cout << "Loi: Ma tran Jacobi suy bien!" << endl;
        return 1;
    }

    double invJ11 = j22 / det, invJ12 = -j12 / det;
    double invJ21 = -j21 / det, invJ22 = j11 / det;

    cout << "Ma tran nghich dao Jacobi J^-1(X0):" << endl;
    cout << "| " << setw(12) << invJ11 << " " << setw(12) << invJ12 << " |" << endl;
    cout << "| " << setw(12) << invJ21 << " " << setw(12) << invJ22 << " |" << endl;

    // Tính X1 = X0 - J^-1 * F
    double dx = invJ11 * F1 + invJ12 * F2;
    double dy = invJ21 * F1 + invJ22 * F2;
    x -= dx; y -= dy;

    double error = max(abs(dx), abs(dy)); // Chuẩn vô cùng
    cout << "Nghiem X1 = (" << x << ", " << y << ")" << endl;
    cout << "Sai so: " << error << endl;

    cout << "\n=== CAC BUOC LAP TIEP THEO ===" << endl;
    int k = 1;
    while (error > eps && k < 50) {
        F1 = f1(x, y); F2 = f2(x, y);
        j11 = df1_dx(x, y); j12 = df1_dy(x, y);
        j21 = df2_dx(x, y); j22 = df2_dy(x, y);
        
        det = j11 * j22 - j12 * j21;
        invJ11 = j22 / det; invJ12 = -j12 / det;
        invJ21 = -j21 / det; invJ22 = j11 / det;

        dx = invJ11 * F1 + invJ12 * F2;
        dy = invJ21 * F1 + invJ22 * F2;
        x -= dx; y -= dy;
        
        error = max(abs(dx), abs(dy));
        k++;
        cout << "Buoc " << k << ": X = (" << x << ", " << y << ") | Sai so: " << error << endl;
    }

    cout << "\nNghiem cuoi cung: X = (" << x << ", " << y << ")" << endl;
    return 0;
}
