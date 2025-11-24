#include <cstdlib>
#include <cstring>
#include "my_quick_sorts.h"

#define INSERTION_SORT_THRESHOLD 20

void swap(void *a, void *b, size_t size, char *temp){
    memcpy(temp, a, size);
    memcpy(a, b, size);
    memcpy(b, temp, size);
}

// orig 파티션 함수
char *partition_orig(void *base, size_t num, size_t size, _Cmpfun *cmp, char *temp){
    char *pivot, *left, *right;
    pivot = (char*)base;
    left = (char*)base + size;
    right = (char*)base + (num-1)*size;
    while (1){
        while (left <= right && cmp(left, pivot) < 0){
            left += size;
        }
        while (left <= right && cmp(right, pivot) > 0){
            right -= size;
        }
        if (left >= right){
            break;
        }
        swap(left, right, size, temp);
        left += size;
        right -= size;
    }
    swap(pivot, right, size, temp);
    return right;
}

// median 파티션 함수
char *partition_median(void *base, size_t num, size_t size, _Cmpfun *cmp, char *temp)
{
    char *pivot, *left, *right, *mid, *big;
    pivot = (char*)base;
    mid = (char*)base + (num/2)*size;
    big = (char*)base + (num-1)*size;

    // pivot, mid, big을 non-decreasing order로 정렬
    if (cmp(pivot, mid) > 0) swap(pivot, mid, size, temp);
    if (cmp(pivot, big) > 0) swap(pivot, big, size, temp);
    if (cmp(mid, big) > 0) swap(mid, big, size, temp);
    // median을 pivot으로 설정
    swap(mid, pivot, size, temp);
    
    left = (char*)base + size;
    right = (char*)base + (num-1)*size;
    while (1){
        while (left <= right && cmp(left, pivot) < 0){
            left += size;
        }
        while (left <= right && cmp(right, pivot) > 0){
            right -= size;
        }
        if (left >= right){
            break;
        }
        swap(left, right, size, temp);
        left += size;
        right -= size;
    }
    swap(pivot, right, size, temp);
    return right;
}

void my_qsort(void *base, size_t num, size_t size, _Cmpfun *cmp)
{
    qsort(base, num, size, cmp);
}

void my_qsort_orig_cal(void *base, size_t num, size_t size, _Cmpfun *cmp, char *temp)
{
    if (num <= 1) return;
    char *pivot = partition_orig(base, num, size, cmp, temp);
    size_t left_size = (pivot - (char*)base) / size;
    size_t right_size = num - left_size - 1;
    my_qsort_orig_cal(base, left_size, size, cmp, temp);
    my_qsort_orig_cal(pivot+size, right_size, size, cmp, temp);
}

void my_qsort_orig(void *base, size_t num, size_t size, _Cmpfun *cmp){
    char *temp;
    temp = (char*)malloc(size);
    my_qsort_orig_cal(base, num, size, cmp, temp);
    free(temp);
}

void my_qsort_median_insert_cal(void *base, size_t num, size_t size, _Cmpfun *cmp, void *key, char *temp){
    if (num <= INSERTION_SORT_THRESHOLD){
        char *a = (char*)base;
        size_t j;
        for (j = 1; j < num; j++){
            memcpy(key, a + j*size, size);
            long long i = j - 1;
            while (i>=0 && cmp(a + i*size, key) > 0){
                memcpy(a + (i+1)*size, a + i*size, size);
                i--;
            }
            memcpy(a+(i+1)*size, key, size);
        }
        return;
    }
    else{
        char *pivot = partition_median(base, num, size, cmp, temp);
        size_t left_size = (pivot - (char*)base) / size;
        size_t right_size = num - left_size - 1;
        my_qsort_median_insert_cal(base, left_size, size, cmp, key, temp);
        my_qsort_median_insert_cal(pivot+size, right_size, size, cmp, key, temp);
    }
}

void my_qsort_median_insert(void *base, size_t num, size_t size, _Cmpfun *cmp)
{
    void *key;
    char *temp;
    key = (char*)malloc(size);
    temp = (char*)malloc(size);
    my_qsort_median_insert_cal(base, num, size, cmp, key, temp);
    free(key);
    free(temp);
}

void my_qsort_median_insert_iter_cal(void *base, size_t num, size_t size, _Cmpfun *cmp, void *key, char *temp)
{
    while (num > 1){
        if (num <= INSERTION_SORT_THRESHOLD){
            char *a = (char*)base;
            size_t j;
            for (j = 1; j < num; j++){
                memcpy(key, a + j*size, size);
                long long i = j - 1;
                while (i>=0 && cmp(a + i*size, key) > 0){
                    memcpy(a + (i+1)*size, a + i*size, size);
                    i--;
                }
                memcpy(a+(i+1)*size, key, size);
            }
            return;
        }
        char *pivot = partition_median(base, num, size, cmp, temp);
        size_t left_size = (pivot - (char*)base) / size;
        size_t right_size = num - left_size - 1;
        if (left_size < right_size){
            my_qsort_median_insert_iter_cal(base, left_size, size, cmp, key, temp);
            base = pivot + size;
            num = right_size;
        }
        else{
            my_qsort_median_insert_iter_cal(pivot+size, right_size, size, cmp, key, temp);
            num = left_size;
        }
    }   
}

void my_qsort_median_insert_iter(void *base, size_t num, size_t size, _Cmpfun *cmp){
    void *key;
    char *temp;
    key = (char*)malloc(size);
    temp = (char*)malloc(size);
    my_qsort_median_insert_iter_cal(base, num, size, cmp, key, temp);
    free(key);
    free(temp);
}

// final함수는 score기준으로 인덱스 배열을 정렬하기 때문에,
// 다른 함수들과 달리 파티션함수와 정렬함수에 함수 인자에 인덱스 배열이 들어감

size_t *partition_final(void *base, size_t *idxs, size_t num, size_t size, _Cmpfun *cmp) // 인덱스 배열은 스왑함수 안쓰고 직접 스왑함
{
    size_t *pivot, *mid, *big, *left, *right, temp;
    pivot = idxs;
    mid = idxs + (num/2);
    big = idxs + (num-1);

    // pivot, mid, big을 non-decreasing order로 정렬
    if (cmp((char*)base+(*pivot)*size, (char*)base+(*mid)*size) > 0){
        temp = *pivot;
        *pivot = *mid;
        *mid = temp;
    }
    if (cmp((char*)base+(*pivot)*size, (char*)base+(*big)*size) > 0){
        temp = *pivot;
        *pivot = *big;
        *big = temp;
    }
    if (cmp((char*)base+(*mid)*size, (char*)base+(*big)*size) > 0){
        temp = *mid;
        *mid = *big;
        *big = temp;
    }
    // median을 pivot으로 설정
    temp = *mid;
    *mid = *pivot;
    *pivot = temp;
    
    left = idxs + 1;
    right = idxs + (num-1);
    while (1){
        while (left <= right && cmp((char*)base+(*left)*size, (char*)base+(*pivot)*size) < 0){
            left++;
        }
        while (left <= right && cmp((char*)base+(*right)*size, (char*)base+(*pivot)*size) > 0){
            right--;
        }
        if (left >= right){
            break;
        }
        temp = *left;
        *left = *right;
        *right = temp;
        left++;
        right--;
    }
    temp = *pivot;
    *pivot = *right;
    *right = temp;
    return right;
}

void my_qsort_final_cal(void *base, size_t *idxs, size_t num, size_t size, _Cmpfun *cmp){
    if (num <= INSERTION_SORT_THRESHOLD){
        for (size_t j = 1; j < num; j++){
            size_t key = idxs[j];
            long long i = j - 1; // -1 했을때 underflow 일어날 수 있으므로 long long 사용
            while (i>=0 && cmp((char*)base + idxs[i]*size, (char*)base + key*size) > 0){
                idxs[i+1] = idxs[i];
                i--;
            }
            idxs[i+1] = key;
        }
        return;
    }
    else{
        size_t *pivot_ptr = partition_final(base, idxs, num, size, cmp);
        size_t left_size = (size_t)(pivot_ptr - idxs);
        size_t right_size = num - left_size - 1;
        my_qsort_final_cal(base, idxs, left_size, size, cmp);
        my_qsort_final_cal(base, pivot_ptr + 1, right_size, size, cmp);
    }
}

void my_qsort_final(void *base, size_t num, size_t size, _Cmpfun *cmp){
    // 인덱스 배열 생성. 출력해보니까 8바이트라서 확실히 레코드보다 정렬이 빠를것으로 생각된다
    size_t *idxs = (size_t*)malloc(num * sizeof(size_t));
    for (size_t i = 0; i < num; i++){
        idxs[i] = i;
    }
    // 레코드 score 기준으로 인덱스 배열 정렬
    my_qsort_final_cal(base, idxs, num, size, cmp);

    // 인덱스 배열 기반으로 레코드 배열 정렬
    char *temp = (char*)malloc(size * num);
    for (size_t i = 0; i < num; i++){
        memcpy(temp + i*size, (char*)base + idxs[i]*size, size);
    }
    memcpy(base, temp, size * num);
    free(temp);
    free(idxs);
}
