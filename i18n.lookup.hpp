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
    if(type_==FileType::ft_unkwon ) type_ = FileType::ft_ini;
    data_.clear();
    std::string line;
    while (std::getline(rd,line)) {
        if(line.size() && (line.front() == '{' || line.back() == '}'))
        {
            type_ = FileType::ft_json;
            continue;
        }
        if(line.empty()) continue;
        int32_t n = static_cast<int32_t>(line.size());
        std::string key,value,temp;
        bool if_find_key_end;
        for (int right = 0; right < n; ++right) {
            if(!if_find_key_end && right < n && ((type_ == FileType::ft_json &&  line[right]==':') ||
                                                  (type_ == FileType::ft_ini &&  line[right]=='='))
                )
            {
                if_find_key_end = true;
                key.swap(temp);
                continue;
            }
            if(line[right] == '\\' && right+ 1 < n)
            {      right++;
                switch (line[right]) {
                case 'n': temp += '\n'; break;
                case 't': temp += '\t'; break;
                case 'r': temp += '\r'; break;
                case '"': temp += '"'; break;
                default: temp += line[right];break;
                }
                continue;
            }
            if(line[right]!='"')temp += line[right];
        }
        value.swap(temp);
        if(key.size()&&value.size()) data_[key] = std::move(value);
    }
    rd.close();
    return data_.size();
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
    std::ofstream rd;
    rd.open(file);
    if(!rd.is_open()) return false;
    locale_ = std::move(locale);
    type_ = type;
    switch (type) {
    case FileType::ft_json:
        type_ = FileType::ft_json;
        rd << "{" << std::endl;
        for (auto &&[key,value] : data_) {
            rd << "\"" << key <<"\":\""<< value << "\""<< std::endl;
        }
        rd << "}" << std::endl;
        break;
    default:
        type_ = FileType::ft_ini;
        for (auto &&[key,value] : data_) {
            rd << key <<"="<< value << std::endl;
        }
        break;
    }
    rd.close();
    return data_.size();
}

}

#endif // I18N_LOOKUP_H
