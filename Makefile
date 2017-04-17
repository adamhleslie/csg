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
	@cd build/bin && ./skinning ../../assets/pmd/Meiko_Sakine.pmd
.PHONY: run

val:
	@cd build/bin && valgrind ./skinning
.PHONY: val

gdb:
	@cd build/bin && gdb ./skinning
.PHONY: gdb

clean:
	-@rm -r build
.PHONY: clean
