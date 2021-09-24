# 一. CMake中常用预定义变量

## 1. CMake的预定义变量

- PROJECT_SOURCE_DIR：工程根目录；
- PROJECT_BINARY_DIR：运行cmake命令的目录。笔者建议定义为${PROJECT_SOURCE_DIR}/build下。具体原因见后文外部编译部分；
- CMAKE_INCLUDE_PATH：环境变量，非cmake变量；
- CMAKE_LIBRARY_PATH：环境变量；
- CMAKE_CURRENT_SOURCE_DIR：当前处理的CMakeLists.txt文件所在路径；
- CMAKE_CURRENT_BINARY_DIR：target编译目录；
  - 使用ADD_SURDIRECTORY指令可以更改该变量的值；
  - SET(EXECUTABLE_OUTPUT_PATH < dir >) 指令不会对该变量有影响，但改变了最终目标文件的存储路径；
- CMAKE_CURRENT_LIST_FILE：输出调用该变量的CMakeLists.txt的完整路径；
- CMAKE_CURRENT_LIST_LINE：输出该变量所在的行；
- CMAKE_MODULE_PATH：定义自己的cmake模块所在路径；
- EXECUTABLE_OUTPUT_PATH：重新定义目标二进制可执行文件的存放位置；
- LIBRARY_OUTPUT_PATH：重新定义目标链接库文件的存放位置；
- PROJECT_NAME：返回由PROJECT指令定义的项目名称；
- CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS：用来控制IF...ELSE...语句的书写方式；

## 2. 系统信息预定义变量

- CMAKE_MAJOR_VERSION cmake主版本号,如2.8.6中的2
- CMAKE_MINOR_VERSION cmake次版本号,如2.8.6中的8
- CMAKE_PATCH_VERSION cmake补丁等级,如2.8.6中的6
- CMAKE_SYSTEM 系统名称,例如Linux-2.6.22
- CMAKE_SYSTEM_NAME 不包含版本的系统名,如Linux
- CMAKE_SYSTEM_VERSION 系统版本,如2.6.22
- CMAKE_SYSTEM_PROCESSOR 处理器名称,如i686
- UNIX 在所有的类UNIX平台为TRUE,包括OS X和cygwin
- WIN32 在所有的win32平台为TRUE,包括cygwin

## 3. 开关选项

- BUILD_SHARED_LIBS 控制默认的库编译方式。
  - 注：如果未进行设置,使用ADD_LIBRARY时又没有指定库类型,默认编译生成的库都是静态库。
- CMAKE_C_FLAGS 设置C编译选项
- CMAKE_CXX_FLAGS 设置C++编译选项

# 二. CMake常用语法

## 1. CMAKE_MINIMUM_REQUIRED

该语句一般都可以放置在CMakeLists.txt的开头，用于说明CMake最低版本要求。
这行命令是可选的，我们可以不写这句话，但在有些情况下，如果CMakeLists.txt文件中使用了一些高版本cmake特有的一些命令的时候，就需要加上这样一行，提醒用户升级到该版本之后再执行cmake。

 

```
cmake_minimum_required (VERSION 2.6)
```

上述例程指cmake的最低版本至少为2.6。

## 2. PROJECT

格式：

 

```
PROJECT(name)
```

- name：工程名称；

该指令一般置于CMakeLists.txt的开头，定义了工程的名称。但项目最终编译生成的可执行文件并不一定是这个项目名称，而是由另一条命令确定的，稍候我们再介绍。
执行了该条指令之后，将会自动创建两个变量：

- < projectname >_BINARY_DIR：二进制文件保存路径；
- < projectname >_SOURCE_DIR：源代码路径；

 

```
project(CRNode)
```

执行了上一条指令，即定义了一个项目名称CRNode，相应的会生成两个变量：CRNode_BINARY_DIR, CRNode_SOURCE_DIR。
cmake中预定义了两个变量：PROJECT_BINARY_DIR与PROJECT_SOURCE_DIR。
在这个例子中：
PROJECT_BINARY_DIR = CRNode_BINARY_DIR
PROJECT_SOURCE_DIR = CRNode_SOURCE_DIR
笔者强烈推荐直接使用PROJECT_BINARY_DIR与PROJECT_SOURCE_DIR，这样及时项目名称发生了变化，也不会影响CMakeLists.txt文件。
关于上面两个变量是否相同的问题，涉及到编译方法是内部编译还是外部编译。如果是内部编译，则上面两个变量相同；如果是外部编译，则两个变量不同。此处对内部编译与外部编译做出介绍：

### (1) 外部构建与内部构建

假设此时已经完成了CMakeLists.txt的编写，在CMakeLists.txt所在目录下，有两种执行cmake的方法：

 

```
cmake ./make
```

以及：

 

```
mkdir build
cd ./build
cmake ../
make
```

第一种方法是内部构建，第二种方法是外部构建。上述两种方法中，最大不同在于cmake与make的工作路径不同。
内部构建方法中，cmake生成的中间文件和可执行文件都会存放在项目目录中；外部构建方法中，中间文件与可执行文件都存放在build目录中。
笔者强烈建议使用外部构建方法。优点显而易见：最大限度的保持了代码目录的整洁，生成、编译与安装是不同于项目目录的其他目录中，在外部构建方法下，PROJECT_SOURCE_DIR指向目录与内部构建相同，为CMakeLists.txt所在根目录；而PROJECT_BINARY_DIR不同，它指向CMakeLists.txt所在根目录下的build目录。

## 3. SET

格式：

 

```
SET(VAR [VALUE] [CACHE TYPEDOCSTRING [FORCE]])
```

例：

 

```
SET(CMAKE_INSTALL_PREFIX /usr/local)
```

该例程中，我们显式的将CMAKE_INSTALL_PREFIX的值定义为/usr/local，如此在外部构建情况下执行make install命令时，make会将生成的可执行文件拷贝到/usr/local/bin目录下。
当然，可执行文件的安装路径CMAKE_INSTALL_PREFIX也可以在执行cmake命令的时候指定，cmake参数如下：

 

```
cmake -DCMAKE_INSTALL_PREFIX=/usr ..
```

如果cmake参数和CMakeLists.txt文件中都不指定该值的话，则该值为默认的/usr/local。

## 4. ADD_SUBDIRECTORY

格式：

 

```
ADD_SUBDIRECTORY(source_dir [binary_dir] [EXCLUDE_FROM_ALL])
```

- source_dir：源文件路径；
- [binary_dir]：中间二进制与目标二进制存放路径；
- [EXECLUDE_FROM_ALL]：将这个目录从编译过程中排除；

这个指令用于向当前工程添加存放源文件的子目录，并可以指定中间二进制和目标二进制存放的位置。EXCLUDE_FROM_ALL 参数的含义是将这个目录从编译过程中排除。比如，工程有时候存在example，可能就需要工程构建完成后，再进入example目录单独进行构建。

## 5. INCLUDE_DIRECTORIES

格式：

 

```
INCLUDE_DIRECTORIES([AFTER|BEFORE] [SYSTEM] dir1 dir2 ...)
```

- [AFTER|BEFORE]：追加标志，指定控制追加或置前；
- [SYSTEM]：（笔者也不知道干嘛用的）
- dir1, ..., dir n：添加的一系列头文件搜索路径；

向工程添加多个特定的头文件搜索路径，路径之间用空格分隔。类似于gcc中的编译参数<code>-l</code>，即指定编译过程中编译器搜索头文件的路径。当项目需要的头文件不在系统默认的搜索路径时，则指定该路径。
AFTER/BEFORE参数，控制追加或置前。默认情况下，追加当前头文件搜索路径的后面。
注：如果路径包含空格，可以使用双引号将它括起来。
例：

 

```
INCLUDE_DIRECTORIES(/usr/include/thrift)
```

## 6. ADD_EXECUTABLE

格式：

 

```
ADD_EXECUTABLE(exename srcname)
```

- exename：可执行文件名
- srcname：生成该可执行文件的源文件

该命令给出源文件名称，并指出需要编译出的可执行文件名。

例1：

 

```
ADD_EXECUTABLE(hello ${SRC_LIST})
```

上述例程说明SRC_LIST变量中的源文件，需要编译出名为hello的可执行文件。

例2：

 

```
SET(SRC_LIST main.cc        
	rpc/CRNode.cpp         
	rpc/Schd_types.cpp         
	task/TaskExecutor.cpp        
	task/TaskMoniter.cpp        
	util/Const.cpp         
	util/Globals.cc        
	) 
ADD_EXECUTABLE(CRNode ${SRC_LIST})
```

该例程中，定义了该工程会生成一个名为CRNode的可执行文件，所依赖的源文件是变量SRC_LIST定义的源文件列表。
注：如果前文PROJECT()指令中定义的项目名称也定义为CRNode，也没有什么问题，两者之间没有任何关系。

## 7. ADD_LIBRARY

格式：

 

```
ADD_LIBRARY(libname [SHARED|STATIC|MODULE] [EXCLUDE_FROM_ALL] source1 source2 ... sourceN)
```

- libname：库文件名称；
- [SHARED|STATIC|MODULE]：生成库文件类型（共享库/静态库）
- [EXCLUDE_FROM_ALL]：表示该库不会被默认构建
- source1, ..., sourceN：生成库所依赖的源文件

例：

 

```
ADD_LIBRARY(hello SHARED ${LIBHELLO_SRC})
```

## 8. 变量EXECUTABLE_OUTPUT_PATH, LIBRARY_OUTPUT_PATH

EXECUTABLE_OUTPUT_PATH为生成可执行文件路径，LIBRARY_OUTPUT_PATH为生成库文件路径。
我们可以通过SET指令对其进行设置最终的目标二进制的位置，即最终生成的工程可执行文件与最终的共享库，而不包含编译生成的中间文件。
命令如下：

 

```
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
```

注：指令ADD_EXECUTABLE, ADD_LIBRARY出现的位置，如果需要改变目标存放路径，就在该处添加上述定义。

## 9. LINK_DIRECTORIES

格式：

 

```
LINK_DIRECTORIES(directory1 directory2 ...)
```

该指令用于添加外部库的搜索路径。

## 10. TARGET_LINK_LIBRARIES

格式：

 

```
TARGET_LINK_LIBRARIES(target library1 <debug | optimized> library2 ..)
```

- target：目标文件；
- library1, ..., libraryN：链接外部库文件；

指定链接目标文件时需要链接的外部库，效果类似于gcc编译参数<code>-L</code>，解决外部库依赖的问题。

## 11. MESSAGE

向终端输出用户定义的信息或变量值；
格式：

 

```
MESSAGE([SEND_ERROR | STATUS | FATAL_ERROR] “message to display” …)
```

- SEND_ERROR：产生错误，生成过程被跳过；
- STATUS：输出前缀为 -- 的信息；
- FATAL_ERROR：立即终止所有cmake过程；

## 12. SET_TARGET_PROPERTIES

设置目标的某些属性，改变它们构建的方式。
格式：

 

```
SET_TARGET_PROPERTIES(target1 target2 ...            PROPERTIES prop1 value1 prop2 value2 ...)
```

该指令为一个目标设置属性，语法是列出所有用户想要变更的文件，然后提供想要设置的值。用户可以使用任何想用的属性与对应的值，并在随后的代码中调用GET_TARGET_PROPERTY命令取出属性的值。
影响目标输出文件的属性<code>PROPERTIES</code>详述如下：

(1) PREFIX, SUFFIX

PREFIX覆盖了默认的目标名前缀（如lib）；
SUFFIX覆盖了默认的目标名后缀（如.so）。

(2) IMPORT_PREFIX, IMPORT_PREFIX

与PREFIX, SUFFIX是等价的属性，但针对的是DLL导入库（即共享库目标）。

(3) OUTPUT_NAME

构建目标时，OUTPUT_NAME用来设置目标的真实名称。

(4) LINK_FLAGS

为一个目标的链接阶段添加额外标志。
LINK_FLAGS_< CONFIG >将为配置< CONFIG >添加链接标志，如DEBUG, RELEASE, MINSIZEREL, RELWITHDEBINFO。

(5) COMPILE_FLAGS

设置附加的编译器标志，在构建目标内的源文件时被用到。

(6) LINKER_LANGUAGE

改变链接可执行文件或共享库的工具。默认值是设置与库中文件相匹配的语言。
CXX与C是该属性的公共值。

(7) VERSION, SOVERSION

VERSION指定构建的版本号，SOVERSION指定构建的API版本号。
构建或安装时，如果平台支持符号链接，且链接器支持so名称，那么恰当的符号链接将会被创建。如果只指定两者中的一个，缺失的另一个假定为具有相同版本号。
例1：

 

```
SET_TARGET_PROPERTIES(hello_static PROPERTIES OUTPUT_NAME "hello")
```

例2：

 

```
SET_TARGET_PROPERTIES(hello PROPERTEIES VERSION 1.2 SOVERSION 1)
```

该指令用于控制版本，VERSION指代动态库版本，SOVERSION指代API版本。

## 13. AUX_SOURCE_DIRECTORY

查找某个路径下的所有源文件，并将源文件列表存储到一个变量中。
格式：

 

```
AUX_SOURCE_DIRECTORY(< dir > < variable >)
```

例：

 

```
AUX_SOURCE_DIRECTORY(. SRC_LIST)
```

该指令将当前目录下的文件列表全部存入变量SRC_LIST中。

## 14. INSTALL

INSTALL命令可以按照对象的不同分为三种类型：目标文件、非目标文件、目录；

### (1) 目标文件：

格式：

 

```
INSTALL(TARGETS targets...    [[ARCHIVE|LIBRARY|RUNTIME]    [DESTINATION < dir >]    [PERMISSIONS permissions...]    [CONFIGURATIONS    [Debug|Release|...]]    [COMPONENT < component >]    [OPTIONAL]    ] [...])
```

- TARGETS targets：targets即为我们通过ADD_EXECUTABLE或ADD_LIBRARY定义的目标文件，可能是可执行二进制，动态库，静态库；
- DESTINATION < dir >：dir即为定义的安装路径。安装路径可以是绝对/相对路径，若如果路径以/开头，则是绝对路径，且绝对路径的情况下，CMAKE_INSTALL_PREFIX就无效了。
  - 注：如果希望使用CMAKE_INSTALL_PREFIX定义安装路径，就需要使用相对路径，这时候安装后的路径就是<code>${CMAKE_INSTALL_PREFIX}/< dir ></code>

其余参数待笔者使用到再进行补充吧……

### (2) 非目标文件：

.sh脚本文件，即为典型的非目标文件的可执行程序。
格式：

 

```
INSTALL(PROGRAMS files... DESTINATION < dir >    [PERMISSIONS permissions...]    [CONFIGURATIONS [Debug|Release|...]]    [COMPONENT < component >]    [RENAME < name >] [OPTIONAL])
```

使用方法基本和上述目标文件指令的INSTALL相同，唯一别的不同是，安装非目标文件之后的权限为OWNER_EXECUTE, GOUP_EXECUTE, WORLD_EXECUTE，即755权限目录的安装。

### (3) 目录：

格式：

 

```
INSTALL(DIRECTORY dirs... DESTINATION < dir >    [FILE_PERMISSIONS permissions...]    [DIRECTORY_PERMISSIONS permissions...]    [USE_SOURCE_PERMISSIONS]    [CONFIGURATIONS [Debug|Release|...]]    [COMPONENT < component >]    [[PATTERN < pattern > | REGEX < regex >]    [EXCLUDE] [PERMISSIONS permissions...]] [...])
```

- DIRECTORY dirs：dirs是所在源文件目录的相对路径。但必须注意：abc与abc/有很大区别：
  - 若是abc，则该目录将被安装为目标路径的abc；
  - 若是abc/，则代表将该目录内容安装到目标路径，但不包括该目录本身。

例：

 

```
INSTALL(DIRECTORY icons scripts/ DESTINATION share/myproj    PATTERN "CVS" EXCLUDE    PATTERN "scripts/*" PERMISSIONS OWNER_EXECUTE OWNER_WRITE OWNER_READ GROUP_EXECUTE GROUP_READ)
```

该指令的执行结果是：

- 将icons目录安装到< prefix >/share/myproj；
- 将scripts/中的内容安装到< prefix >/share/myproj；
- 不包含目录名为CVS的目录；
- 对于scripts/*文件指定权限为OWNER_EXECUTE, OWNER_WRITE, OWNER_READ, GROUP_EXECUT, GROUP_READ；

# 三. 基本控制语法

## 1. IF

IF...ELSE...语法格式基本类似与C语言，大致如下：

 

```
IF (expression)    COMMAND1(ARGS ...)    COMMAND2(ARGS ...)    ...ELSE (expression)    COMMAND1(ARGS ...)    COMMAND2(ARGS ...)    ...ENDIF (expression)
```

其中，一定要有ENDIF与IF对应。

### (1) IF基本用法：

- IF (expression), expression不为：空,0,N,NO,OFF,FALSE,NOTFOUND或< var >_NOTFOUND,为真；
- IF (not exp), 与上面相反；
- IF (var1 AND var2)
- IF (var1 OR var2)
- IF (COMMAND cmd) 如果cmd确实是命令并可调用，为真；
- IF (EXISTS dir) 如果目录存在，为真；
- IF (EXISTS file) 如果文件存在，为真；
- IF (file1 IS_NEWER_THAN file2)，当file1比file2新，或file1/file2中有一个不存在时为真，文件名需使用全路径；
- IF (IS_DIRECTORY dir) 当dir是目录时，为真；
- IF (DEFINED var) 如果变量被定义，为真；
- IF (var MATCHES regex) 此处var可以用var名，也可以用${var}；
- IF (string MATCHES regex) 当给定变量或字符串能匹配正则表达式regex时，为真；
  - 例：
  - <code>IF ("hello" MATCHES "ell")
    MESSAGE("true")
    ENDIF ("hello" MATCHES "ell")
    </code>

### (2) 数字比较表达式

- IF (var LESS number)
- IF (var GREATER number)
- IF (var EQUAL number)

### (3) 字母表顺序比较

- IF (var1 STRLESS var2)
- IF (var1 STRGREATER var2)
- IF (var1 STREQUAL var2)

例1：
判断平台差异：

 

```
IF(WIN32)    MESSAGE(STATUS "This is windows.")ELSE(WIN32)    MESSAGE(STATUS "This is not windows.")ENDIF(WIN32)
```

上述代码可以控制不同平台进行不同控制。

注：也许ELSE(WIN32)之类的语句阅读起来很不舒服，这时候可以加上语句：

 

```
SET(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS ON)
```

这时候上述结构就可以写成：

 

```
IF(WIN32)ELSE()ENDIF()
```

例2：
配合ELSEIF使用，不同平台上的控制：

 

```
IF(WIN32)    #do something related to WIN32ELSEIF(UNIX)    #do something related to UNIXELSEIF(APPLE)    #do something related to APPLEENDIF (WIN32)
```

## 2. WHILE

语法结构如下：

 

```
WHILE(condition)    COMMAND1(ARGS ...)    COMMAND2(ARGS ...)    ...ENDWHILE(condition)
```

真假判断条件可以参考IF指令。

## 3. FOREACH

FOREACH有三种使用形式的语法，且每个FOREACH都需要一个ENDFOREACH()与之匹配。

### (1) 列表语法

 

```
FOREACH(loop_var arg1 arg2 ...)     COMMAND1(ARGS ...)     COMMAND2(ARGS ...) ...ENDFOREACH(loop_var)
```

例：

 

```
AUX_SOURCE_DIRECTORY(. SRC_LIST)FOREACH(F ${SRC_LIST})     MESSAGE(${F})ENDFOREACH(F)
```

该例程中，现将当前路径下的所有源文件列表赋值给变量SRC_LIST，然后遍历SRC_LIST中的文件，并持续输出信息，信息内容是当前路径下所有源文件的名称。

### (2) 范围语法

 

```
FOREACH(loop_var RANGE total)    COMMAND1(ARGS ...)    COMMAND2(ARGS ...)    ...ENDFOREACH(loop_var)
```

例：

 

```
FOREACH(VAR RANGE 10)   MESSAGE(${VAR})ENDFOREACH(VAR)
```

该例程从0到total（此处为10），以1为步进。此处输出为：012345678910

### (3) 范围步进语法

 

```
FOREACH(loop_var RANGE start stop [step])    COMMAND1(ARGS ...)    COMMAND2(ARGS ...)    ...ENDFOREACH(loop_var)
```

从start开始，到stop结束，以step为步进。
例：

 

```
FOREACH(A RANGE 5 15 3)    MESSAGE(${A})ENDFOREACH(A)
```

此处输出为581114