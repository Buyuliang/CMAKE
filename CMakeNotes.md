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

### Step 2 添加库

#### 1、使用 add_library 添加库

例：我们将把库放到名为MathFunctions的子目录中。该目录已经包含了一个头文件MathFunctions.h，以及一个源文件 mysgrt.cxx。源文件有一个名为 mysgrt 的函数，它提供了与编译器的 sart 函数类似的功能。

**MathFunctions/CMakeLists.txt**

```cmake
add_library(MathFunctions mysqrt.cxx)
```

#### 2、添加库到可执行文件

为了使用这个新库，我们将在上层 CMakelists 中添加一个 add_subdirectory 去调用 CMakeLists.txt 文件。我们将新库添加到可执行文件中，并将 MathFunctions 添加为包含目录，以便可以找到 mysçrt.h 头文件。

**CMakeLists.txt**

```cmake
# 添加 MathFunctions library
add_subdirectory(MathFunctions)

# 添加可执行文件
add_executable(Tutorial tutorial.cxx)

target_link_libraries(Tutorial PUBLIC MathFunctions)

# 添加文件路径到二进制树以便于找到 TutorialConfig.h
target_include_directories(Tutorial PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}/MathFunctions"
                          )
```

#### 3、设置库为可选择

将 MathFunctions 库设置为可选的。第一步是在上级 CMakeLists.txt 文件中添加一个选项。

**CMakeLists.txt**

```cmake
option(USE_MYMATH "Use tutorial provided math implementation" ON)

# 配置一个头文件来传递源代码中的一些 CMake 设置
configure_file(TutorialConfig.h.in TutorialConfig.h)

if(USE_MYMATH)
  add_subdirectory(MathFunctions)
  list(APPEND EXTRA_LIBS MathFunctions)
  list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/MathFunctions")
endif()

# 添加可执行文件
add_executable(Tutorial tutorial.cxx)

target_link_libraries(Tutorial PUBLIC ${EXTRA_LIBS})

# 添加文件路径到二进制树以便于找到 TutorialConfig.h
target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           ${EXTRA_INCLUDES}
                           )
```

**tutorial.cxx**

```cmake
#ifdef USE_MYMATH
  const double outputValue = mysqrt(inputValue);
#else
  const double outputValue = sqrt(inputValue);
#endif
```

**TutorialConfig.h.in**

```cmake
#cmakedefine USE_MYMATH
```

现在让我们更新USE MYMATH的值。最简单的方法是使用 cmake-gui 或 cmake，如果你在终端。或者，如果您想要从命令行更改该选项，可以尝试：

```cmake
cmake ../TutorialPro -D USE_MYMATH=OFF
```

### Step 3 添加库的使用要求

#### 1、主要命令

```cmake
target_compile_definitions()

target_compile_options()

target_include_directories()

target_link_libraries()
```

任何链接到 MathFunctions 的都需要包含当前源目录，而 MathFunctions 本身不需要。

**MathFunctions/CMakeLists.txt**

```cmake
target_include_directories(MathFunctions
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          )
```

**CMakeLists.txt**

```cmake
if(USE_MYMATH)
  add_subdirectory(MathFunctions)
  list(APPEND EXTRA_LIBS MathFunctions)
endif()

target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
```

然后使用 cmake 执行或者 使用 cmake-gui 配置工程然后构建它。

