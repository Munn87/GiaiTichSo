#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>

using namespace std;

// --- Định nghĩa các hàm f1, f2, f3 ---
double f1(double x1, double x2, double x3) {
    return 10*x1 - 2*x2*x2 + x2 - 2*x3 - 5;
}

double f2(double x1, double x2, double x3) {
    return 8*x2*x2 + 4*x3*x3 - 9;
}

double f3(double x1, double x2, double x3) {
    return 8*x2*x3 + 4;
}

// --- Tính ma trận Jacobi J(X) ---
void get_jacobi(double x1, double x2, double x3, double J[3][3]) {
    J[0][0] = 10;
    J[0][1] = -4*x2 + 1;
    J[0][2] = -2.0;

    J[1][0] = 0.0;
    J[1][1] = 16*x2;
    J[1][2] = 8*x3;

    J[2][0] = 0;
    J[2][1] = 8*x3;
    J[2][2] = 8*x2;
}

// --- Hàm tính định thức ma trận 3x3 ---
double determinant(double m[3][3]) {
    return m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
           m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
           m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
}

// --- Hàm tìm ma trận nghịch đảo 3x3 ---
bool inverse3x3(double m[3][3], double inv[3][3]) {
    double det = determinant(m);
    if (abs(det) < 1e-15) return false;

    double invDet = 1.0 / det;
    inv[0][0] = (m[1][1] * m[2][2] - m[1][2] * m[2][1]) * invDet;
    inv[0][1] = (m[0][2] * m[2][1] - m[0][1] * m[2][2]) * invDet;
    inv[0][2] = (m[0][1] * m[1][2] - m[0][2] * m[1][1]) * invDet;
    inv[1][0] = (m[1][2] * m[2][0] - m[1][0] * m[2][2]) * invDet;
    inv[1][1] = (m[0][0] * m[2][2] - m[0][2] * m[2][0]) * invDet;
    inv[1][2] = (m[1][0] * m[0][2] - m[0][0] * m[1][2]) * invDet;
    inv[2][0] = (m[1][0] * m[2][1] - m[1][1] * m[2][0]) * invDet;
    inv[2][1] = (m[2][0] * m[0][1] - m[0][0] * m[2][1]) * invDet;
    inv[2][2] = (m[0][0] * m[1][1] - m[1][0] * m[0][1]) * invDet;

    return true;
}

// --- In ma trận 3x3 ---
void print_matrix(double m[3][3]) {
    for (int i = 0; i < 3; i++) {
        cout << "| ";
        for (int j = 0; j < 3; j++) cout << setw(12) << m[i][j] << " ";
        cout << " |" << endl;
    }
}

int main() {
    double x[3] = {0.01, 0.01, 0.01}; // X0
    double epsilon;
    cout << "Nhap sai so epsilon: "; cin >> epsilon;

    cout << fixed << setprecision(8);
    cout << "\n=== TINH TOAN BUOC DAU TIEN (X1) ===" << endl;

    double F[3] = { f1(x[0], x[1], x[2]), f2(x[0], x[1], x[2]), f3(x[0], x[1], x[2]) };
    double J[3][3], invJ[3][3];
    
    get_jacobi(x[0], x[1], x[2], J);
    if (!inverse3x3(J, invJ)) {
        cout << "Loi: Jacobi khong the nghich dao tai X0!" << endl;
        return 1;
    }

    cout << "Ma tran nghich dao Jacobi J^-1(X0):" << endl;
    print_matrix(invJ);

    // X1 = X0 - J^-1 * F
    double dx[3] = {0, 0, 0};
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) dx[i] -= invJ[i][j] * F[j];
    }
    
    x[0] += dx[0]; x[1] += dx[1]; x[2] += dx[2];
    double error = max({abs(dx[0]), abs(dx[1]), abs(dx[2])});

    cout << "Nghiem xap xi X1: (" << x[0] << ", " << x[1] << ", " << x[2] << ")" << endl;
    cout << "Sai so tai buoc 1: " << error << endl;

    cout << "\n=== CAC BUOC LAP TIEP THEO ===" << endl;
    int k = 1;
    while (error > epsilon && k < 50) {
        F[0] = f1(x[0], x[1], x[2]); 
        F[1] = f2(x[0], x[1], x[2]); 
        F[2] = f3(x[0], x[1], x[2]);

        get_jacobi(x[0], x[1], x[2], J);
        inverse3x3(J, invJ);

        for (int i = 0; i < 3; i++) dx[i] = 0;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) dx[i] -= invJ[i][j] * F[j];
        }

        x[0] += dx[0]; x[1] += dx[1]; x[2] += dx[2];
        error = max({abs(dx[0]), abs(dx[1]), abs(dx[2])});
        k++;

        cout << "Buoc lap " << k << ": X = (" << x[0] << ", " << x[1] << ", " << x[2] << ")";
        cout << " | Sai so: " << error << endl;
    }

    cout << "\nKet qua cuoi cung: X = (" << x[0] << ", " << x[1] << ", " << x[2] << ")" << endl;
    return 0;
}