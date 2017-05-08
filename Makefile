build_run: build run
.PHONY: build_run

build:
	@cd build && make
.PHONY: build

default:
	-@rm -r build
	@mkdir build
	@cd build && cmake .. && make
.PHONY: default

debug:
	-@rm -r build
	@mkdir build
	@cd build && cmake -DCMAKE_BUILD_TYPE=Debug .. && make
.PHONY: debug

release:
	-@rm -r build
	@mkdir build
	@cd build && cmake -DCMAKE_BUILD_TYPE=Release .. && make
.PHONY: release

run:
	@cd build/bin && ./csg
.PHONY: run

val:
	@cd build/bin && valgrind ./csg
.PHONY: val

gdb:
	@cd build/bin && gdb ./csg
.PHONY: gdb

clean:
	-@rm -r build
.PHONY: clean
