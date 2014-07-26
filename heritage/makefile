p = heritage

$p: $p.cpp
	g++ -O3 -Wall $p.cpp -o $p

.PHONY: run

run: $p
	./$p
	cat *.out
