.PHONY: all clean test cli_app

all: cli_app

cli_app:
	mkdir -p build
	cd build && cmake .. && cmake --build .
	cp build/cli_app ./cli_app || cp build/Debug/cli_app.exe ./cli_app.exe || cp build/cli_app.exe ./cli_app.exe

test:
	mkdir -p build
	cd build && cmake .. && cmake --build .
	cd build && ctest --output-on-failure

clean:
	rm -rf build cli_app cli_app.exe
