cc = g++
judge: now.o cgroup.o spawn.o main.o allutils.o
	${cc} main.o cgroup.o now.o spawn.o allutils.o -std=c++17 -lstdc++fs -o judge
	make clean

now.o:
	${cc} -c src/utils/now.cc

cgroup.o:
	${cc} -c src/cgroup.cc -std=c++17 -lstdc++fs

spawn.o:
	${cc} -c src/spawn.cc -std=c++17 -lstdc++fs

main.o:
	${cc} -c src/main.cpp -std=c++17 -lstdc++fs

allutils.o:
	${cc} -c src/utils/allutils.cc -std=c++17 -lstdc++fs
clean:
	rm cgroup.o main.o now.o spawn.o allutils.o 
