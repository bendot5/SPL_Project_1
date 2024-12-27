# Please implement your Makefile rules and targets below.
# Customize this file to define how to build your project.

all: clean link 

link : compile
	g++ -o ./bin/simulation ./bin/*.o

compile: ./src/*.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o ./bin/Settlement.o ./src/Settlement.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o ./bin/Facility.o ./src/Facility.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o ./bin/Auxiliary.o ./src/Auxiliary.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o ./bin/SelectionPolicy.o ./src/SelectionPolicy.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o ./bin/Plan.o ./src/Plan.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o ./bin/Simulation.o ./src/Simulation.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o ./bin/Action.o ./src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o ./bin/main.o ./src/main.cpp
	
clean:
	rm -f ./bin/*