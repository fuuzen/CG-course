# 使用 Qt 的构建工具
export QT_PATH=/c/Users/YQ/Desktop/CG-course/qt-6.8.0
export CMAKE=${QT_PATH}/Tools/CMake_64/bin/cmake.exe
export MAKE=${QT_PATH}/Tools/MinGW/bin/make.exe
export CXX=${QT_PATH}/Tools/MinGW/bin/g++.exe
export CC=${QT_PATH}/Tools/MinGW/bin/gcc.exe

# 使用 Visual Studio 的构建工具
export VS_PATH=/d/Program Files/Microsoft Visual Studio/2022/Community
export VS_GEN="Visual Studio 17 2022"

.PHONY: build sln run clean

all: build

build:
	${CMAKE} -S . -B build -G "MinGW Makefiles" -DCMAKE_CXX_COMPILER=${CXX} -DCMAKE_C_COMPILER=${CC}
	cd build && ${MAKE} -j8

sln:
	"${VS_PATH}/VC/Auxiliary/Build/vcvarsall.bat" x64
	cmake -S . -B build_sln -A "x64" -G ${VS_GEN}

run: build
	cp build/CG-HW3.exe .
	./CG-HW3.exe

clean:
	rm -rf build
	rm -rf build_sln