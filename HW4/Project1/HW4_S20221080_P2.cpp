#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
void print_res(vector<vector<int>> arr, vector<int> s, int idx, ofstream& write_file) {
    int start_idx = arr[idx][idx + 1];
    if (start_idx > 0) {
        print_res(arr, s, start_idx - 1, write_file);
    }
    for (int i = start_idx; i <= idx; i++) {
        write_file << s[i];
    }
    write_file << '\n';
    if (start_idx <= 0) {
        return;
    }
}
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
    int n, len, min, min_idx;
    read_file >> n;
    write_file << n << '\n';
    while (n--) {
        read_file >> len;
        vector<int> s(len, 0);
        vector<vector<int>> arr(len, vector<int>(len + 1, 0));
        // arr[i][j]: i번째까지의 문자열에서 j번째부터 회문일 시 그 경우에 해당하는 회문의 최소 개수
        // or j번째부터 i까지는 회문이 아닌 경우 i+1(최대값)
        // arr[i][i+1]: i번째까지 문자열 중 최소 회문을 만들때 i까지의 회문이 시작하는 인덱스 번호
        for (int i = 0; i < len; i++) {
            char c;
            read_file >> c;
            s[i] = c - '0';
        }
        arr[0][0] = 1;
        arr[0][1] = 0;
        for (int i = 1; i < len; i++) {
            arr[i][0] = i + 2; // 불가능한 값으로 초기화
        }
        for (int i = 1; i < len; i++) {
            min = i + 1; // 무조건 i+1보다는 작은 개수를 가지게 됨. i의 idx에서는 0~i까지 전부 회문에 속하지 않는, 전부 다른 숫자라고 했을 때의 개수가 i+1개임.
            min_idx = i; // arr[i][i+1]칸에 넣어놓을 i까지의 최소 회문개수가 되도록 하는 i가 포함될 회문의 시작점
            for (int j = 1; j < i; j++) {
                if (arr[i - 1][j] != i + 1 && s[j - 1] == s[i]) { // i-1행에서 i+1은 불가능한 개수.
                    arr[i][j - 1] = arr[j - 1][j - 1];
                    if (arr[i][j - 1] < min) {
                        min = arr[i][j - 1];
                        min_idx = j - 1;
                    }
                } // 바로 전 원소~j번째까지 회문이라면, j-1번째와 i가 같을 시에는 j-1~i도 회문
                else {
                    arr[i][j-1] = i + 2; // 회문이 아닐 시 불가능한 값으로 초기화
                }
            }
            if (s[i - 1] == s[i]) { // 바로 전 원소와 같다면 i~i-1까지도 회문
                arr[i][i - 1] = arr[i - 1][i - 1];
                if (arr[i][i - 1] < min) {
                    min = arr[i][i - 1];
                    min_idx = i - 1;
                }
            }
            else{
                arr[i][i - 1] = i+2;
            }
            arr[i][i] = arr[i - 1][arr[i - 1][i]] + 1;
            if (arr[i][i] < min) {
                min_idx = i;
            }
            arr[i][i + 1] = min_idx;
        }
        write_file << arr[len - 1][arr[len - 1][len]] << '\n'; // 최소 회문 개수
        int idx = arr[len - 1][len];
        print_res(arr, s, len - 1, write_file);
    }
    read_file.close();
    write_file.close();
    return 0;
}
