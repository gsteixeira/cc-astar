
SRC := src/

default: $(SRC)astar.cc $(SRC)demo.cc $(SRC)tile.cc
	g++ -std=c++20 $(SRC)astar.cc $(SRC)demo.cc $(SRC)tile.cc -o astar_demo

run:
	./astar_demo

clean:
	rm -f astar_demo


lint:
	find ./ -name "*.cc" -type f -exec sed -i 's/ \+$$//g' {} \;
	find ./ -name "*.h" -type f -exec sed -i 's/ \+$$//g' {} \;
