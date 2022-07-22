<!--
 * @Author: user8361
 * @Date: 2022-06-28 21:02:47
 * @LastEditors: user8361
 * @LastEditTime: 2022-07-17 22:33:42
 * @FilePath: /projects/HLog/README.md
 * @Description: 
 * 
 * Copyright (c) 2022 by user8361-AHU, All Rights Reserved. 
-->
# HLog
>实现sylar高性能服务器中的日志模块
log日志器

# 使用自旋锁 https://blog.csdn.net/vincent_wen0766/article/details/108558656
自旋锁：自我旋转，这里旋转指循环 \
自旋锁不会放弃cpu时间片，通过自旋等待锁实释放 \

非自旋锁：若获取不到锁，就将线程休眠，切换线程状态为阻塞态，cpu处理其他任务 \

## 自旋锁的好处
阻塞和唤醒线程都是需要高昂开销。
自旋锁用循环去不停尝试获取锁，让线程始终处于 运行 状态，节省线程切换带来的开销


# 循环引用的解决
（相互引用的两个类，其中至少一个是使用的引用，而不能使用实体）
- 一个类可以多次声明，但是只能定义一次
- 一个类A的声明中可以使用一个只声明未定义的类B，但是只能使用B的指针或者引用
- 若类A的函数要使用类B的函数，则类B的函数必须已经定义好而不能只是声明

# 关于 stringstream 被遗弃之后的使用
```
error: use of deleted function ‘std::__cxx11::basic_stringstream<_CharT, _Traits, _Alloc>::basic_stringstream(const std::__cxx11::basic_stringstream<_CharT, _Traits, _Alloc>&) [with _CharT = char; _Traits = std::char_traits<char>; _Alloc = std::allocator<char>]’
```
禁用拷贝构造和赋值构造。
使用指针传回 stringstream对象

# 使用yaml库 需要在CMakeLists.txt中添加依赖库


# define 中#和##的使用
> https://www.cnblogs.com/talenth/p/5820100.html
```
# ： 使得#后面的首个参数返回为一个带引号的字符串
## ：把两个符号连接起来

```

# 继承关系中，构造函数问题
父类未显式提供构造函数时，可能报错： error: use of deleted function.


# 协程的使用
> https://blog.csdn.net/ThinPikachu/article/details/121325198
## 上下文切换
- 进程：从 【用户态】--> 【内核态】 --> 【用户态】
- 线程：从 【用户态】--> 【内核态】 --> 【用户态】
- 协程：只涉及 【用户态】 --> 【用户态】的切换
协程上下文切换只涉及 : cpu上下文（包含少量的 寄存器值的修改）

- 协程切换：将当前协程的cpu寄存器状态保存下来，将需要切换进来的协程的cpu寄存器状态加载到cpu寄存器上

## 大量的进程、线程带来的问题：
- 系统线程会占用非常多的内存空间
- 过多的线程切换会占用大量的系统时间
协程恰好可以解决上面两个问题。
**协程运行在线程之上，一个协程运行结束可以主动让出，让另一个协程运行在当前线程之上 。协程没有增加线程数量，只是在线程的基础上通过分时复用的方式运行多个协程。协程的切换是在用户态完成的，切换代价较小。**




## 前向声明和引用
> https://blog.csdn.net/marcosun_sw/article/details/84376068
使用C++编程，编译时经常出现这种错误"error: invalid use of incomplete type ‘class xxx’"，或“error: forward declaration of ‘class xxx’”. 解决这种错误就是用理解c++中前向声明与引用头文件的区别。

### 前置声明的使用场景
- 只能使用于指针\引用\函数形参\函数返回值 
- 若用于类内部的成员变量的声明,类的继承列表,模板的参数 等都是**非法**的
  
### 总结
- 原则1: 若可以不包含头文件,就不要包含,使用前置声明可以解决问题.
  - 解释:若使用的仅仅是一个类的指针,没有使用这个指针的具体对象(非指针),也没有访问类的具体成员,那么前置声明就可以了.
    - PS:指针的大小固定,编译器可以获知
- 原则2: 尽量在源文件(.cpp文件)中包含头文件,而非在头文件中.
  - 假设: 类A的一个成员是 类B的一个指针,在A的头文件中,使用类B的前置声明即可.在A的实现中,需要访问B的成员,那么在A.cpp中包含B的头文件.


# #define 中使用 if条件语句报错
error: expected unqualified-id before ‘if’ (使用宏的时候加了作用域，所以一致导致出错)

# 遇到的错误
> https://blog.csdn.net/oaix101125/article/details/79016705
- 获取到stringstream后需要解引用，方可传入字符串数据

## gdb 调试段错误

- 使用 ulimit -c 查看core file 的大小:默认为0，即不生成 core 文件
- 修改大小： ulimit -c unlimited ，之后出现 Segmentation fault(core dumped)错误，会生成 core 文件。
- gdb ./HLOG core进行调试
- where 查看段错误的地方
![image.png](https://p3-juejin.byteimg.com/tos-cn-i-k3u1fbpfcp/112ef992e6524478afb677cf3d2c79de~tplv-k3u1fbpfcp-watermark.image?)
- 定位发现，只声明了智能指针而没有分配内存就使用了，所以出现了段错误。
  - std::shared_ptr<Logger> logger; --> std::shared_ptr<Logger> logger(new Logger);


#  undefined reference to `vtable for 
- 检查使用的类的基础关系上是否有没有实现的虚函数
- 检查使用的类的库是否被链接