# Minilog
Minilog是一个使用C++11开发的日志系统。

## Minilog的使用
Minilog的使用包括初始化日志文件以及记录。
```c++
minilog::initialize(minilog::GuaranteedLogger(), "/somewhere/", "Minilog", 1);
```
初始化完成之后，你可以通过Minilog提供的写入API对程序进行日志记录

*Minilog提供了C宏风格的API：*
1. `LOG_INFO`
2. `LOG_WARN`
3. `LOG_CRIT`

这三种API分别对应不同程度的记录级别

Minilog使用GMT时间

## 关于Minilog的优点

因为格式化操作是一个比较耗时的操作，因此，Minilog使用线程来进行日志写入，为了保证程序执行与写线程之间的配合，Minilog使用了缓冲区，
事先将生成的MinilogLine日志记录先写入一个缓冲区里头，然后让线程去从缓冲区里头读取然后写入文件里头。

Minilog提供两种写入，

一种是有保证的写入，但是，由于需要使用缓冲区进行记录，它将所有记录都存放到队列缓冲区里头，因此如果记录数目多的时候，会有性能影响；

另一种是无保证的写入，这是用环形缓冲区来实现的，它的大小可以指定，但是一旦记录的长度超过了环形缓冲区的大小，就会覆盖掉原有记录，
这可以简单通过取余实现。

## Benchmark测试
使用`Google Benchmark`进行测试，单线程，写入50000条测试的执行结果

```c++
Run on (8 X 1400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x4)
  L1 Instruction 32 KiB (x4)
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB (x1)
Load Average: 1.78, 1.99, 2.23
----------------------------------------------------------
Benchmark                Time             CPU   Iterations
----------------------------------------------------------
BM_logwrite     273337 ns       234771 ns         3066
```
spdlog写入50000条测试的结果。spdlog文件较大，构建的时也明显更多
```
Run on (8 X 1400 MHz CPU s)
CPU Caches:
  L1 Data 32 KiB (x4)
  L1 Instruction 32 KiB (x4)
  L2 Unified 256 KiB (x4)
  L3 Unified 6144 KiB (x1)
Load Average: 2.32, 2.03, 1.91
-------------------------------------------------------------
Benchmark                   Time             CPU   Iterations
-------------------------------------------------------------
BM_spdlog_fileinfo   20522389 ns     15318189 ns           53
```
