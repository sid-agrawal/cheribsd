#include <sys/mman.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
int main(int argc, char * argv[]) {
	if ( argc != 2) {
		printf("Usage ./side_car <memory to consume in GB>\n");
		exit(1);
	}

	printf("%d\n", atoi(argv[1]));
	size_t mem_to_consume = atof(argv[1]) * (1ULL << 30);

	printf("Mem to consume %zu\n", mem_to_consume);
	char * arr = malloc(mem_to_consume);

	if (!arr) { 
		printf("Malloc failed with error %s\n", strerror(errno));
		exit(1);
	}
	for (int i = 0; i < mem_to_consume; i+=4096) {
		arr[i]='s';
	}

	int res = mlockall(MCL_CURRENT);

	if (res) {
		printf("mlockall failed with errno %s\n", strerror(errno));
		exit(1);
	}
	int value;
	scanf("%d", value);
}
