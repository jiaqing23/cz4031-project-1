build:
	g++ -std=c++17 main.cpp database.cpp memory.cpp b-plus-tree.cpp -o out

clean:
	rm out

run: build
	./out

debug:
	g++ -std=c++17 -DDEBUG main.cpp database.cpp memory.cpp b-plus-tree.cpp -o out
	./out

btree:
	g++ -std=c++17 -DDEBUG b-plus-tree.cpp btree_main.cpp -o out
	./out

commit:
	git add .
	git commit -m .
	git push origin master