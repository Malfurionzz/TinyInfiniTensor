.PHONY : build clean format install-python test-cpp test-onnx

TYPE ?= Release
TEST ?= ON

CMAKE_OPT = -DCMAKE_BUILD_TYPE=$(TYPE)
CMAKE_OPT += -DBUILD_TEST=$(TEST)

build:
	mkdir -p build/$(TYPE)
	cd build/$(TYPE) && cmake $(CMAKE_OPT) ../.. -DCMAKE_EXPORT_COMPILE_COMMANDS=1 && make -j64

clean:
	rm -rf build

test-cpp:
	@echo
	cd build/$(TYPE) && make test
