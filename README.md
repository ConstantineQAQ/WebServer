# ConstantineQAQ的WebServer学习笔记

## 开发环境
Centos7 \
gcc 9.3.1 \
cmake

## 项目路径
bin -- 二进制 \
build -- cmake生成的中间文件 \
cmake -- cmake函数文件夹 \
CMakeLists.txt -- cmake的定义文件 \
lib -- 库文件 \
Makefile \
src -- 源代码 \
tests -- 测试代码

## 1.日志系统
(1) 仿照Log4j

    Logger（定义日志类别）
       |
       |---------Formatter(日志格式)
       |
    Appender(日志输出位置)

## 2.协程库的封装
将异步的操作封装成协程，使用协程的方式来进行异步操作

## 3.socket函数库的开发

## 4.http协议的开发

## 5.分布式协议
逻辑业务分离，将业务逻辑分离到不同的服务器上，通过分布式协议来进行通信，将业务功能和系统功能分割开来，提高系统的可扩展性和可维护性

## 6.推荐系统