#include <iostream>
#include <cmath>
#include <iomanip>
#include <algorithm>

using namespace std;

// 1. Định nghĩa hàm số và các đạo hàm
double f(double x) {
    return pow(x, 3) - x - 1; // Ví dụ: x^3 - x - 1 = 0
}

double f_prime(double x) {
    return 3 * pow(x, 2) - 1;
}

double f_double_prime(double x) {
    return 6 * x;
}

void newtonFourier(double a, double b, double epsilon) {
    double x_tan, x_chord; // x_tan: đầu tiếp tuyến, x_chord: đầu dây cung
    int k = 0;

    // B2: Kiểm tra điều kiện Fourier
    // Chọn đầu đồ thị lồi về phía nghiệm làm điểm bắt đầu cho tiếp tuyến
    if (f(a) * f_double_prime(a) > 0) {
        x_tan = a;
        x_chord = b;
        cout << "Diem Fourier: x0 = a = " << a << " (Dung tiep tuyen)\n";
    } else {
        x_tan = b;
        x_chord = a;
        cout << "Diem Fourier: x0 = b = " << b << " (Dung tiep tuyen)\n";
    }

    cout << "--------------------------------------------------------------------------\n";
    cout << setw(5) << "k" << " | " << setw(15) << "Tiep tuyen (xk)" << " | " 
         << setw(15) << "Day cung (x'k)" << " | " << setw(15) << "Sai so" << endl;
    cout << "--------------------------------------------------------------------------\n";

    double error = abs(x_tan - x_chord);
    
    // B3 & B4: Vong lap bien doi va kiem tra dieu kien dung
    while (error > epsilon && k < 100) {
        double x_tan_old = x_tan;
        double x_chord_old = x_chord;

        // B3.1: Cong thuc tiep tuyen (Newton)
        x_tan = x_tan_old - f(x_tan_old) / f_prime(x_tan_old);

        // B3.2: Cong thuc day cung (Ket hop voi diem x_tan moi tim duoc)
        x_chord = x_chord_old - (f(x_chord_old) * (x_chord_old - x_tan)) / (f(x_chord_old) - f(x_tan));

        error = abs(x_tan - x_chord);
        k++;

        cout << fixed << setprecision(10);
        cout << setw(5) << k << " | " << setw(15) << x_tan << " | " 
             << setw(15) << x_chord << " | " << setw(15) << error << endl;
    }

    cout << "--------------------------------------------------------------------------\n";
    cout << "Nghiem xap xi cuoi cung: x = " << (x_tan + x_chord) / 2.0 << endl;
}

int main() {
    double a, b, epsilon;

    cout << "--- PHUONG PHAP KET HOP DAY CUNG & TIEP TUYEN ---\n";
    cout << "Nhap khoang cach ly nghiem [a, b]:\n";
    cout << "a = "; cin >> a;
    cout << "b = "; cin >> b;
    cout << "Nhap sai so epsilon: "; cin >> epsilon;

    // Kiem tra dieu kien cach ly nghiem
    if (f(a) * f(b) >= 0) {
        cout << "Loi: Ham so khong doi dau tren doan [" << a << ", " << b << "]\n";
    } else {
        newtonFourier(a, b, epsilon);
    }

    return 0;
}