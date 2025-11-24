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
    // x, y 문자열 체크배열. 0: gap, 1: mismatch, 2: match
    vector<int> x_check(len_x+1, 0);
    vector<int> y_check(len_y+1, 0);
    for (int i = 1; i <= len_x; i++){
        read_file.read(reinterpret_cast<char*>(&x[i]), sizeof(char));
    }
    for (int i = 1; i <= len_y; i++){
        read_file.read(reinterpret_cast<char*>(&y[i]), sizeof(char));
    }
    read_file.close();
    int score1, score2, score3, temp, one, two, three;
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
                    score1 = mat[i-1][j-1] + s; // x의 i와 y의 j가 일치
                    is_match = true;
                }
                else{
                    score1 = mat[i-1][j-1] - f; // x의 i와 y의 j가 불일치
                    is_match = false;
                }
                score2 = mat[i][j-1] - p; // y에 gap 삽입
                score3 = mat[i-1][j] - p; // x에 gap 삽입
                one = score1;
                two = score2;
                three = score3;
                if (one > two){
                    temp = one;
                    one = two;
                    two = temp;
                }
                if (two > three){
                    temp = two;
                    two = three;
                    three = temp;
                }
                if (one > two){
                    temp = one;
                    one = two;
                    two = temp;
                } // one, two, three non-decreasing 정렬
                if (score1 == three){
                    if (is_match){
                        check[i][j]=3;
                    }
                    else{
                        check[i][j]=2;
                    }
                }
                else if (score2 == three){
                    check[i][j]=1;
                }
                else if (score3 == three){
                    check[i][j]=0;
                }
            }
        }
    }


    ofstream output_f;
    output_f.open(name_out);

    return 0;
}