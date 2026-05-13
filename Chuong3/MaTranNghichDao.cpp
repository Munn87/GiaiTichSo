#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

using namespace std;

// Hàm in ma trận
void printMatrix(const vector<vector<double>>& mat) {
    for (const auto& row : mat) {
        for (double val : row) {
            cout << setw(12) << fixed << setprecision(4) << (abs(val) < 1e-9 ? 0.0 : val) << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

int main() {
    // 1. ĐỌC FILE DỮ LIỆU (Ma trận A)
    string filename = "test.txt"; 
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Loi: Khong the mo duoc file!\n";
        return 1;
    }

    vector<vector<double>> A;
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        vector<double> row;
        double val;
        while (ss >> val) row.push_back(val);
        if (!row.empty()) A.push_back(row);
    }
    file.close();

    int n = A.size();
    if (n == 0 || A[0].size() != n) {
        cerr << "Loi: Ma tran phai la ma tran vuong de tim nghich dao!\n";
        return 1;
    }

    // 2. TẠO MA TRẬN MỞ RỘNG [A | I]
    int totalCols = 2 * n;
    vector<vector<double>> Aug(n, vector<double>(totalCols, 0.0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            Aug[i][j] = A[i][j];
        }
        Aug[i][n + i] = 1.0; // Gan ma tran don vi I vao ben phai
    }

    cout << "--- MA TRAN MO RONG BAN DAU [A|I] ---\n";
    printMatrix(Aug);

    // 3. THUẬT TOÁN GAUSS-JORDAN
    vector<int> ind(n, -1); // ind[r] = c nghia la hang r co pivot tai cot c
    vector<bool> row_used(n, false);
    vector<bool> col_used(n, false);

    for (int k = 0; k < n; ++k) {
        double max_val = 0;
        int p = -1, q = -1;
        bool found_priority_1 = false;

        // Uu tien 1: Tim phan tu bang 1 hoac -1
        for (int r = 0; r < n; ++r) {
            if (!row_used[r]) {
                for (int c = 0; c < n; ++c) {
                    if (!col_used[c] && abs(abs(Aug[r][c]) - 1) < 1e-9) {
                        p = r; q = c;
                        max_val = abs(Aug[r][c]);
                        found_priority_1 = true;
                        break;
                    }
                }
            }
            if (found_priority_1) break;
        }

        // Uu tien 2: Tim phan tu tri tuyet doi lon nhat
        if (!found_priority_1) {
            for (int r = 0; r < n; ++r) {
                if (!row_used[r]) {
                    for (int c = 0; c < n; ++c) {
                        if (!col_used[c] && abs(Aug[r][c]) > max_val) {
                            max_val = abs(Aug[r][c]);
                            p = r; q = c;
                        }
                    }
                }
            }
        }

        // Neu max_val = 0 thi ma tran suy bien, khong co nghich dao
        if (max_val < 1e-9) {
            cout << "============================================\n";
            cout << " KET LUAN: MA TRAN SUY BIEN (KHONG CO NGHICH DAO)!\n";
            cout << "============================================\n";
            return 0;
        }

        row_used[p] = true;
        col_used[q] = true;
        ind[p] = q; 

        // Chuan hoa hang pivot
        double pivot_val = Aug[p][q];
        for (int c = 0; c < totalCols; ++c) {
            Aug[p][c] /= pivot_val;
        }

        // Khu cac hang khac
        for (int r = 0; r < n; ++r) {
            if (r != p) {
                double factor = Aug[r][q];
                for (int c = 0; c < totalCols; ++c) {
                    Aug[r][c] -= factor * Aug[p][c];
                }
            }
        }
    }

    // 4. TRÍCH XUẤT MA TRẬN NGHỊCH ĐẢO
    // Vi chung ta chon pivot tu do, hang r co pivot o cot q
    // thi hang r hien tai cua ben phai chinh la hang q cua ma tran nghich dao
    vector<vector<double>> Inverse(n, vector<double>(n));
    for (int r = 0; r < n; ++r) {
        int target_row = ind[r]; // Pivot cua hang r nam o cot target_row
        for (int c = 0; c < n; ++c) {
            Inverse[target_row][c] = Aug[r][n + c];
        }
    }

    cout << "--- MA TRAN NGHICH DAO A^-1 ---\n";
    printMatrix(Inverse);

    return 0;
}