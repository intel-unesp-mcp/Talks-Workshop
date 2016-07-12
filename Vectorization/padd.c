#include <stdio.h>

// size is 8, 4 + 1, then round to multiple of 4 (int's size),
struct stu_a {
    int i;
    char c;
};

// size is 16, 8 + 1, then round to multiple of 8 (long's size),
struct stu_b {
    long l;
    char c;
};

// size is 24, l need padding by 4 before it, then round to multiple of 8 (long's size),
struct stu_c {
    int i;
    long l;
    char c;
};

// size is 16, 8 + 4 + 1, then round to multiple of 8 (long's size),
struct stu_d {
    long l;
    int i;
    char c;
};

int test() {
    printf("%s: %ld\n", "stu_a", sizeof(struct stu_a));
    printf("%s: %ld\n", "stu_b", sizeof(struct stu_b));
    printf("%s: %ld\n", "stu_c", sizeof(struct stu_c));
    printf("%s: %ld\n", "stu_c", sizeof(struct stu_d));
    return 0;
}

int main(int argc, char * argv[]) {
    test();
    return 0;
}
