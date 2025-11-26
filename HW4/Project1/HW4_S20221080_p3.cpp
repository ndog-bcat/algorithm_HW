#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
int arr[1025];
int main() {
    const string name_in = "input.txt";
    const string name_out = "output.txt";

    ifstream read_file;
    read_file.open(name_in);
    if (!read_file.is_open()) {
        cout << "There is no file named " << name_in << '\n';
        return 1;
    }
    ofstream write_file;
    write_file.open(name_out);
    if (!write_file.is_open()) {
        cout << "There is no file named " << name_out << '\n';
        return 1;
    }

    int t, n, sum, W, tmp;
    read_file >> t;
    write_file << t << '\n';
    while (t--){
        read_file >> n;
        sum = 0;
        for (int i=1; i<=n; i++){
            read_file >> arr[i];
            sum += arr[i];
        }
        vector<int> check_s(n+1,0);
        W = sum/2; // 원소의 총 합인 S가 짝수일 경우에는 S/2, 홀수일 경우에는 (S-1)/2가 됨
        vector<vector<int>> p(n+1, vector<int> (W+1,0));
        vector<vector<int>> check(n+1, vector<int> (W+1,0));
        for (int ww=0; ww<=W; ww++){
            p[0][ww] = 0;
        }
        for (int i=1; i<=n; i++){
            p[i][0] = 0;
            for (int ww=1; ww<=W; ww++){
                if (arr[i] <= ww){
                    if ((tmp = arr[i] + p[i-1][ww-arr[i]]) > p[i-1][ww]){
                        p[i][ww] = tmp;
                        check[i][ww] = ww-arr[i];
                    }
                    else{
                        p[i][ww] = p[i-1][ww];
                        check[i][ww] = ww;
                    }
                }
                else{
                    p[i][ww] = p[i-1][ww];
                    check[i][ww] = ww;
                }
            }
        }
        int w_tmp = W;
        int idx_tmp = n;
        int a1_cnt = 0;
        while (n>=1){
            if (check[idx_tmp][w_tmp] != w_tmp){
                check_s[idx_tmp] = 1;
                w_tmp = check[idx_tmp][w_tmp];
                a1_cnt++;
            }
            idx_tmp--;
        }
        if (check_s[1] == 0){
            a1_cnt = n-a1_cnt;
        }
        write_file << sum-p[n][W] << ' ' << n << ' ' << a1_cnt << '\n';
        for (int i=1; i<=n; i++){
            if (check_s[i]==0){
                write_file << arr[i] << '\n';
            }
        }
        for (int i=1; i<=n; i++){
            if (check_s[i]==1){
                write_file << arr[i] << '\n';
            }
        }
    }
    read_file.close();
    write_file.close();
    return 0;
}
