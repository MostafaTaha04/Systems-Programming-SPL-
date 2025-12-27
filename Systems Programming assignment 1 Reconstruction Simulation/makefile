all: clean bin/simulation

bin/simulation: main Action Auxiliary Facility Plan SelectionPolicy Simulation Settlement
	g++ -o bin/simulation bin/main.o bin/Action.o bin/Auxiliary.o bin/Facility.o bin/Plan.o bin/SelectionPolicy.o bin/Settlement.o bin/Simulation.o

main:
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp

Action:
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp

Auxiliary:
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Auxiliary.o src/Auxiliary.cpp

Facility:
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Facility.o src/Facility.cpp

Plan:
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Plan.o src/Plan.cpp

SelectionPolicy:
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/SelectionPolicy.o src/SelectionPolicy.cpp

Settlement:
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Settlement.o src/Settlement.cpp

Simulation:
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Simulation.o src/Simulation.cpp	

clean:
	rm -rf bin/*.o simulation

valgrind:
	valgrind --leak-check=full --show-reachable=yes bin/simulation

run:
	rm -f bin/*.o
	./bin/simulation ./config_file.txt