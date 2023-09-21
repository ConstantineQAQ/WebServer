#include "../include/constantineQAQ.h"
#include <yaml-cpp/yaml.h>
#include <unistd.h>

ConstantineQAQ::Logger::ptr g_logger = CONSTANTINEQAQ_LOG_ROOT();

volatile int count = 0;
// ConstantineQAQ::RWMutex s_mutex;
ConstantineQAQ::Mutex s_mutex;

void fun1() {
    CONSTANTINEQAQ_LOG_INFO(g_logger) << "name=" << ConstantineQAQ::Thread::GetName()
        << " this.name=" << ConstantineQAQ::Thread::GetThis()->getName()
        << " id=" << ConstantineQAQ::GetThreadId()
        << " this.id=" << ConstantineQAQ::Thread::GetThis()->getId();
    for(int i = 0;i < 1000000;++i) {
        // ConstantineQAQ::RWMutex::WriteLock lock(s_mutex);
        ConstantineQAQ::Mutex::Lock lock(s_mutex);
        ++count;
    }
}

void fun2() {
    while(1) {
        CONSTANTINEQAQ_LOG_INFO(g_logger) << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
    }
}

void fun3() {
    while(1) {
        CONSTANTINEQAQ_LOG_INFO(g_logger) << "=========================================";
    }
}

int main(int argc,char** argv) {
    CONSTANTINEQAQ_LOG_INFO(g_logger) << "thread test begin";
    YAML::Node root = YAML::LoadFile("/home/ConstantineQAQ/workspace/webserver/bin/conf/log2.yml");
    ConstantineQAQ::Config::LoadFromYaml(root);

    std::vector<ConstantineQAQ::Thread::ptr> thrs;
    for(int i = 0;i < 2;++i) {
        ConstantineQAQ::Thread::ptr thr(new ConstantineQAQ::Thread(&fun2, "name_" + std::to_string(i * 2)));
        // ConstantineQAQ::Thread::ptr thr2(new ConstantineQAQ::Thread(&fun3, "name_" + std::to_string(i * 2 + 1)));
        thrs.push_back(thr);
        // thrs.push_back(thr2);
    }

    for(size_t i = 0;i < thrs.size();++i)  {
        thrs[i]->join();
    }
    CONSTANTINEQAQ_LOG_INFO(g_logger) << "thread test end";
    CONSTANTINEQAQ_LOG_INFO(g_logger) << "count=" << count;
    return 0;
}