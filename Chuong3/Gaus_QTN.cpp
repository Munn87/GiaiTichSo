#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <sstream>

using namespace std;

// Hàm in ma trận
void printMatrix(const vector<vector<double>>& mat) {
    for (const auto& row : mat) {
        for (double val : row) {
            cout << setw(10) << fixed << setprecision(4) << (abs(val) < 1e-9 ? 0.0 : val) << " ";
        }
        cout << "\n";
    }
    cout << "\n";
}

// ==========================================
// HÀM: QUY TRÌNH NGHỊCH ĐỘC LẬP (Cho ma trận đã ở dạng bậc thang)
// ==========================================
void quyTrinhNghichDocLap(const vector<vector<double>>& Aug, int m, int colsA, int colsB) {
    // 1. TỰ ĐỘNG QUÉT TÌM VỊ TRÍ PIVOT (Do không chạy QTT nên phải tự tìm)
    vector<int> ind(m, -1);
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < colsA; ++j) {
            if (abs(Aug[i][j]) > 1e-9) { // Tìm phần tử khác 0 đầu tiên trong hàng
                ind[i] = j;
                break;
            }
        }
    }

    // 2. KIỂM TRA HỆ VÔ NGHIỆM
    bool isConsistent = true;
    for (int r = 0; r < m; ++r) {
        if (ind[r] == -1) { // Nếu hàng trong ma trận A toàn 0
            for (int cb = 0; cb < colsB; ++cb) {
                if (abs(Aug[r][colsA + cb]) > 1e-9) { // Mà bên B lại khác 0
                    isConsistent = false; 
                    break;
                }
            }
        }
        if (!isConsistent) break;
    }

    if (!isConsistent) {
        cout << "============================================\n";
        cout << " KET LUAN: HE PHUONG TRINH VO NGHIEM!\n";
        cout << "============================================\n";
        return;
    }

    // 3. TÍNH HẠNG VÀ PHÂN LOẠI NGHIỆM
    vector<bool> isBasic(colsA, false);
    int rank = 0;
    for (int r = 0; r < m; ++r) {
        if (ind[r] != -1 && ind[r] < colsA) {
            isBasic[ind[r]] = true;
            rank++;
        }
    }

    // 4. GIẢI VÀ IN KẾT QUẢ THEO PHÂN NHÁNH
    if (rank == colsA) {
        cout << "============================================\n";
        cout << " KET LUAN: HE CO NGHIEM DUY NHAT\n";
        cout << "============================================\n";
        
        vector<vector<double>> X(colsA, vector<double>(colsB, 0.0));
        for (int r = m - 1; r >= 0; --r) {
            int pivot_col = ind[r];
            if (pivot_col != -1 && pivot_col < colsA) {
                for (int cb = 0; cb < colsB; ++cb) {
                    double sum = 0;
                    for (int k = pivot_col + 1; k < colsA; ++k) {
                        sum += Aug[r][k] * X[k][cb];
                    }
                    X[pivot_col][cb] = (Aug[r][colsA + cb] - sum) / Aug[r][pivot_col];
                }
            }
        }
        cout << "--- MA TRAN NGHIEM X ---\n";
        printMatrix(X);

    } else {
        cout << "============================================\n";
        cout << " KET LUAN: HE CO VO SO NGHIEM\n";
        cout << "============================================\n";
        
        vector<int> freeVars;
        for (int j = 0; j < colsA; ++j) {
            if (!isBasic[j]) freeVars.push_back(j);
        }
        
        cout << "So an tu do: " << freeVars.size() << " (Gom cac an: ";
        for (size_t i = 0; i < freeVars.size(); ++i) {
            cout << "x" << freeVars[i] + 1 << (i == freeVars.size() - 1 ? "" : ", ");
        }
        cout << ")\n\n";

        // VECTƠ CƠ SỞ
        vector<vector<double>> V(freeVars.size(), vector<double>(colsA, 0.0));
        for (size_t i = 0; i < freeVars.size(); ++i) {
            int f = freeVars[i];
            V[i][f] = 1.0; 
            for (int r = m - 1; r >= 0; --r) {
                int p_col = ind[r];
                if (p_col != -1 && p_col < colsA) {
                    double sum = 0;
                    for (int k = p_col + 1; k < colsA; ++k) {
                        sum += Aug[r][k] * V[i][k];
                    }
                    V[i][p_col] = -sum / Aug[r][p_col];
                }
            }
        }

        // NGHIỆM RIÊNG
        for (int cb = 0; cb < colsB; ++cb) {
            cout << ">>> XET MA TRAN B COT THU " << cb + 1 << ":\n";
            vector<double> X0(colsA, 0.0); 
            
            for (int r = m - 1; r >= 0; --r) {
                int p_col = ind[r];
                if (p_col != -1 && p_col < colsA) {
                    double sum = 0;
                    for (int k = p_col + 1; k < colsA; ++k) {
                        sum += Aug[r][k] * X0[k];
                    }
                    X0[p_col] = (Aug[r][colsA + cb] - sum) / Aug[r][p_col];
                }
            }

            cout << "Bang toa do cac vector:\n";
            cout << "An       X0(Rieng)";
            for (size_t i = 0; i < freeVars.size(); ++i) {
                cout << "     V" << i + 1 << "(t" << i + 1 << ")";
            }
            cout << "\n-------------------------------------------------\n";

            for (int j = 0; j < colsA; ++j) {
                cout << "x" << j+1 << setw(15) << fixed << setprecision(4) << (abs(X0[j]) < 1e-9 ? 0.0 : X0[j]);
                for (size_t i = 0; i < freeVars.size(); ++i) {
                    cout << setw(13) << fixed << setprecision(4) << (abs(V[i][j]) < 1e-9 ? 0.0 : V[i][j]);
                }
                cout << "\n";
            }
            cout << "-------------------------------------------------\n\n";
        }
    }
}

// ==========================================
// HÀM MAIN
// ==========================================
int main() {
    string filename = "test.txt";
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Loi: Khong the mo duoc file!\n";
        return 1;
    }

    vector<vector<double>> Aug;
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue; 
        stringstream ss(line);
        vector<double> row;
        double val;
        while (ss >> val) {
            row.push_back(val);
        }
        if (!row.empty()) {
            Aug.push_back(row);
        }
    }
    file.close();

    if (Aug.empty()) {
        cerr << "Loi: File rong!\n";
        return 1;
    }

    int m = Aug.size();          
    int totalCols = Aug[0].size(); 
    
    int colsB = -1;
    cout << "Ma tran doc duoc co " << totalCols << " cot.\n";
    while (colsB < 1 || colsB >= totalCols) {
        cout << "Nhap so cot cua ma tran B (tu 1 den " << totalCols - 1 << "): ";
        cin >> colsB;
    }

    int colsA = totalCols - colsB; 
    
    cout << "\n--- MA TRAN DAU VAO ---\n";
    printMatrix(Aug);

    // Chạy thẳng Quy trình nghịch
    quyTrinhNghichDocLap(Aug, m, colsA, colsB);

    return 0;
}