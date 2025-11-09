#include <iostream>
#include <fstream>
#include <string>
#include "my_quick_sorts.h"
using namespace std;
#define N_MAX_RECORDS 1024 * 1024 * 8
RECORD records[N_MAX_RECORDS];

// RECORD 비교 함수. non-decreasing order
int cmp_rcrd(const void *a, const void *b){
    const RECORD *rec_a = (const RECORD *)a;
    const RECORD *rec_b = (const RECORD *)b;
    
    if (rec_a->score == rec_b->score){
        return 0;
    }
    else if (rec_a->score < rec_b->score){
        return -1;
    }
    else return 1; // 양수일 때 교환해야한다는 뜻. 즉 non-decreasing
};

int main(){
    // commands 파일 읽기
    const string f_name = "HW3_commands.txt";
    ifstream read_file;
    read_file.open(f_name);
    if (!read_file.is_open()){
        cerr << "There is no file named "<< f_name <<'\n';
        return -1;
    }
    int func_num;
    size_t num_records;
    string name_in, name_out;
    read_file >> func_num;
    read_file >> num_records;
    read_file >> name_in;
    read_file >> name_out;
    read_file.close();

    // 입력 파일 읽기
    ifstream read_input(name_in, ios::binary);
    if (!read_input.is_open()){ //그런 파일 없으면 에러구문
        cerr << "There is no file named "<< name_in <<'\n';
        return -1;
    }
    for (size_t i=0; i<num_records; i++){
        read_input.read(reinterpret_cast<char*>(&records[i]), sizeof(RECORD));
    }
    read_input.close();

    // 정렬
    switch (func_num){
        case 1:
            my_qsort(records, num_records, sizeof(RECORD), cmp_rcrd);
            break;
        case 21:
            my_qsort_orig(records, num_records, sizeof(RECORD), cmp_rcrd);
            break;
        case 22:
            my_qsort_median_insert(records, num_records, sizeof(RECORD), cmp_rcrd);
            break;
        case 23:
            my_qsort_median_insert_iter(records, num_records, sizeof(RECORD), cmp_rcrd);
            break;
        case 24:
            my_qsort_final(records, num_records, sizeof(RECORD), cmp_rcrd);
            break;
        default:
            cerr << "Invalid function number: " << func_num << '\n';
            return -1;
    }

    // 출력 파일 쓰기
    ofstream write_output(name_out, ios::binary);
    write_output.write(reinterpret_cast<char*>(records), num_records * sizeof(RECORD));
    write_output.close();
    return 0;
}