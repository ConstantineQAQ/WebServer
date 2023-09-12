#include "../include/log.h"
#include "../include/config.h"
#include <yaml-cpp/yaml.h>

ConstantineQAQ::ConfigVar<int>::ptr g_int_value_config = 
    ConstantineQAQ::Config::Lookup("system.port", (int)8080, "system port");

ConstantineQAQ::ConfigVar<float>::ptr g_float_value_config = 
    ConstantineQAQ::Config::Lookup("system.value", (float)10.2f, "system value");

ConstantineQAQ::ConfigVar<std::vector<int>>::ptr g_int_vec_value_config = 
    ConstantineQAQ::Config::Lookup("system.int_vec", std::vector<int>{1,2}, "system int vec");

ConstantineQAQ::ConfigVar<std::list<int>>::ptr g_int_list_value_config = 
    ConstantineQAQ::Config::Lookup("system.int_list", std::list<int>{1,2}, "system int list");

ConstantineQAQ::ConfigVar<std::set<int>>::ptr g_int_set_value_config =
    ConstantineQAQ::Config::Lookup("system.int_set", std::set<int>{1,2}, "system int set");

ConstantineQAQ::ConfigVar<std::unordered_set<int>>::ptr g_int_unordered_set_value_config =
    ConstantineQAQ::Config::Lookup("system.int_unordered_set", std::unordered_set<int>{1,2}, "system int unordered_set");

ConstantineQAQ::ConfigVar<std::map<std::string,int>>::ptr g_str_int_map_value_config =
    ConstantineQAQ::Config::Lookup("system.str_int_map", std::map<std::string,int>{{"k",2}}, "system str int map");

ConstantineQAQ::ConfigVar<std::unordered_map<std::string,int>>::ptr g_str_int_unordered_map_value_config =
    ConstantineQAQ::Config::Lookup("system.str_int_unordered_map", std::unordered_map<std::string,int>{{"k",2}}, "system str int unordered_map");

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
    #define XX(g_var, name, prefix) \
        { \
            auto& v = g_var->getValue(); \
            for(auto& i : v){ \
                CONSTANTINEQAQ_LOG_INFO(CONSTANTINEQAQ_LOG_ROOT()) << #prefix " " #name ": " << i; \
            } \
            CONSTANTINEQAQ_LOG_INFO(CONSTANTINEQAQ_LOG_ROOT()) << #prefix " " #name " yaml: " << g_var->toString(); \
        }

    #define XX_M(g_var, name, prefix) \
        { \
            auto& v = g_var->getValue(); \
            for(auto& i : v){ \
                CONSTANTINEQAQ_LOG_INFO(CONSTANTINEQAQ_LOG_ROOT()) << #prefix " " #name ": {" << i.first << " - " << i.second << "}"; \
            } \
            CONSTANTINEQAQ_LOG_INFO(CONSTANTINEQAQ_LOG_ROOT()) << #prefix " " #name " yaml: " << g_var->toString(); \
        }
    XX(g_int_vec_value_config, int_vec, before);
    XX(g_int_list_value_config, int_list, before);
    XX(g_int_set_value_config, int_set, before);
    XX(g_int_unordered_set_value_config, int_unordered_set, before);
    XX_M(g_str_int_map_value_config, str_int_map, before);
    XX_M(g_str_int_unordered_map_value_config, str_int_unordered_map, before);
    YAML::Node root = YAML::LoadFile("/home/ConstantineQAQ/workspace/webserver/bin/conf/log.yml");
    ConstantineQAQ::Config::LoadFromYaml(root);
    std::cout << "---------------------------------------------------------" << std::endl;
    XX(g_int_vec_value_config, int_vec, after);
    XX(g_int_list_value_config, int_list, after);
    XX(g_int_set_value_config, int_set, after);
    XX(g_int_unordered_set_value_config, int_unordered_set, after);
    XX_M(g_str_int_map_value_config, str_int_map, after);
    XX_M(g_str_int_unordered_map_value_config, str_int_unordered_map, after);
    CONSTANTINEQAQ_LOG_INFO(CONSTANTINEQAQ_LOG_ROOT()) << "after: " << g_int_value_config->getValue();
    CONSTANTINEQAQ_LOG_INFO(CONSTANTINEQAQ_LOG_ROOT()) << "after: " << g_float_value_config->getValue();
}

int main(int argc, char** argv){
    test_config();
    // test_yaml();
    return 0;
}