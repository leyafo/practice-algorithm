#include <stdlib.h>
#include <stdio.h>

struct st{
    int a;
    char b;
    void *c;
};


int cmp(int A, int B)
{
    if(A > B)
        return 1;
    else if (B > A)
        return -1;
    return 0;
}

int main(int argc, char *argv[])
{
    int a = 1; b = 2;
    int result = cmp(a, b);
    cmp(3, 2);

    return 0;
}
