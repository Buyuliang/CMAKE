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

##### 命令解析

```cmake
project(Tutorial)
```

CMake 构建包含了一个项目名称，会自动生成一些变量，比如：PROJECT_NAME 这个变量，PROJECT_NAME 为变量名，${PROJECT_NAME}为变量值，为 Tutorial。

```cmake
add_executable(Tutorial tutorial.cxx)
```

add_executable 命令是指定某些源文件生成可执行文件，第一个参数是可执行文件名，第二个参数是要编译的源文件列表。

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
# 配置一个头文件来传递源代码中的一些 CMake 设置
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
#define Tutorial_VERSION_MAJOR X	//@Tutorial_VERSION_MAJOR@
#define Tutorial_VERSION_MINOR X	//@Tutorial_VERSION_MINOR@
```

当CMake配置这个头文件时，@Tutorial_VERSION_MAJOR@ 和 @Tutorial_VERSION_MINOR@ 的值将被替换。

修改包含配置头文件 (TutorialConfig.h) 的 tutorial.cxx 。

通过更新 tutorial.cxx 打印出可执行文件的名称和版本号。

**tutorial.cxx** 

```c++
#include <iostream>
#include <TutorialConfig.h>

 int main(int argc, char* argv[])
{
	if (argc < 2) {
		// report version
		std::cout << argv[0] << " Version " << Tutorial_VERSION_MAJOR << "."
				  << Tutorial_VERSION_MINOR << std::endl;
		std::cout << "Usage: " << argv[0] << " number" << std::endl;
		return 1;
	}
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

如果搭建的是 windows 的编译环境需要安装 CMake 和 MinGW 工具 [配置环境]

```cmake
set(CMAKE_C_FLAGS "-g -Wall  -I D:\\mingw64\\include -L D:\\mingw64\\lib")
set(CMAKE_CXX_COMPILER "g++")	#设置C++编译器  
set(CMAKE_CXX_FLAGS "-g -Wall  -I D:\\mingw64\\include -L D:\\mingw64\\lib")  
```

运行 cmake 可执行文件或 cmake-gui 来配置项目，然后使用您选择的构建工具来构建它。例如，从命令行，我们可以切换到 tutorial 工程目录下创建一个 bulid 目录。

```c
mkdir build
```

到 build 目录并运行CMake来配置项目并生成一个本地构建系统。

```c++
cd build
cmake -G "MinGW Makefiles" ../../TutorialPro 
```

然后调用构建系统来实际编译/链接项目。

```cmake
make
```

使用以下命令运行

```c++
./Tutorial.exe
```

### Step 2 添加库

#### 1、使用 add_library 添加库

例：我们将把库放到名为MathFunctions的子目录中。该目录已经包含了一个头文件MathFunctions.h，以及一个源文件 mysqrt.cxx。源文件有一个名为 mysqrt 的函数，它提供了与编译器的 sqrt 函数类似的功能。

**MathFunctions/CMakeLists.txt**

```cmake
# 设置 cmake 最小版本号
cmake_minimum_required(VERSION 3.10)

# 设置工程名
project(MathFunctions)
message(${PROJECT_BINARY_DIR})
add_library(MathFunctions mysqrt.cxx)

target_include_directories(MathFunctions PUBLIC
                          "${PROJECT_BINARY_DIR}"
                          "${PROJECT_SOURCE_DIR}"
                          )
```

#### 2、添加库到可执行文件

为了使用这个新库，我们将在上层 CMakeLists 中添加一个 add_subdirectory 去调用 CMakeLists.txt 文件。我们将新库添加到可执行文件中，并将 MathFunctions 添加为包含目录，以便可以找到 mysqrt.h 头文件。

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
	std::cout << "mysqrt: " << outputValue << std::endl;
#else
	const double outputValue = sqrt(inputValue);
	std::cout << "sqrt: " << outputValue << std::endl;
#endif
```

**TutorialConfig.h.in**

```cmake
#cmakedefine USE_MYMATH
```

现在让我们更新USE MYMATH的值。最简单的方法是使用 cmake-gui 或 cmake，如果你在终端。或者，如果您想要从命令行更改该选项，可以尝试：

```cmake
$ cmake -G "MinGW Makefiles" ../../TutorialPro  -D USE_MYMATH=OFF
```

### Step 3 添加库的使用要求

#### 1、主要命令

```cmake
target_compile_definitions()

target_compile_options()

target_include_directories()

target_link_libraries()
```

任何链接到 `MathFunctions` 的都需要包含当前源目录，而 `MathFunctions `本身不需要。

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

### Step 4 安装和测试

#### 1、安装规则

对于` MathFunctions `我们想要安装 `library `和 `header` 文件，对于应用程序我们想要安装可执行文件和配置头文件。

**MathFunctions/CMakeLists.txt**

```cmake
install(TARGETS MathFunctions DESTINATION lib)
install(FILES MathFunctions.h DESTINATION include)
```

在最上层 `CMakeLists.txt` 中添加

**CMakeLists.txt**

```cmake
install(TARGETS Tutorial DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h"
  DESTINATION include
  )
```

安装命令

```
cmake --install .
```

CMake 变量 `CIAKE ISTALL PREFI` 用于确定文件安装的根目录。如果使用 `cmake--install `命令，安装前缀可以通过 `-prefix` 参数被覆盖。例如

```
cmake --install . --prefix "/home/myuser/installdir"
```

#### 2、测试

接下来让我们测试应用程序。在最上层` CMakelists.txt` 的最后，我们可以启用测试，然后添加一些基本测试，以验证应用程序是否正常工作。

**CMakeLists.txt**

```cmake
enable_testing()

# does the application run
add_test(NAME Runs COMMAND Tutorial 25)

# does the usage message work?
add_test(NAME Usage COMMAND Tutorial)
set_tests_properties(Usage
  PROPERTIES PASS_REGULAR_EXPRESSION "Usage:.*number"
  )

# define a function to simplify adding tests
function(do_test target arg result)
  add_test(NAME Comp${arg} COMMAND ${target} ${arg})
  set_tests_properties(Comp${arg}
    PROPERTIES PASS_REGULAR_EXPRESSION ${result}
    )
endfunction(do_test)

# do a bunch of result based tests
do_test(Tutorial 4 "4 is 2")
do_test(Tutorial 9 "9 is 3")
do_test(Tutorial 5 "5 is 2.236")
do_test(Tutorial 7 "7 is 2.645")
do_test(Tutorial 25 "25 is 5")
do_test(Tutorial -25 "-25 is [-nan|nan|0]")
do_test(Tutorial 0.0001 "0.0001 is 0.01")
```

- 第一个测试只是验证应用程序是否运行，没有出现段错误或崩溃，并且返回值为零。这是`CTest`测试的基本形式。

- 第二个测试使用 `PASS RECILAR EXPRESSION` 测试属性来验证测试的输出是否包含某些字符串。
- 验证当提供的参数数量不正确时是否打印使用消息。最后，我们有一个名为 `do_test` 的函数，它运行应用程序并验证计算出的平方根对于给定输入是否正确。
- 对于` do_test` 的每次调用，都会根据传入的参数将另一个测试添加到项目中，其中包含名称、输入和预期结果。
- 重新构建应用程序，然后cd到二进制目录并运行`ctest`可执行文件: `ctest -N` 和 `ctest -vv` 。对于多配置生成器(例如Visual Studio)，必须指定配置类型。例如，在 `build` 目录使用 `ctest -C Debug -VV` (而不是调试子目录!)中使用`ctest Debug -vv`。或者，从IDE构建 `RUN_TESTS` 目标。

### Step 5 

如果平台有 `log` 和 `exp`，那么我们将在 `mysqrt` 函数中使用它们来计算平方根。我们首先使用`MathFunctions/CMakelists.txt` 中的` CheckSymbolExists `模块测试这些函数的可用性。在一些平台上，我们需要链接到`m` 库。如果最初没有找到` log` 和` exp`，需要` m` 库，然后再试一次。

**MathFunctions/CMakeLists.txt**

```cmake
include(CheckSymbolExists)
check_symbol_exists(log "math.h" HAVE_LOG)
check_symbol_exists(exp "math.h" HAVE_EXP)
if(NOT (HAVE_LOG AND HAVE_EXP))
  unset(HAVE_LOG CACHE)
  unset(HAVE_EXP CACHE)
  set(CMAKE_REQUIRED_LIBRARIES "m")
  check_symbol_exists(log "math.h" HAVE_LOG)
  check_symbol_exists(exp "math.h" HAVE_EXP)
  if(HAVE_LOG AND HAVE_EXP)
    target_link_libraries(MathFunctions PRIVATE m)
  endif()
endif()
```

如果可用，使用 `target_compile_definitions()` 指定 `HAVE_LOG`  和 `HAVE_EXP`  做为私有编译定义。

**MathFunctions/CMakeLists.txt**

```cmake
if(HAVE_LOG AND HAVE_EXP)
  target_compile_definitions(MathFunctions
                             PRIVATE "HAVE_LOG" "HAVE_EXP")
endif()
```

如果 `log`  和 `exp` 在系统上可用，那么我们将在`mysart`函数中使用它们来计算平方根。在`MathFunctions/mysqrt`中添加以下代码到`mysqrt`函数。

**MathFunctions/mysqrt.cxx**

```c++
#if defined(HAVE_LOG) && defined(HAVE_EXP)
  double result = exp(log(x) * 0.5);
  std::cout << "Computing sqrt of " << x << " to be " << result
            << " using log and exp" << std::endl;
#else
  double result = x;
```

添加`cmath`头文件

**MathFunctions/mysqrt.cxx**

```c++
#include <cmath>
```

### Step 6 添加自定义命令和生成文件

首先移除`MathFunctions/CMakeLists.txt`中 `log`和`exp函数`，然后从`mysqrt.cxx`中移除``HAVE_LOG` ``和`HAVE_EXP `，同时也移除`#include <cmath>`，在``MathFunctions`目录下，新建一个`MakeTable.cxx` 文件

**MathFunctions/CMakeLists.txt**

```cmake
add_executable(MakeTable MakeTable.cxx)
```

然后，我们添加一个自定义命令，指定如何通过运行MakeTable生成Table.h。

**MathFunctions/CMakeLists.txt**

```cmake
add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Table.h
  COMMAND MakeTable ${CMAKE_CURRENT_BINARY_DIR}/Table.h
  DEPENDS MakeTable
  )
```

接下来我们必须让`CMake`知道`mysqrt.cxx`依赖于生成的文件`Table.h`，这是通过添加生成的`Table`来完成的`Table.h`到库`MathFunctions`的源代码列表。

**MathFunctions/CMakeLists.txt**

```cmake
add_library(MathFunctions
            mysqrt.cxx
            ${CMAKE_CURRENT_BINARY_DIR}/Table.h
            )
```

我们还必须将当前的二进制目录添加到包含目录列表中，以便`Table.h`可以被`mysart.cxx`找到并包含。

**MathFunctions/CMakeLists.txt**

```cmake
target_include_directories(MathFunctions
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
          )
```

现在让我们使用生成的表。首先,修改`mysqrt.cxx`包含`Table.h`接下来，我们可以重写`mysqrt`函数来使用这个表

**MathFunctions/mysqrt.cxx**

```cmake
double mysqrt(double x)
{
  if (x <= 0) {
    return 0;
  }

  // use the table to help find an initial value
  double result = x;
  if (x >= 1 && x < 10) {
    std::cout << "Use the table to help find an initial value " << std::endl;
    result = sqrtTable[static_cast<int>(x)];
  }

  // do ten iterations
  for (int i = 0; i < 10; ++i) {
    if (result <= 0) {
      result = 0.1;
    }
    double delta = x - (result * result);
    result = result + 0.5 * delta / result;
    std::cout << "Computing sqrt of " << x << " to be " << result << std::endl;
  }

  return result;
}
```

### Step 7 打包安装程序

接下来假设我们想要将我们的项目分发给其他人，以便他们能够使用它。我们希望在各种平台上提供二进制和源代码发行版。这与我们之前在安装和测试中所做的安装略有不同，在安装中我们安装了从源代码构建的二进制文件。在这个例子中，我们将构建支持二进制安装和包管理特性的安装包。为此，我们将使用cppack创建特定于平台的安装程序。具体来说，我们需要在最上级`cmakelist.txt`的底部添加几行代码文件

**CMakeLists.txt**

```cmake
include(InstallRequiredSystemLibraries)
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set(CPACK_PACKAGE_VERSION_MAJOR "${Tutorial_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${Tutorial_VERSION_MINOR}")
include(CPack)
```

CPack变量设置为存储该项目的许可和版本信息的位置。

最后，我们将包含CPack模块，该模块将使用这些变量和当前系统的一些其他属性来设置安装程序。下一步是以通常的方式构建项目，然后运行cpack可执行文件。要构建一个二进制发行版，从二进制目录运行:

```
cpack
```

要指定生成器，可以使用`-G`选项。对于多配置构建，使用`-c`来指定配置。

```
cpack -G ZIP -C Debug
```

创建一个源代码发行版本

```
cpack --config CPackSourceConfig.cmake
```

### Step 8 添加测试支持

**CMakeLists.txt**

```cmake
# enable testing
enable_testing()

# enable dashboard scripting
include(CTest)
```

CTest模块将自动调用`enable_testing()`，因此我们可以从CMake文件中删除它。我们还需要创建一个`CTestconfig.cmake`，在顶级目录中，我们可以指定项目的名称和提交测试的位置。

**CTestConfig.cmake**

```camke
set(CTEST_PROJECT_NAME "CMakeTutorial")
set(CTEST_NIGHTLY_START_TIME "00:00:00 EST")

set(CTEST_DROP_METHOD "http")
set(CTEST_DROP_SITE "my.cdash.org")
set(CTEST_DROP_LOCATION "/submit.php?project=CMakeTutorial")
set(CTEST_DROP_SITE_CDASH TRUE)
```

```
ctest [-VV] -D Experimental
```

对于多配置生成器(例如Visual Studio)，必须指定配置类型:

```
ctest [-VV] -C Debug -D Experimental
```

### Step 9 选择静态或共享库

在本节中，我们将展示如何使用`BUILD_SHARED_LIBS`变量来控制`add_library()`的默认行为，并允许控制如何构建没有显式类型(STATIC、SHARED、MODULE或OBJECT)的库。

为此，我们需要将`BUILD_SHARED_LIBS`添加到顶级`CMakeLists.txt`中。三种。我们使用`option()`命令，因为它允许用户选择值是`ON`还是`OFF`。接下来，我们将重构`MathFunctions`，使其成为一个使用`mysqrt`或`sqrt`封装的真正的库，而不需要调用代码来实现这个逻辑。这也意味着`USE_MYMATH`将不控制构建`MathFunctions`。

**CMakeLists.txt**

```cmake
cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(Tutorial VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

# configure a header file to pass the version number only
configure_file(TutorialConfig.h.in TutorialConfig.h)

# add the MathFunctions library
add_subdirectory(MathFunctions)

# add the executable
add_executable(Tutorial tutorial.cxx)
target_link_libraries(Tutorial PUBLIC MathFunctions)
```

现在我们已经使`MathFunctions`始终被使用，接下来需要更新该库的逻辑。所以,在`MathFunctions / CMakelists`，我们需要创建一个`SqrtLibrary`，当启用`USE_MYMATH`时，有条件地构建和安装它。现在，我们将显式要求静态构建`SqrtLibrary`。

**MathFunctions/CMakeLists.txt**

```cmake
# add the library that runs
add_library(MathFunctions MathFunctions.cxx)

# state that anybody linking to us needs to include the current source dir
# to find MathFunctions.h, while we don't.
target_include_directories(MathFunctions
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
                           )

# should we use our own math functions
option(USE_MYMATH "Use tutorial provided math implementation" ON)
if(USE_MYMATH)

  target_compile_definitions(MathFunctions PRIVATE "USE_MYMATH")

  # first we add the executable that generates the table
  add_executable(MakeTable MakeTable.cxx)

  # add the command to generate the source code
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/Table.h
    COMMAND MakeTable ${CMAKE_CURRENT_BINARY_DIR}/Table.h
    DEPENDS MakeTable
    )

  # library that just does sqrt
  add_library(SqrtLibrary STATIC
              mysqrt.cxx
              ${CMAKE_CURRENT_BINARY_DIR}/Table.h
              )

  # state that we depend on our binary dir to find Table.h
  target_include_directories(SqrtLibrary PRIVATE
                             ${CMAKE_CURRENT_BINARY_DIR}
                             )

  target_link_libraries(MathFunctions PRIVATE SqrtLibrary)
endif()

# define the symbol stating we are using the declspec(dllexport) when
# building on windows
target_compile_definitions(MathFunctions PRIVATE "EXPORTING_MYMATH")

# install rules
set(installable_libs MathFunctions)
if(TARGET SqrtLibrary)
  list(APPEND installable_libs SqrtLibrary)
endif()
install(TARGETS ${installable_libs} DESTINATION lib)
install(FILES MathFunctions.h DESTINATION include)
```

**MathFunctions/mysqrt.cxx**

```c++
#include <iostream>

#include "MathFunctions.h"

// include the generated table
#include "Table.h"

namespace mathfunctions {
	namespace detail {
        // a hack square root calculation using simple operations
        double mysqrt(double x)
        {
          if (x <= 0) {
            return 0;
          }

          // use the table to help find an initial value
          double result = x;
          if (x >= 1 && x < 10) {
            std::cout << "Use the table to help find an initial value " << std::endl;
            result = sqrtTable[static_cast<int>(x)];
          }

          // do ten iterations
          for (int i = 0; i < 10; ++i) {
            if (result <= 0) {
              result = 0.1;
            }
            double delta = x - (result * result);
            result = result + 0.5 * delta / result;
            std::cout << "Computing sqrt of " << x << " to be " << result << std::endl;
          }

          return result;
    	}
    }
}
```

最后，更新`MathFunctions/MathFunctions.h`以使用dll导出定义:

**MathFunctions/MathFunctions.h**

```c++
#if defined(_WIN32)
#  if defined(EXPORTING_MYMATH)
#    define DECLSPEC __declspec(dllexport)
#  else
#    define DECLSPEC __declspec(dllimport)
#  endif
#else // non windows
#  define DECLSPEC
#endif

namespace mathfunctions {
double DECLSPEC sqrt(double x);
}
```

此时，如果您构建了所有代码，您可能会注意到链接失败，因为我们将一个没有位置独立代码的静态库与一个具有位置独立代码的库组合在一起。解决这个问题的方法是`POSITION_INDEPENDENT_CODE将SqrtLibrary`目标属性设置为True，而不管构建type是什么

**MathFunctions/CMakeLists.txt**

```cmake
  # state that SqrtLibrary need PIC when the default is shared libraries
  set_target_properties(SqrtLibrary PROPERTIES
                        POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS}
                        )

  target_link_libraries(MathFunctions PRIVATE SqrtLibrary)
```

