#ifndef I18N_LOOKUP_H
#define I18N_LOOKUP_H
#include <string>
#include <unordered_map>
#include <fstream>

namespace i18n::utf8 {

enum class FileType
{
    ft_unkwon = 0,
    ft_ini,
    ft_json,
};
class LookUp
{
public:
    bool setLocale(std::string locale,std::string file,FileType type = FileType::ft_ini);
    std::string locale(){return locale_;}
    FileType type(){return type_;}
    std::string lookup(std::string text);
private:
    FileType type_ = FileType::ft_unkwon;
    std::string locale_ = "unkwon";
    std::unordered_map<std::string,std::string> data_;
};
class SaveUp
{
public:
    bool addtext(std::string label, std::string text);
    std::string locale(){return locale_;}
    FileType type(){return type_;}
    bool syncSave(std::string locale,std::string file,FileType type = FileType::ft_ini);
private:
    FileType type_ = FileType::ft_unkwon;
    std::string locale_= "unkwon";
    std::unordered_map<std::string,std::string> data_;
};
bool LookUp::setLocale(std::string locale,std::string file,FileType type)
{
    std::ifstream rd;
    rd.open(file);
    if(!rd.is_open()) return false;
    locale_ = std::move(locale);
    type_ = type;
    data_.clear();
    std::string line;
    while (std::getline(rd,line)) {
        if(line.size() && (line.front() == '{' || line.back() == '}'))
        {
            type_ = FileType::ft_json;
            continue;
        }
        std::string key,value;
        bool if_key = true;
        for (int var = 0; var < line.size() ; ++var) {

        }
        if(key.size()&&value.size()) data_[key] = std::move(value);
    }
    rd.close();
    return true;
}
std::string LookUp::lookup(std::string text)
{
    if(auto iter = data_.find(text); iter!= data_.end())
        return iter->second;
    return text;
}
bool SaveUp::addtext(std::string label, std::string text){
    data_[label] = std::move(text);
    return false;

}
bool SaveUp::syncSave(std::string locale,std::string file,FileType type){
    std::ifstream rd;
    rd.open(file);
    if(!rd.is_open()) return false;
    locale_ = std::move(locale);
    type_ = type;
    switch (type) {
    case FileType::ft_json:
        type_ = FileType::ft_json;
        break;
    default:
        type_ = FileType::ft_ini;
        break;
    }

    rd.close();
    return  true;
}

}

#endif // I18N_LOOKUP_H
