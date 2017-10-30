/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   circuitTest.c
 * Author: edward
 *
 * Created on October 10, 2017, 10:15 AM
 */

#include <stdio.h>
#include <stdlib.h>
#include "Circuit.h"

/*
 *
 */
int main(int argc, char** argv) {
    NET net;
    MOD mod;
    int t = 0;

    char* filename = "./ckt/test.netlist";
    Circuit* ckt = newCircuit(filename);

    printf("%d %d\n", t++,  netCount(ckt) == 11);
    printf("%d %d\n", t++,  modCount(ckt) == 10);
    printf("%d %d\n", t++,  netSize(ckt, 0) == 2);
    printf("%d %d\n", t++,  netSize(ckt, 4) == 2);
    printf("%d %d\n", t++,  netSize(ckt, 5) == 3);
    printf("%d %d\n", t++,  netSize(ckt, 10) == 4);

    printf("%d %d\n", t++,  modSize(ckt, 0) == 2);
    printf("%d %d\n", t++,  modSize(ckt, 1) == 3);

    net = getNet(ckt, 0);
    printf("%d %d\n", t++,  net[0] == 0);
    printf("%d %d\n", t++,  net[1] == 1);

    net = getNet(ckt, 5);
    printf("%d %d\n", t++,  net[0] == 0);
    printf("%d %d\n", t++,  net[1] == 2);
    printf("%d %d\n", t++,  net[2] == 4);

    mod = getMod(ckt, 0);
    printf("%d %d\n", t++,  mod[0] == 0);
    printf("%d %d\n", t++,  mod[1] == 5);

    int sol[10] = {0, 0, 0, 0, 0, 1, 1, 1, 1, 1};
    printf("%d %d\n", t++,  cutSize(ckt, sol) == 4);

    deleteCircuit(ckt);

    return (EXIT_SUCCESS);
}

