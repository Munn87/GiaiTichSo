
#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>

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

int main() {
    // 1. ĐỌC FILE DỮ LIỆU
    string filename = "de2GK20242gts.txt"; // Đặt tên file của bạn ở đây
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Loi: Khong the mo duoc file: " << filename << "'. Kiem tra lai duong dan!\n";
        return 1;
    }

    vector<vector<double>> Aug;
    string line;

    // Đọc từng dòng trong file
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

    // 2. LẤY KÍCH THƯỚC TỰ ĐỘNG & CHIA TÁCH [A|B]
    if (Aug.empty()) {
        cerr << "Loi: File rong hoac khong chua du lieu hop le!\n";
        return 1;
    }

    int m = Aug.size();          
    int totalCols = Aug[0].size(); 
    
    int colsB;
    cout << "Ma tran mo rong co tong cong " << totalCols << " cot.\n";
    cout << "Nhap so cot cua ma tran B (vi du p = 1, 2...): ";
    cin >> colsB;

    int colsA = totalCols - colsB; 

    cout << "=> Ma tran A co kich thuoc " << m << " x " << colsA << ".\n";

    // ==========================================
    // 3. THUẬT TOÁN GAUSS-JORDAN (KHỬ TOÀN DIỆN)
    // ==========================================
    vector<int> ind(m, -1); // Lưu vị trí cột của pivot tại hàng r
    vector<bool> row_used(m, false);
    vector<bool> col_used(colsA, false);
    int step = 1;
    
    cout << "--- LICH SU CHON PHAN TU KHOA (PIVOT) ---\n";
    
    // Lặp tối đa min(m, colsA) bước
    for (int k = 0; k < min(m, colsA); ++k) {
        double max_val = 0;
        int p = -1, q = -1;

        // BƯỚC 3.1: Tìm pivot theo ĐÚNG 2 Ưu tiên của slide
        bool found_priority_1 = false;
        double target_values[] = {1.0, -1.0}; // Các số ưu tiên 1
        
        // --- Thử Ưu tiên 1 ---
        for (double val : target_values) {
            for (int r = 0; r < m; ++r) {
                if (!row_used[r]) {
                    for (int c = 0; c < colsA; ++c) {
                        if (!col_used[c]) {
                            // Kiểm tra xem trị tuyệt đối có xấp xỉ các số 1, -1 không
                            if (abs(abs(Aug[r][c]) - val) < 1e-9) {
                                p = r;
                                q = c;
                                max_val = abs(Aug[r][c]);
                                found_priority_1 = true;
                                break;
                            }
                        }
                    }
                }
                if (found_priority_1) break;
            }
            if (found_priority_1) break;
        }

        // --- Nếu không thỏa Ưu tiên 1, dùng Ưu tiên 2 ---
        if (!found_priority_1) {
            for (int r = 0; r < m; ++r) {
                if (!row_used[r]) {
                    for (int c = 0; c < colsA; ++c) {
                        if (!col_used[c]) {
                            if (abs(Aug[r][c]) > max_val) {
                                max_val = abs(Aug[r][c]);
                                p = r;
                                q = c;
                            }
                        }
                    }
                }
            }
        }

        // Nếu phần tử lớn nhất bằng 0 -> ma trận còn lại toàn số 0 -> Dừng khử
        if (max_val < 1e-9) break;

        row_used[p] = true;
        col_used[q] = true;
        ind[p] = q; // Ghi nhận: Hàng p có pivot ở cột q

        cout << "Lan lap " << step << ": Chon pivot a[" << p+1 << "][" << q+1 << "] = " 
             << fixed << setprecision(4) << Aug[p][q] << "\n";

        // BƯỚC 3.2: Chuẩn hóa hàng chứa pivot (Chia cả hàng cho giá trị pivot)
        double pivot_val = Aug[p][q];
        for (int c = 0; c < totalCols; ++c) {
            Aug[p][c] /= pivot_val;
        }

        // BƯỚC 3.3: Khử các phần tử CẢ BÊN TRÊN VÀ BÊN DƯỚI
        for (int r = 0; r < m; ++r) {
            if (r != p) { // Khác hàng đang chứa pivot
                double factor = Aug[r][q];
                for (int c = 0; c < totalCols; ++c) {
                    Aug[r][c] -= factor * Aug[p][c];
                }
            }
        }

        if (step == 1) {
            cout << "\n--- MA TRAN SAU LAN LAP 1 ---\n";
            printMatrix(Aug);
        }
        step++; 
    }

    cout << "\n--- MA TRAN KET THUC KHOA GAUSS-JORDAN (RREF) ---\n";
    printMatrix(Aug);

    // ==========================================
    // 4. KIỂM TRA NGHIỆM TỪ MA TRẬN RÚT GỌN
    // ==========================================
    
    // 4.1. KIỂM TRA HỆ VÔ NGHIỆM
    bool isConsistent = true;
    for (int r = 0; r < m; ++r) {
        bool allZeroInA = true;
        for (int c = 0; c < colsA; ++c) {
            if (abs(Aug[r][c]) > 1e-9) { 
                allZeroInA = false; 
                break; 
            }
        }
        if (allZeroInA) {
            for (int cb = 0; cb < colsB; ++cb) {
                if (abs(Aug[r][colsA + cb]) > 1e-9) {
                    isConsistent = false; 
                    break;
                }
            }
        }
        if (!isConsistent) break;
    }

    if (!isConsistent) {
        cout << "\n============================================\n";
        cout << " KET LUAN: HE PHUONG TRINH VO NGHIEM!\n";
        cout << "============================================\n";
        return 0;
    }

    // 4.2. XÁC ĐỊNH SỐ ẨN CƠ SỞ & ẨN TỰ DO
    vector<bool> isBasic(colsA, false);
    int rank = 0;
    for (int r = 0; r < m; ++r) {
        if (ind[r] != -1 && ind[r] < colsA) {
            isBasic[ind[r]] = true;
            rank++;
        }
    }

    // 4.3. PHÂN NHÁNH KẾT QUẢ VÀ TÌM NGHIỆM (SIÊU NHANH NHỜ GAUSS-JORDAN)
    if (rank == colsA) {
        // TRƯỜNG HỢP 1: NGHIỆM DUY NHẤT
        cout << "\n============================================\n";
        cout << " KET LUAN: HE CO NGHIEM DUY NHAT\n";
        cout << "============================================\n";
        
        vector<vector<double>> X(colsA, vector<double>(colsB, 0.0));
        
        // VÌ LÀ GAUSS-JORDAN -> KHÔNG CẦN VÒNG LẶP THẾ NGƯỢC NỮA!
        // Nghiệm nằm thẳng ở cột B ứng với phương trình của biến cơ sở.
        for (int r = 0; r < m; ++r) {
            int pivot_col = ind[r];
            if (pivot_col != -1 && pivot_col < colsA) {
                for (int cb = 0; cb < colsB; ++cb) {
                    X[pivot_col][cb] = Aug[r][colsA + cb];
                }
            }
        }
        cout << "--- MA TRAN NGHIEM X ---\n";
        printMatrix(X);

    } else {
        // TRƯỜNG HỢP 2: VÔ SỐ NGHIỆM
        cout << "\n============================================\n";
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

        // BƯỚC A: TÍNH CÁC VECTƠ CƠ SỞ (V_i) CỦA KHÔNG GIAN NGHIỆM THUẦN NHẤT
        vector<vector<double>> V(freeVars.size(), vector<double>(colsA, 0.0));
        for (size_t i = 0; i < freeVars.size(); ++i) {
            int f = freeVars[i];
            V[i][f] = 1.0; 
            
            // Không cần thế ngược. Giá trị của ẩn cơ sở chính là âm hệ số của ẩn tự do
            for (int r = 0; r < m; ++r) {
                int p_col = ind[r];
                if (p_col != -1 && p_col < colsA) {
                    V[i][p_col] = -Aug[r][f];
                }
            }
        }

        // BƯỚC B: TÍNH NGHIỆM RIÊNG (X0) CHO TỪNG CỘT CỦA B
        for (int cb = 0; cb < colsB; ++cb) {
            cout << ">>> XET MA TRAN B COT THU " << cb + 1 << ":\n";
            vector<double> X0(colsA, 0.0); 
            
            // Không cần thế ngược. Giá trị của ẩn cơ sở chính là cột tự do
            for (int r = 0; r < m; ++r) {
                int p_col = ind[r];
                if (p_col != -1 && p_col < colsA) {
                    X0[p_col] = Aug[r][colsA + cb];
                }
            }

            cout << "Nghiem tong quat co dang: X = X0";
            for (size_t i = 0; i < freeVars.size(); ++i) {
                cout << " + t" << i + 1 << "*V" << i + 1;
            }
            cout << "\n\nBang toa do cac vector:\n";
            
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

    return 0;
}