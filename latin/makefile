
p = latin
$p: $p.cpp
	g++ -O3 -Wall $p.cpp -o $p

.PHONY: run

run: $p
	time ./$p
	cat *.out

