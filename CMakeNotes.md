### Introduce

CMake是一个跨平台的安装（编译）工具，可以用简单的语句来描述所有平台的安装(编译过程)。他能够输出各种各样的makefile或者project文件，能测试编译器所支持的C++特性,类似UNIX下的automake。只是 CMake 的组态档取名为 CMakeLists.txt。Cmake 并不直接建构出最终的软件，而是产生标准的建构档（如 Unix 的 Makefile 或 Windows Visual C++ 的 projects/workspaces），然后再依一般的建构方式使用。这使得熟悉某个集成开发环境（IDE）的开发者可以用标准的方式建构他的软件，这种可以使用各平台的原生建构系统的能力是 CMake 和 SCons 等其他类似系统的区别之处。

### Step 1 基础示例

#### 1、基本内容（版本号、工程名、可执行文件）

最基本的项目是由源代码文件构建的可执行文件。对于简单的项目，首先创建一个工程文件夹 TutorialPro ,在其文件夹下可以创建一个CMakelists.txt文件。

**CMakelists.txt**

```cmake
# 设置 cmake 最小版本号
cmake_minimum_required(VERSION 3.10)

# 设置工程名
project(Tutorial)

# 添加可执行文件
add_executable(Tutorial tutorial.cxx)

```

#### 2、添加版本号和配置头文件

使用 project 命令设置工程名和版本号。

**CMakelists.txt**

```cmake
# 设置 cmake 最小版本号
cmake_minimum_required(VERSION 3.10)

# 设置工程名和版本号
project(Tutorial VERSION 1.0)
```

配置头文件，将版本号传递给源代码。

```cmake
configure_file(TutorialConfig.h.in TutorialConfig.h)
```

由于配置文件将被写入二叉树，我们必须将该目录添加到搜索包含文件的路径列表中。将以下行添加到CMakelists的末尾。

```cmake
target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
```

在源目录下创建 TutorialConfig.h.in 文件。

**TutorialConfig.h.in**

```cmake
// the configured options and settings for Tutorial
#define Tutorial_VERSION_MAJOR @Tutorial_VERSION_MAJOR@
#define Tutorial_VERSION_MINOR @Tutorial_VERSION_MINOR@
```

当CMake配置这个头文件时，@Tutorial_VERSION_MAJOR@ 和 @Tutorial_VERSION_MINOR@ 的值将被替换。

修改包含配置头文件 (TutorialConfig.h) 的 tutorial.cxx 。

通过更新 tutorial.cxx 打印出可执行文件的名称和版本号。

**tutorial.cxx** 

```c++
if (argc < 2) {
    // report version
    std::cout << argv[0] << " Version " << Tutorial_VERSION_MAJOR << "."
              << Tutorial_VERSION_MINOR << std::endl;
    std::cout << "Usage: " << argv[0] << " number" << std::endl;
    return 1;
}
```

#### 3、指定 C++ 标准

通过在 tutorial.cxx 中用 std:stod 替换 atof 来添加一些 c++ 11 特性到我们的项目中。同时，删除tinclude (cstdlib)。

**tutorial.cxx**

```c++
const double inputValue = std::stod(argv[1]);
```

我们需要在CMake代码中明确声明应该使用正确的标志。在CMake中启用对特定c++标准的支持的最简单方法是使用 CIAKE CXX standard 变量。可以将 cmakellists .txt 文件中的 CIAKE CXX STANDARD变量设置为11，并将 CIAKE CXX STANDARD REQUIRED  设置为True。确保在添加可执行文件的调用之上添加CMAKE_CX STANDARD声明。

**tutorial.cxx**

```cmake
cmake_minimum_required(VERSION 3.10)

# 设置工程名和版本号
project(Tutorial VERSION 1.0)

# C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
```

#### 4、构建和测试

运行 cmake 可执行文件或 cmake-gui 来配置项目，然后使用您选择的构建工具来构建它。例如，从命令行，我们可以切换到 tutorial 工程目录下创建一个 bulid 目录。

```c
mkdir build
```

到 build 目录并运行CMake来配置项目并生成一个本地构建系统。

```c++
cd build
cmake ../TutorialPro 
```

然后调用构建系统来实际编译/链接项目。

```cmake
cmake --build 
```

使用以下命令运行

```c++
Tutorial 4294967296
Tutorial 10
Tutorial
```

