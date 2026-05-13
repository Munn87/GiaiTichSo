#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

using namespace std;

const double PI = acos(-1.0);
const double E = exp(1.0);

// Hàm f1(x1, x2) 
double f1(double x1, double x2) {
    return x1*x1 + 2.0*x2*x2 - 4.0;
}

// Hàm f2(x1, x2) 
double f2(double x1, double x2) {
    return 5*x2*x2 + x1*x2 - 4.0;
}

// Ma trận Jacobi J(X)
// [ df1/dx1  df1/dx2 ]
// [ df2/dx1  df2/dx2 ]
void get_jacobi(double x1, double x2, double J[2][2]) {
    J[0][0] = 10*x1;       // df1/dx1
    J[0][1] = -2*x2;         // df1/dx2
    J[1][0] = -0.25*cos(x1); // df2/dx1
    J[1][1] = 1 + 0.25*sin(x2);       // df2/dx2
}

int main() {
    double x1 = 1.0, x2 = -3.0; // Xấp xỉ ban đầu X0
    int iterations = 5;

    cout << fixed << setprecision(10);
    cout << "Buoc lap 0: x1 = " << x1 << ", x2 = " << x2 << endl;

    for (int k = 1; k <= iterations; ++k) {
        double F1 = f1(x1, x2);
        double F2 = f2(x1, x2);
        
        double J[2][2];
        get_jacobi(x1, x2, J);

        // Giai he phuong trinh tuyen tinh J * delta = -F bang quy tac Cramer
        // J00*dx1 + J01*dx2 = -F1
        // J10*dx1 + J11*dx2 = -F2
        double det = J[0][0] * J[1][1] - J[0][1] * J[1][0];
        
        if (abs(det) < 1e-12) {
            cout << "Ma tran Jacobi bi suy bien!" << endl;
            break;
        }

        double dx1 = (-F1 * J[1][1] - (-F2 * J[0][1])) / det;
        double dx2 = (J[0][0] * (-F2) - J[1][0] * (-F1)) / det;

        // Cap nhat nghiem: X_{k+1} = X_k + delta
        x1 += dx1;
        x2 += dx2;

        cout << "Buoc lap " << k << ": x1 = " << x1 << ", x2 = " << x2 << endl;
    }

    return 0;
}