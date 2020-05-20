#!/bin/bash
kroc -c demo_cycles.occ
kroc -c demo_nets.occ
kroc -c utils.occ
kroc demo.occ demo_nets.o demo_cycles.o
