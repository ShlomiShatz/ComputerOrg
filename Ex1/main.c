#include <stdio.h>
#include <stdbool.h>
#include "ex1.h"

int main() {
    bool bitArray[] = {true, true, true, true, true, false, false, true};

	printf("%d\n", is_big_endian());

	printf("%d\n", get_sign_magnitude(bitArray));

    printf("%d\n", get_two_comp(bitArray));

	return 0;
}
