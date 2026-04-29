FLAGS = -Wextra -Werror -Wall -std=c++17
FILES=Model/parser.cc Model/calculation.cc Controller/controller.cc

all: uninstall install tests

install:
	mkdir build
	cd ./build; cmake ../CMakeLists.txt; cmake --build .
	open build/view.app/Contents/MacOS/view

uninstall:
	rm -rf build

dist:
	cd ..; tar -cvf archive.tar src

dvi:
	makeinfo manual.texi --html
	open ./manual/index.html

tests: clean
	g++ --coverage $(FLAGS) tests.cc $(FILES) -lgtest -lgtest_main -o test
	./test

clean:
	rm -rf main *.dSYM test *.o *.a *.gcda *.gcno *.info *.out report .clang_format report.html build-* ../*tar manual *txt.user

style:
	cp ../materials/linters/.clang-format ./
#	clang-format -i Model/* Controller/* view/* main.cc tests.cc
	clang-format -n Model/* Controller/* view/* main.cc tests.cc
	rm .clang-format

gcov_report: tests
	lcov --no-external -ignore-errors mismatch -t "test" -o report.info -c -d .
	genhtml -o report.html report.info
	rm *.gcda *.gcno
	open report.html/index.html 2>/dev/null

valgrind: tests
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=RESULT_VALGRIND.txt ./test
