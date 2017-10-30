#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Sanchis.h"
#include "Circuit.h"

int main(int argc, char** argv) {
    printf("Loading circuit\n");
    char* filename = "./ckt/chip1.netlist";
    if (argc > 1) filename = argv[1];

    Circuit* ckt = newCircuit(filename);
    Sanchis* sanchis = newSanchis(ckt, 10, 1);
    SOLUTION solution = (int*) calloc(modCount(ckt), sizeof (int));

    printf("circuit loaded\n");
    printf("circuit net count: %d\n", netCount(ckt));
    printf("circuit mod count: %d\n", modCount(ckt));

    srand(time(0));

    int count = 1;
    if (argc > 2) sscanf(argv[2], "%d", &count);

    while (count-- > 0) {
        for (int i = 0; i < modCount(ckt); i++) {
            int r = ((float) rand() / RAND_MAX) * 2;
            solution[i] = r;
        }

        int before = cutSize(ckt, solution);
        Sanchis_repair(sanchis, solution);
        int after = cutSize(ckt, solution);
        printf("%d %d\n", before, after);
    }

    free(solution);
    deleteSanchis(sanchis);
    deleteCircuit(ckt);

    exit(0);
}