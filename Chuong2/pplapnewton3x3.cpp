#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

using namespace std;

const double PI = acos(-1.0);

// --- Định nghĩa các hàm f1, f2, f3 của Câu 22a ---
double f1(double x1, double x2, double x3) {
    return 10*x1 - 2*x2*x2 + x2 - 2*x3 - 5;
}

double f2(double x1, double x2, double x3) {
    return 8*x2*x2 + 4*x3*x3 - 9;
}

double f3(double x1, double x2, double x3) {
    return 8*x2*x3 + 4;
}

// --- Ma trận Jacobi J(X) cho hệ 3 ẩn ---
void get_jacobi(double x1, double x2, double x3, double J[3][3]) {
    // Hàng 1: Đạo hàm của f1 theo x1, x2, x3
    J[0][0] = 10;
    J[0][1] = -4*x2 + 1;
    J[0][2] = -2.0;

    // Hàng 2: Đạo hàm của f2 theo x1, x2, x3
    J[1][0] = 0.0;
    J[1][1] = 16*x2;
    J[1][2] = 8*x3;

    // Hàng 3: Đạo hàm của f3 theo x1, x2, x3
    J[2][0] = 0;
    J[2][1] = 8*x3;
    J[2][2] = 8*x2;
}

// --- Hàm giải hệ phương trình tuyến tính J * delta = -F bằng khử Gauss ---
bool solve_linear_system(double J[3][3], double F[3], double dx[3]) {
    double A[3][4];
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) A[i][j] = J[i][j];
        A[i][3] = -F[i]; // Ma trận mở rộng [J | -F]
    }

    // Khử Gauss
    for (int i = 0; i < 3; i++) {
        int pivot = i;
        for (int j = i + 1; j < 3; j++)
            if (abs(A[j][i]) > abs(A[pivot][i])) pivot = j;
        
        for (int k = i; k <= 3; k++) swap(A[i][k], A[pivot][k]);

        if (abs(A[i][i]) < 1e-15) return false; // Ma trận suy biến

        for (int j = i + 1; j < 3; j++) {
            double factor = A[j][i] / A[i][i];
            for (int k = i; k <= 3; k++) A[j][k] -= factor * A[i][k];
        }
    }

    // Thế ngược
    for (int i = 2; i >= 0; i--) {
        dx[i] = A[i][3];
        for (int j = i + 1; j < 3; j++) dx[i] -= A[i][j] * dx[j];
        dx[i] /= A[i][i];
    }
    return true;
}

int main() {
    // Xấp xỉ ban đầu X0 = (0, 0, 0)
    double x[3] = {0.0, 0.0, 0.0}; 
    int iterations = 5;

    cout << fixed << setprecision(10);
    cout << "Buoc lap 0: x1 = " << x[0] << ", x2 = " << x[1] << ", x3 = " << x[2] << endl;

    for (int k = 1; k <= iterations; ++k) {
        double F[3] = { f1(x[0], x[1], x[2]), f2(x[0], x[1], x[2]), f3(x[0], x[1], x[2]) };
        double J[3][3];
        
        // Newton chuẩn: Cập nhật Jacobi ở mỗi bước lặp 
        get_jacobi(x[0], x[1], x[2], J); 

        double dx[3];
        if (!solve_linear_system(J, F, dx)) {
            cout << "Ma tran Jacobi bi suy bien tai buoc " << k << "!" << endl;
            break;
        }

        // Cập nhật nghiệm: X_{k+1} = X_k + delta 
        x[0] += dx[0];
        x[1] += dx[1];
        x[2] += dx[2];

        cout << "Buoc lap " << k << ": x1 = " << x[0] << ", x2 = " << x[1] << ", x3 = " << x[2] << endl;
    }

    return 0;
}