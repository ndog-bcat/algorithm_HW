#pragma once
#pragma warning(disable:4996)

typedef struct {
    unsigned int score;
    char other_data[12];
} RECORD;

typedef int _Cmpfun(const void *, const void *);

void my_qsort(void *, size_t, size_t, _Cmpfun *);
void my_qsort_orig(void *, size_t, size_t, _Cmpfun *);
void my_qsort_median_insert(void *, size_t, size_t, _Cmpfun *);
void my_qsort_median_insert_iter(void *, size_t, size_t, _Cmpfun *);
void my_qsort_final(void *, size_t, size_t, _Cmpfun *);