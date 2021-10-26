## wildcard用法

一般使用通配符会自动展开，但在变量定义和函数引用的时候，通配符失效。这时就可以使用`wildcard`

用法： `$(wildcard PATTERN...)`

展开位已存在的，使用空格隔开，匹配模式下所有文件列表。如果不存在任何符合此模式的文件，函数会忽略模式字符并返回空。

扩展使用：（依据列表目录下`.C`文件列表，得到生成的`.O`文件列表）

`$(patsubst %.c,%.o,$(wildcard *.c))`

例：

```makefile
objects := $(patsubst %.c,%.o,$(wildcard *.c))

foo : $(objects)

cc -o foo $(objects)

```

wildcard : 扩展通配符

notdir ： 去除路径

patsubst ：替换通配符

替换引用规则，用指定的变量替换另一个变量

标准格式：

$(var:a=b) 或 ${var:a=b}
它的含义是把变量var中的每一个值结尾用b替换掉a
