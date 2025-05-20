
#include <stdio.h>
#include <stdlib.h>


int template_app_main(int argc, char* argv[]) {
    printf("Template App Started\n");

    if (argc > 2) {
        int a = atoi(argv[1]);
        int b = atoi(argv[2]);
        int result = template_app_add(a, b);
        printf("%d + %d = %d\n", a, b, result);
    } else {
        int result = template_app_add(5, 7);
        printf("5 + 7 = %d\n", result);
    }

    return 0;
}
