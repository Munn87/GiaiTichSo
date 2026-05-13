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
    string filename = "test.txt" ; // Đặt tên file của bạn ở đây
    ifstream file(filename);

    if (!file.is_open()) {
        cerr << "Loi: Khong the mo duoc file: " << filename << "'. Kiem tra lai duong dan!\n";
        return 1;
    }

    vector<vector<double>> Aug;
    string line;

    // Đọc từng dòng trong file
    while (getline(file, line)) {
        if (line.empty()) continue; // Bỏ qua nếu có dòng trống

        stringstream ss(line);
        vector<double> row;
        double val;

        // Tách các số cách nhau bởi khoảng trắng trên dòng đó
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

    int m = Aug.size();          // Số hàng = số dòng đọc được
    int totalCols = Aug[0].size(); // Tổng số cột của ma trận mở rộng [A|B]
    
    // Yêu cầu nhập số cột của ma trận B
    int colsB;
    cout << "Ma tran mo rong co tong cong " << totalCols << " cot.\n";
    cout << "Nhap so cot cua ma tran B (vi du p = 1, 2...): ";
    cin >> colsB;

    int colsA = totalCols - colsB; // Tự động suy ra số cột của A

    cout << "=> Ma tran A co kich thuoc " << m << " x " << colsA << ".\n";

    // ==========================================
    // 3. THUẬT TOÁN GAUSS - QUY TRÌNH THUẬN 
    // ==========================================
    vector<int> ind(m, -1);
    int i = 0, j = 0;
    int step = 1;
    
    cout << "--- LICH SU CHON PHAN TU KHOA (PIVOT) ---\n";
    
    // SỬA: j < colsA (chỉ tìm pivot trong phạm vi của A)
    while (i < m && j < colsA) { 
        if (abs(Aug[i][j]) < 1e-9) {
            bool found = false;
            for (int t = i + 1; t < m; ++t) {
                if (abs(Aug[t][j]) > 1e-9) {
                    swap(Aug[i], Aug[t]);
                    found = true;
                    break;
                }
            }
            if (!found) { 
                j++;
                continue; 
            }
        }

        ind[i] = j;
        cout << "Lan lap " << step << ": Chon pivot a[" << i+1 << "][" << j+1 << "] = " 
             << fixed << setprecision(4) << Aug[i][j] << "\n";

        if (i == m - 1) break; 

        // Khử các phần tử bên dưới (Phép trừ hàng vẫn kéo dài đến hết totalCols)
        for (int k = i + 1; k < m; ++k) {
            double factor = Aug[k][j] / Aug[i][j];
            for (int c = j; c < totalCols; ++c) { // SỬA: c chạy tới totalCols để trừ cả phần B
                Aug[k][c] -= factor * Aug[i][c];
            }
        }

        // THÊM ĐOẠN NÀY ĐỂ IN MA TRẬN SAU LẦN LẶP ĐẦU TIÊN
        if (step == 1) {
            cout << "\n--- MA TRAN SAU LAN LAP 1 ---\n";
            printMatrix(Aug);
        }

        // SỬA: Chặn không cho j nhảy sang phần của B
        if (j == colsA - 1) break; 
        
        i++;
        j++;
        step++; 
    }

    cout << "\n--- MA TRAN SAU LAN LAP CUOI (KET THUC QUY TRINH THUAN) ---\n";
    printMatrix(Aug);

    // ==========================================
    // 4. KIỂM TRA VÀ QUY TRÌNH NGHỊCH (HỖ TRỢ VÔ SỐ NGHIỆM)
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
        // Nếu dòng A toàn 0, kiểm tra bên B xem có khác 0 không
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

    // 4.2. XÁC ĐỊNH SỐ ẨN CƠ SỞ & ẨN TỰ DO (TÍNH HẠNG CỦA A)
    vector<bool> isBasic(colsA, false);
    int rank = 0;
    for (int r = 0; r < m; ++r) {
        if (ind[r] != -1 && ind[r] < colsA) {
            isBasic[ind[r]] = true;
            rank++;
        }
    }

    // 4.3. PHÂN NHÁNH KẾT QUẢ VÀ TÌM NGHIỆM
    if (rank == colsA) {
        // TRƯỜNG HỢP 1: NGHIỆM DUY NHẤT (Giữ nguyên logic cũ của bạn)
        cout << "\n============================================\n";
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
        // TRƯỜNG HỢP 2: VÔ SỐ NGHIỆM
        cout << "\n============================================\n";
        cout << " KET LUAN: HE CO VO SO NGHIEM\n";
        cout << "============================================\n";
        
        // Lọc ra danh sách các ẩn tự do
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
        // Mỗi ẩn tự do ứng với 1 vectơ V
        vector<vector<double>> V(freeVars.size(), vector<double>(colsA, 0.0));
        for (size_t i = 0; i < freeVars.size(); ++i) {
            int f = freeVars[i];
            V[i][f] = 1.0; // Đặt ẩn tự do tương ứng bằng 1, các ẩn tự do khác bằng 0
            
            // Thế ngược để tìm các ẩn cơ sở (Với ma trận B coi như = 0)
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

        // BƯỚC B: TÍNH NGHIỆM RIÊNG (X0) CHO TỪNG CỘT CỦA B VÀ IN KẾT QUẢ
        for (int cb = 0; cb < colsB; ++cb) {
            cout << ">>> XET MA TRAN B COT THU " << cb + 1 << ":\n";
            vector<double> X0(colsA, 0.0); // Mặc định các ẩn tự do = 0
            
            // Thế ngược để tìm nghiệm riêng
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

            // IN RA MÀN HÌNH THEO ĐỊNH DẠNG CỘT DỄ NHÌN
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