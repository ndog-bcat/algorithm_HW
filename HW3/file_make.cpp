#include <cstdio>
#include <cstdlib>
#include <ctime>
#include "my_quick_sorts.h"

int main() {
    srand(20221080); // 난수 고정

    // 테스트 개수 5개
    const int NUM_FILES = 5;

    for (int k = 1; k <= NUM_FILES; k++) {
        size_t num = k * (1 << 20);
        char filename[64];
        sprintf(filename, "unsorted_array%d.bin", k);

        FILE *fp = fopen(filename, "wb");
        for (size_t i = 0; i < num; i++) {
            RECORD rec;
            rec.score = rand() % 1000000000;
            for (int j = 0; j < 12; j++)
                rec.other_data[j] = rand() % 256;
            fwrite(&rec, sizeof(RECORD), 1, fp);
        }

        fclose(fp);
        printf("%s generated (%zu records)\n", filename, num);
    }

    printf("\nAll binary files generated successfully.\n", NUM_FILES);
    return 0;
}
