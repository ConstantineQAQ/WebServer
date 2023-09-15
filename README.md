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

## 2.配置系统
Config --> Yaml

Yaml-cpp : github.com/jbeder/yaml-cpp

```cpp
YAML::Node node = YAML::LoadFile("config.yaml");
node.IsMap();
for(auto it = node.begin(); it != node.end(); ++it) {
}

node.IsSequence();
for(size_t i = 0; i < node.size(); ++i) {
}

node.IsScalar();
```
配置系统的原则，约定优于配置，配置文件的格式要简单，易于阅读，易于修改

```cpp
template<T, FromStr, ToStr>
class ConfigVar;

template<F,T>
LexicalCast;

// 容器片特化，支持vector,list,set,unordered_set
// map,unordered_map 支持key = std::string的片特化
// Config::Lookup(key) , key相同，但是类型不同的，不会报错，这是一个问题
```

自定义类型，需要实现ConstantineQAQ::LexicalCast<T, std::string>的片特化，实现后，就可以支持Config解析自定义类型，自定义类型可以和常规STL容器一起使用

配置的事件机制
当一个配置项发生修改的时候，可以反向通知对应的代码，回调

## 3.协程库的封装
将异步的操作封装成协程，使用协程的方式来进行异步操作

## 4.socket函数库的开发

## 5.http协议的开发

## 6.分布式协议
逻辑业务分离，将业务逻辑分离到不同的服务器上，通过分布式协议来进行通信，将业务功能和系统功能分割开来，提高系统的可扩展性和可维护性

## 7.推荐系统