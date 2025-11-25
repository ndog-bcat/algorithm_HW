#include <iostream>
#include <fstream>
#include <string>
#include <vector>
using namespace std;
int main(){
    const string name_in = "input.txt";
    const string name_out = "output.txt";

    ifstream read_file; // binary 파일명 및 인자 입력받음
    read_file.open(name_in);
    if (!read_file.is_open()){
        cout << "There is no file named " << name_in << '\n';
        return 1;
    }
    string string_file;
    int s, f, p; // match: +s, mismatch: -f, gap: -p
    read_file >> string_file >> s >> f >> p;
    read_file.close();

    read_file.open(string_file, ios::binary); // twostrings 입력 시작
    if (!read_file.is_open()){
        cout << "There is no file named " << string_file << '\n';
        return 1;
    }
    int len_x, len_y;
    read_file.read(reinterpret_cast<char*>(&len_x), sizeof(int));
    read_file.read(reinterpret_cast<char*>(&len_y), sizeof(int));
    // x, y 문자열
    vector<char> x(len_x+1);
    vector<char> y(len_y+1);
    // x, y 문자열 체크배열. gap: 1: mismatch: 2: match: 3
    vector<int> x_check(len_x+1, 0);
    vector<int> y_check(len_y+1, 0);
    for (int i = 1; i <= len_x; i++){
        read_file.read(reinterpret_cast<char*>(&x[i]), sizeof(char));
    }
    for (int i = 1; i <= len_y; i++){
        read_file.read(reinterpret_cast<char*>(&y[i]), sizeof(char));
    }
    read_file.close();

    int iandj, ygap, xgap, best;
    bool is_match;
    // 계산 과정
    vector<vector<int>> mat(len_x+1, vector<int>(len_y+1,0));
    vector<vector<int>> check(len_x+1, vector<int>(len_y+1,0));
    // check --> gap_x: 0, gap_y: 1, mismatch: 2, match: 3
    for (int i=0; i<=len_x; i++){
        for (int j=0; j<=len_y; j++){
            if (i==0){
                mat[i][j] = -p*j;
            }
            else if (j==0){
                mat[i][j] = -p*i;
            }
            else{
                if (x[i]==y[j]){
                    iandj = mat[i-1][j-1] + s; // x의 i와 y의 j가 일치
                    is_match = true;
                }
                else{
                    iandj = mat[i-1][j-1] - f; // x의 i와 y의 j가 불일치
                    is_match = false;
                }
                ygap = mat[i][j-1] - p; // y에 gap 삽입
                xgap = mat[i-1][j] - p; // x에 gap 삽입
                
                best = iandj;
                if (is_match){
                    check[i][j] = 3; // match
                }
                else{
                    check[i][j] = 2; // mismatch
                }
                if (ygap > best){
                    best = ygap;
                    check[i][j] = 1;
                }
                if (xgap > best){
                    best = xgap;
                    check[i][j] = 0;
                }
                mat[i][j] = best; // 최댓값 저장
            }
        }
    }
    int idx_i = len_x;
    int idx_j = len_y;
    int gap_count_x = 0;
    int gap_count_y = 0;
    int string_len = 0;
    while (idx_i > 0 && idx_j > 0){
        if (check[idx_i][idx_j] == 3 || check[idx_i][idx_j] == 2){ // match or mismatch
            x_check[idx_i] = check[idx_i][idx_j];
            y_check[idx_j] = check[idx_i][idx_j];
            idx_i--;
            idx_j--;
        }
        else if (check[idx_i][idx_j] == 1){ // gap in y
            y_check[idx_j] = 1;
            idx_j--;
            gap_count_y++;
        }
        else if (check[idx_i][idx_j] == 0){ // gap in x
            x_check[idx_i] = 1;
            idx_i--;
            gap_count_x++;
        }
        string_len++;
    }
    while (idx_i > 0){
        x_check[idx_i] = 1;
        idx_i--;
        gap_count_x++;
        string_len++;
    }
    while (idx_j > 0){
        y_check[idx_j] = 1;
        idx_j--;
        gap_count_y++;
        string_len++;
    }
    ofstream output_f;
    output_f.open(name_out);
    if (!output_f.is_open()){
        cout << "There is no file named " << name_out << '\n';
        return 1;
    }
    output_f << mat[len_x][len_y] << '\n'; // 최대 점수
    output_f << string_len << '\n'; // 정렬된 문자열 길이
    output_f << gap_count_x << '\n'; // x의 gap 개수
    for (int i=1; i<=len_x; i++){
        if (x_check[i] == 1){
            output_f << i << '\n';
        }
    } // x의 gap 위치
    output_f << gap_count_y << '\n'; // y의 gap 개수
    for (int i=1; i<=len_y; i++){
        if (y_check[i] == 1){
            output_f << i << '\n';
        }
    } // y의 gap 위치
    output_f.close();
    return 0;
}