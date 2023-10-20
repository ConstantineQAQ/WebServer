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

// 容器偏特化，支持vector,list,set,unordered_set
// map,unordered_map 支持key = std::string的偏特化
// Config::Lookup(key) , key相同，但是类型不同的，不会报错，这是一个问题
```

自定义类型，需要实现ConstantineQAQ::LexicalCast<T, std::string>的偏特化，实现后，就可以支持Config解析自定义类型，自定义类型可以和常规STL容器一起使用

配置的事件机制
当一个配置项发生修改的时候，可以反向通知对应的代码，回调

### 日志系统整合配置系统
![顶层类图](/resources/UML图.png) 
```yaml
logs:
    - name: root 
    - level: debug (debug,info,warn,error,fatal)
    - formatter: "%d%T%p%T%t%m%n"
    - appender:
        - type: (StdoutLogAppender, FileLogAppender)
          level: (debug,...)
          file: /logs/xxx.log
```

```cpp
// 解决动态绑定日志器的问题
ConstantineQAQ::Logger g_logger = 
ConstantineQAQ::LoggerMgr::GetInstance()->getLogger(name);
CONSTANTINEQAQ_LOG_INFO(g_logger) << "xxx log";
```

```cpp
static Logger::ptr g_log = CONSTANTINEQAQ_LOG_NAME("system");
// m_root, m_system没东西时还是用m_root写logger，当m_system有东西时就会换成新的值写logger。
```

```cpp
// 定义LogDefine LogAppenderDefine，偏特化LexicalCast，
// 实现了日志解析
```

使用processon画图，画出类的关系图，类的关系图，类的继承关系图，类的组合关系图，类的依赖关系图

## 3.线程库

Thread, Mutex
Pthread

pthread pthread_create

互斥量 mutex
信号量 semaphore

和log整合
Logger, Appender, Formatter

Spinlock,替换了Mutex
写文件，周期性reopen，防止当文件被删除，占用句柄，磁盘空间没有释放。

## 4.协程库的封装
将异步的操作封装成协程，使用协程的方式来进行异步操作

## 5.socket函数库的开发

## 6.http协议的开发

## 7.分布式协议
逻辑业务分离，将业务逻辑分离到不同的服务器上，通过分布式协议来进行通信，将业务功能和系统功能分割开来，提高系统的可扩展性和可维护性
1
## 8.推荐系统