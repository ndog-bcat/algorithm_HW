#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <chrono> // 시간 측정용
#include "my_quick_sorts.h"
using namespace std;
#define N_MAX_RECORDS 1024 * 1024 * 8
RECORD records[N_MAX_RECORDS];
RECORD records1[N_MAX_RECORDS];
RECORD records2[N_MAX_RECORDS];
RECORD records3[N_MAX_RECORDS];
RECORD records4[N_MAX_RECORDS];
RECORD records5[N_MAX_RECORDS];

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

int cmp_arr_members(const RECORD *arr1, const RECORD *arr2, size_t n){
    for (size_t i=0; i<n; i++){
        if (arr1[i].score != arr2[i].score){
            cout << "Different at index " << i << ": " << arr1[i].score << " vs " << arr2[i].score << '\n';
            return 0;
        }
    }
    return 1;
}

int main(){
    // commands 파일 읽기
    const string f_name = "test_commands.txt";
    ifstream read_file;
    read_file.open(f_name);
    if (!read_file.is_open()){
        cerr << "There is no file named "<< f_name <<'\n';
        return -1;
    }
    int case_num;
    size_t num_records;
    string name_in, name_out;
    read_file >> case_num;
    for (int n=1; n<=case_num; n++){
        read_file >> num_records;
        read_file >> name_in;
        read_file >> name_out;

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
        
        // 배열 복사
        memcpy(records1, records, num_records * sizeof(RECORD));
        memcpy(records2, records, num_records * sizeof(RECORD));
        memcpy(records3, records, num_records * sizeof(RECORD));
        memcpy(records4, records, num_records * sizeof(RECORD));
        memcpy(records5, records, num_records * sizeof(RECORD));

        int identical_check = 1;
        int test_times = 10;

        // 정렬
        cout << "\n========== Test Size " << num_records << " ==========\n";
        cout << "========== Test time \t" << test_times << " ==========\n";

        // my_qsort 실행 및 시간 측정
        cout << "USING MY_QSORT \t\t\t\t";
        long long tmp_time = 0;
        for (int i = 0; i < test_times; i++) {
            auto start_time = chrono::high_resolution_clock::now(); // 시간 측정 시작
            my_qsort(records1, num_records, sizeof(RECORD), cmp_rcrd);
            auto end_time = chrono::high_resolution_clock::now(); // 시간 측정 끝
            auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
            tmp_time += duration;
            memcpy(records1, records, num_records * sizeof(RECORD)); // 다시 원래대로 복원
        }
        tmp_time /= test_times; // 평균 시간 계산
        cout << "average execution time: " << tmp_time << " ms" << endl;

        // my_qsort_orig 실행 및 시간 측정
        cout << "USING MY_QSORT_ORIG \t\t\t";
        tmp_time = 0;
        for (int i = 0; i < test_times; i++) {
            auto start_time = chrono::high_resolution_clock::now(); // 시간 측정 시작
            my_qsort_orig(records2, num_records, sizeof(RECORD), cmp_rcrd);
            auto end_time = chrono::high_resolution_clock::now(); // 시간 측정 끝
            auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
            tmp_time += duration;
            memcpy(records2, records, num_records * sizeof(RECORD)); // 다시 원래대로 복원
        }
        tmp_time /= test_times; // 평균 시간 계산
        cout << "average execution time: " << tmp_time << " ms" << endl;
        identical_check *= cmp_arr_members(records1, records2, num_records); // 결과 비교

        // my_qsort_median_insert 실행 및 시간 측정
        cout << "USING MY_QSORT_MEDIAN_INSERT \t\t";
        tmp_time = 0;
        for (int i = 0; i < test_times; i++) {
            auto start_time = chrono::high_resolution_clock::now(); // 시간 측정 시작
            my_qsort_median_insert(records3, num_records, sizeof(RECORD), cmp_rcrd);
            auto end_time = chrono::high_resolution_clock::now(); // 시간 측정 끝
            auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
            tmp_time += duration;
            memcpy(records3, records, num_records * sizeof(RECORD)); // 다시 원래대로 복원
        }
        tmp_time /= test_times; // 평균 시간 계산
        cout << "average execution time: " << tmp_time << " ms" << endl;
        identical_check *= cmp_arr_members(records1, records3, num_records); // 결과 비교

        // my_qsort_median_insert_iter 실행 및 시간 측정
        cout << "USING MY_QSORT_MEDIAN_INSERT_ITER \t";
        tmp_time = 0;
        for (int i = 0; i < test_times; i++) {
            auto start_time = chrono::high_resolution_clock::now(); // 시간 측정 시작
            my_qsort_median_insert_iter(records4, num_records, sizeof(RECORD), cmp_rcrd);
            auto end_time = chrono::high_resolution_clock::now(); // 시간 측정 끝
            auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
            tmp_time += duration;
            memcpy(records4, records, num_records * sizeof(RECORD)); // 다시 원래대로 복원
        }
        tmp_time /= test_times; // 평균 시간 계산
        cout << "average execution time: " << tmp_time << " ms" << endl;
        identical_check *= cmp_arr_members(records1, records4, num_records); // 결과 비교

        // my_qsort_final 실행 및 시간 측정
        cout << "USING_MY_QSORT_FINAL \t\t\t";
        tmp_time = 0;
        for (int i = 0; i < test_times; i++) {
            auto start_time = chrono::high_resolution_clock::now(); // 시간 측정 시작
            my_qsort_final(records5, num_records, sizeof(RECORD), cmp_rcrd);
            auto end_time = chrono::high_resolution_clock::now(); // 시간 측정 끝
            auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
            tmp_time += duration;
            memcpy(records5, records, num_records * sizeof(RECORD)); // 다시 원래대로 복원
        }
        tmp_time /= test_times; // 평균 시간 계산
        cout << "average execution time: " << tmp_time << " ms" << endl;
        identical_check *= cmp_arr_members(records1, records5, num_records); // 결과 비교

        if (identical_check){
            cout << "All sorting methods produced identical results.\n";
        }
        else {
            cout << "There are differences among sorting method results!\n";
		}
    }
    read_file.close();
    return 0;
}