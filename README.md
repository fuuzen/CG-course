
在您的电脑上构建的时候，请修改 `code` 下的 `Makefile` 和 `CmakeLists.txt` 中的如下环境变量为您自己电脑上的：
- `QT_PATH`: 您电脑上的 Qt 安装路径
- `QT6_DIR`: `${QT_PATH}` 下具体的 cmake 构建相关路径
- `GLEW_PATH`: 您电脑上的 glew 安装路径

我的 Qt 6.8.0 自带的编译构建环境, 仅供参考：
```shell shell
# cmake --version
cmake version 3.29.3
CMake suite maintained and supported by Kitware (kitware.com/cmake).

# make --version
GNU Make 4.2.1
Built for x86_64-w64-mingw32
Copyright (C) 1988-2016 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.

# g++ --version
g++.exe (x86_64-posix-seh-rev1, Built by MinGW-Builds project) 13.1.0
Copyright (C) 2023 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

# gcc --version
gcc.exe (x86_64-posix-seh-rev1, Built by MinGW-Builds project) 13.1.0
Copyright (C) 2023 Free Software Foundation, Inc.
This is free software; see the source for copying conditions.  There is NO
warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```

依赖库：
- QT: 6.8.0
- glew: 2.1.0