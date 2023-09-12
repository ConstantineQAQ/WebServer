#include "../include/log.h"
#include "../include/config.h"
#include <yaml-cpp/yaml.h>

ConstantineQAQ::ConfigVar<int>::ptr g_int_value_config = 
    ConstantineQAQ::Config::Lookup("system.port", (int)8080, "system port");

ConstantineQAQ::ConfigVar<float>::ptr g_float_value_config = 
    ConstantineQAQ::Config::Lookup("system.value", (float)10.2f, "system value");

ConstantineQAQ::ConfigVar<std::vector<int>>::ptr g_int_vec_value_config = 
    ConstantineQAQ::Config::Lookup("system.int_vec", std::vector<int>{1,2}, "system int vec");

void print_yaml(const YAML::Node& node, int level) {
    if(node.IsScalar()){
        CONSTANTINEQAQ_LOG_INFO(CONSTANTINEQAQ_LOG_ROOT()) << std::string(level*4, ' ') << node.Scalar() << " - " << node.Type() << " - " << level;
    }else if(node.IsNull()){
        CONSTANTINEQAQ_LOG_INFO(CONSTANTINEQAQ_LOG_ROOT()) << std::string(level*4, ' ') << "NULL - " << node.Type() << " - " << level;
    }else if(node.IsMap()){
        for(auto it = node.begin(); it != node.end(); ++it){
            CONSTANTINEQAQ_LOG_INFO(CONSTANTINEQAQ_LOG_ROOT()) << std::string(level*4, ' ') << it->first << " - " << it->second.Type() << " - " << level;
            print_yaml(it->second, level + 1);
        }
    }else if(node.IsSequence()){
        for(size_t i = 0; i < node.size(); ++i){
            CONSTANTINEQAQ_LOG_INFO(CONSTANTINEQAQ_LOG_ROOT()) << std::string(level*4, ' ') << i << " - " << node[i].Type() << " - " << level;
            print_yaml(node[i], level + 1);
        }
    }
}

void test_yaml(){
    YAML::Node root = YAML::LoadFile("/home/ConstantineQAQ/workspace/webserver/bin/conf/log.yml");
    print_yaml(root,0);
}

void test_config(){
    CONSTANTINEQAQ_LOG_INFO(CONSTANTINEQAQ_LOG_ROOT()) << "before: " << g_int_value_config->getValue();
    CONSTANTINEQAQ_LOG_INFO(CONSTANTINEQAQ_LOG_ROOT()) << "before: " << g_float_value_config->getValue();
    auto vec = g_int_vec_value_config->getValue();
    for(auto& i : vec){
        CONSTANTINEQAQ_LOG_INFO(CONSTANTINEQAQ_LOG_ROOT()) << "before: " << i;
    }

    YAML::Node root = YAML::LoadFile("/home/ConstantineQAQ/workspace/webserver/bin/conf/log.yml");
    ConstantineQAQ::Config::LoadFromYaml(root);

    vec = g_int_vec_value_config->getValue();
    for(auto& i : vec){
        CONSTANTINEQAQ_LOG_INFO(CONSTANTINEQAQ_LOG_ROOT()) << "after: " << i;
    }
    CONSTANTINEQAQ_LOG_INFO(CONSTANTINEQAQ_LOG_ROOT()) << "after: " << g_int_value_config->getValue();
    CONSTANTINEQAQ_LOG_INFO(CONSTANTINEQAQ_LOG_ROOT()) << "after: " << g_float_value_config->getValue();
}

int main(int argc, char** argv){
    test_config();
    // test_yaml();
    return 0;
}