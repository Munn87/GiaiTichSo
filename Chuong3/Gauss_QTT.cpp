#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>
#include <fstream>
#include <sstream>

using namespace std;

// ==========================================
// HÀM TIỆN ÍCH: IN MA TRẬN
// ==========================================
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
// HÀM: CHỈ CHẠY QUY TRÌNH THUẬN 
// ==========================================
void quyTrinhThuan(vector<vector<double>>& A, int m, int n) {
    // B1: Khởi tạo i=0; j=0; ind (chuyển sang 0-based index)
    int i = 0, j = 0;
    vector<int> ind(m, -1); // Khởi tạo -1 nghĩa là chưa có pivot
    int step = 1;
    
    cout << "\n--- LICH SU CHON PHAN TU KHOA (PIVOT) THEO SLIDE ---\n";
    
    // Vòng lặp này bao trọn B3, B5, B8 (Điều kiện dừng)
    while (i < m && j < n) { 
        
        // B2: Kiểm tra nếu a_ij != 0
        if (abs(A[i][j]) > 1e-9) { 
            ind[i] = j; // ind[i] = j => Chuyển sang B3/B4
            
            cout << "Lan lap " << step << ": Chon pivot a[" << i+1 << "][" << j+1 << "] = " 
                 << fixed << setprecision(4) << A[i][j] << "\n";

            // B4: Cho k chạy từ i+1 đến m, thực hiện biến đổi khử
            for (int k = i + 1; k < m; ++k) {
                double factor = A[k][j] / A[i][j];
                for (int c = j; c < n; ++c) { 
                    A[k][c] -= factor * A[i][c];
                }
            }

            if (step == 1) {
                cout << "\n--- MA TRAN SAU LAN LAP 1 ---\n";
                printMatrix(A);
            }

            // B5: Tăng i, j (do vòng lặp while đã lo việc check j=n nên ta cứ tăng)
            i++;
            j++;
            step++; 

        } else {
            // Trái lại thì sang B6: Cho t = i + 1
            bool found_nonzero = false;
            
            for (int t = i + 1; t < m; ++t) {
                // B7: Kiểm tra nếu a_tj != 0 thì đổi chỗ 2 hàng t và i
                if (abs(A[t][j]) > 1e-9) {
                    cout << "[!] Phat hien a["<<i+1<<"]["<<j+1<<"] = 0. Doi cho hang " << i+1 << " va hang " << t+1 << "\n";
                    swap(A[i], A[t]);
                    
                    // ind[i] = j và quay lại B3 (break vòng for t, quay lại vòng while để nó tự đi vào nhánh B2->B4)
                    found_nonzero = true;
                    break;
                }
            }
            
            // B8: Nếu quét hết ở dưới t < m mà không thấy ai != 0 (tức là cột toàn số 0)
            if (!found_nonzero) {
                // Tăng cột j = j + 1 và tiếp tục (tương đương quay lại B2)
                j++; 
            }
        }
    }

    // --- IN KẾT QUẢ ĐỂ KIỂM CHỨNG ---
    cout << "\n--- MANG LUU VI TRI PIVOT (Mang ind) ---\n";
    cout << "ind = [ ";
    for (int r = 0; r < m; ++r) {
        // In theo index 1-based cho giống toán học, nếu -1 thì in 0 (không có)
        cout << (ind[r] != -1 ? ind[r] + 1 : 0) << " ";
    }
    cout << "]\n";

    cout << "\n--- MA TRAN BAC THANG (SAU QUY TRINH THUAN) ---\n";
    printMatrix(A);
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

    vector<vector<double>> A;
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
            A.push_back(row);
        }
    }
    file.close();

    if (A.empty()) {
        cerr << "Loi: File rong!\n";
        return 1;
    }

    int m = A.size();          // Số hàng
    int n = A[0].size();       // Số cột
    
    cout << "=> Ma tran doc duoc co kich thuoc " << m << " x " << n << ".\n";

    // Chỉ gọi duy nhất 1 hàm
    quyTrinhThuan(A, m, n);

    return 0;
}