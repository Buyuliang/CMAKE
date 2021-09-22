CMake是一个跨平台的安装（编译）工具，可以用简单的语句来描述所有平台的安装(编译过程)。他能够输出各种各样的makefile或者project文件，能测试编译器所支持的C++特性,类似UNIX下的automake。只是 CMake 的组态档取名为 CMakeLists.txt。Cmake 并不直接建构出最终的软件，而是产生标准的建构档（如 Unix 的 Makefile 或 Windows Visual C++ 的 projects/workspaces），然后再依一般的建构方式使用。这使得熟悉某个集成开发环境（IDE）的开发者可以用标准的方式建构他的软件，这种可以使用各平台的原生建构系统的能力是 CMake 和 SCons 等其他类似系统的区别之处。

### Step 1 基础示例

最基本的项目是由源代码文件构建的可执行文件。对于简单的项目，可以创建一个CMakelists.txt。



```txt
# 设置 cmake 最小版本号
cmake_minimum_required(VERSION 3.10)

# 设置工程名
project(Tutorial)

# 添加可执行文件
add_executable(Tutorial tutorial.cxx)

```

#### 添加版本号和配置头文件

使用 project 命令设置工程名和版本号。

```txt
# 设置 cmake 最小版本号
cmake_minimum_required(VERSION 3.10)

# 设置工程名和版本号
project(Tutorial VERSION 1.0)
```

配置头文件，将版本号传递给源代码。

```txt
configure_file(TutorialConfig.h.in TutorialConfig.h)
```

由于配置文件将被写入二叉树，我们必须将该目录添加到搜索包含文件的路径列表中。将以下行添加到CMakelists的末尾。

```txt
target_include_directories(Tutorial PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )
```

在源目录下创建 TutorialConfig.h.in 文件。

```txt
// the configured options and settings for Tutorial
#define Tutorial_VERSION_MAJOR @Tutorial_VERSION_MAJOR@
#define Tutorial_VERSION_MINOR @Tutorial_VERSION_MINOR@
```

当CMake配置这个头文件时，@Tutorial_VERSION_MAJOR@ 和 @Tutorial_VERSION_MINOR@ 的值将被替换。

修改包含配置头文件 (TutorialConfig.h) 的 tutorial.cxx 。

通过更新 tutorial.cxx 打印出可执行文件的名称和版本号。

<table>
    <tr>
        <td>tutorial.cxx</td>
    </tr>
        <tr>
        <td>
```c++
if (argc < 2) {
    // report version
    std::cout << argv[0] << " Version " << Tutorial_VERSION_MAJOR << "."
              << Tutorial_VERSION_MINOR << std::endl;
    std::cout << "Usage: " << argv[0] << " number" << std::endl;
    return 1;
}
```
       	</td>
    </tr>
</table>

```c++
if (argc < 2) {
    // report version
    std::cout << argv[0] << " Version " << Tutorial_VERSION_MAJOR << "."
              << Tutorial_VERSION_MINOR << std::endl;
    std::cout << "Usage: " << argv[0] << " number" << std::endl;
    return 1;
}
```



```

```

