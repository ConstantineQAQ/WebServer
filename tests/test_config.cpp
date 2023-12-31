#include "../include/log.h"
#include "../include/config.h"
#include <yaml-cpp/yaml.h>
#include <iostream>

ConstantineQAQ::ConfigVar<int>::ptr g_int_value_config = 
    ConstantineQAQ::Config::Lookup("system.port", (int)8080, "system port");

ConstantineQAQ::ConfigVar<float>::ptr g_int_config = 
    ConstantineQAQ::Config::Lookup("system.port", (float)8080, "system port");

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
    YAML::Node root = YAML::LoadFile("/home/ConstantineQAQ/workspace/webserver/bin/conf/test.yml");
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
    YAML::Node root = YAML::LoadFile("/home/ConstantineQAQ/workspace/webserver/bin/conf/test.yml");
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

class Person{
public:
    std::string m_name = "";
    int m_age = 0;
    bool m_sex = 0;

    std::string tostring() const{
        std::stringstream ss;
        ss << "[Person name=" << m_name
           << " age=" << m_age
           << " sex=" << m_sex
           << "]";
           return ss.str();
    }

    bool operator== (const Person& oth) const {
        return m_name == oth.m_name
            && m_age == oth.m_age
            && m_sex == oth.m_sex;
    }
};

namespace ConstantineQAQ{
template<>
class LexicalCast<std::string, Person>{
public:
    Person operator() (const std::string& v){
        YAML::Node node = YAML::Load(v);
        Person p;
        p.m_name = node["name"].as<std::string>();
        p.m_age = node["age"].as<int>();
        p.m_sex = node["sex"].as<bool>();
        return p;
    }
};

template<>
class LexicalCast<Person, std::string>{
public:
    std::string operator() (const Person& v) {
        YAML::Node node;
        node["name"] = v.m_name;
        node["age"] = v.m_age;
        node["sex"] = v.m_sex;
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};
}

ConstantineQAQ::ConfigVar<Person>::ptr g_person = 
    ConstantineQAQ::Config::Lookup("class.person", Person(), "system person");

ConstantineQAQ::ConfigVar<std::map<std::string, Person>>::ptr g_person_map = 
    ConstantineQAQ::Config::Lookup("class.map", std::map<std::string, Person>(), "system person");

ConstantineQAQ::ConfigVar<std::map<std::string, std::vector<Person> > >::ptr g_person_vec_map = 
    ConstantineQAQ::Config::Lookup("class.vec_map", std::map<std::string, std::vector<Person>>(), "system person");

/* void test_class(){
    CONSTANTINEQAQ_LOG_INFO(CONSTANTINEQAQ_LOG_ROOT()) << "before: " << g_person->getValue().tostring() << " - " << g_person->toString();
    #define XX_PM(g_var, prefix) \
        { \
            auto m = g_var->getValue(); \
            for(auto& i : m){ \
                CONSTANTINEQAQ_LOG_INFO(CONSTANTINEQAQ_LOG_ROOT()) << prefix << ": " << i.first << " - " << i.second.tostring(); \
            } \
            CONSTANTINEQAQ_LOG_INFO(CONSTANTINEQAQ_LOG_ROOT()) << prefix << ": size=" << m.size(); \
        }

    g_person->addListener(10,[](const Person& old_value, const Person& new_value){
        CONSTANTINEQAQ_LOG_INFO(CONSTANTINEQAQ_LOG_ROOT()) << "old_value=" << old_value.tostring() << " new_value=" << new_value.tostring();
    });
    XX_PM(g_person_map, "class.map before");
    CONSTANTINEQAQ_LOG_INFO(CONSTANTINEQAQ_LOG_ROOT()) << "before: " << g_person_vec_map->toString();
    YAML::Node root = YAML::LoadFile("/home/ConstantineQAQ/workspace/webserver/bin/conf/log.yml");
    ConstantineQAQ::Config::LoadFromYaml(root);

    CONSTANTINEQAQ_LOG_INFO(CONSTANTINEQAQ_LOG_ROOT()) << "after: " << g_person->getValue().tostring() << " - " << g_person->toString();
    XX_PM(g_person_map, "class.map after");
    CONSTANTINEQAQ_LOG_INFO(CONSTANTINEQAQ_LOG_ROOT()) << "after: " << g_person_vec_map->toString();

} */

void test_log(){
    static ConstantineQAQ::Logger::ptr system_log = CONSTANTINEQAQ_LOG_NAME("system");
    CONSTANTINEQAQ_LOG_INFO(system_log) << "hello system" << std::endl;
    std::cout << ConstantineQAQ::LoggerMgr::GetInstance()->toYamlString() << std::endl;
    YAML::Node root = YAML::LoadFile("/home/ConstantineQAQ/workspace/webserver/bin/conf/log.yml");
    ConstantineQAQ::Config::LoadFromYaml(root);
    std::cout << "===============================================" << std::endl;
    std::cout << ConstantineQAQ::LoggerMgr::GetInstance()->toYamlString() << std::endl;
    std::cout << "===============================================" << std::endl;
    std::cout << root << std::endl;
    CONSTANTINEQAQ_LOG_INFO(system_log) << "hello system" << std::endl;

    system_log->setFormatter("%d - %m%n");
    CONSTANTINEQAQ_LOG_INFO(system_log) << "hello system" << std::endl;
}

int main(int argc, char** argv){
    // test_config();
    // test_yaml();
    // test_class();
    test_log();
    ConstantineQAQ::Config::Visit([](ConstantineQAQ::ConfigVarBase::ptr var) {
        CONSTANTINEQAQ_LOG_INFO(CONSTANTINEQAQ_LOG_ROOT()) << "name=" << var->getName()
            << " description=" << var->getDescription()
            << " typename=" << var->getTypeName()
            << " value=" << var->toString();
        });
    return 0;
}