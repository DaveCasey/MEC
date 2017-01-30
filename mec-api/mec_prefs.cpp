#include "mec_prefs.h"

#include <fstream>

#include <cJSON.h>

#include "mec_log.h"

MecPreferences::MecPreferences(const std::string& file) : jsonData_(nullptr), owned_(true) {
    valid_ = loadPreferences(file);
}

MecPreferences::MecPreferences(void* p) : jsonData_(static_cast<cJSON*> (p)), owned_(false) {
}



MecPreferences::~MecPreferences() {
    if (jsonData_ && owned_ ) cJSON_Delete((cJSON*) jsonData_);
}

bool MecPreferences::loadPreferences(const std::string& file) {
    if (jsonData_ && owned_) {
        cJSON_Delete((cJSON*) jsonData_);
        jsonData_ = nullptr;
    }

    // std::string file = "./mec.json";
    std::ifstream t(file);
    if (t.good())
    {
        std::string jsonStr;
        t.seekg(0, std::ios::end);
        jsonStr.reserve(t.tellg());
        t.seekg(0, std::ios::beg);

        jsonStr.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
        jsonData_ = cJSON_Parse(jsonStr.c_str());
        owned_ = true;


        if (jsonData_) {
            LOG_0("loaded preferences");
            char* p = cJSON_Print( (cJSON*) jsonData_);
            std::cout << p << std::endl;
            delete p;
            return true;
        }
        else
        {
            LOG_0( "unable to parse preferences file");
        }
    }
    else
    {
        LOG_0("unable to load preferences file");
    }
    return false;
}

bool MecPreferences::getBool(const std::string& v, bool def) {
    if (! jsonData_) return def;
    cJSON * node = cJSON_GetObjectItem((cJSON*) jsonData_, v.c_str());
    if (node!=nullptr) {
        if(node->type == cJSON_True) return true;
        else if (node->type == cJSON_False) return false;
    }
    return def;
}


int MecPreferences::getInt(const std::string& v, int def) {
    if (! jsonData_) return def;
    cJSON * node = cJSON_GetObjectItem((cJSON*) jsonData_, v.c_str());
    if (node!=nullptr && node->type == cJSON_Number) {
        return node->valueint;
    }
    return def;
}


double MecPreferences::getDouble(const std::string& v, double def) {
    if (! jsonData_) return def;
    cJSON * node = cJSON_GetObjectItem((cJSON*) jsonData_, v.c_str());
    if (node!=nullptr && node->type == cJSON_Number) {
        return node->valuedouble;
    }
    return def;
}

std::string MecPreferences::getString(const std::string& v, const std::string def) {
    if (! jsonData_) return def;
    cJSON * node = cJSON_GetObjectItem((cJSON*) jsonData_, v.c_str());
    if (node!=nullptr && node->type == cJSON_String) {
        return node->valuestring;
    }
    return def;
}

void* MecPreferences::getArray(const std::string v) {
    if (! jsonData_) return nullptr;
    cJSON * node = cJSON_GetObjectItem((cJSON*) jsonData_, v.c_str());
    if (node!=nullptr && node->type == cJSON_Array) {
        return node;
    }
    return nullptr;
}

int MecPreferences::getArraySize(void* v) {
    cJSON *node = (cJSON*) v;
    if(node!=nullptr && node->type == cJSON_Array) {
        return cJSON_GetArraySize(node);
    }
    return 0;
}

int MecPreferences::getArrayInt(void* v,int i, int def) {
    cJSON *array = (cJSON*) v;
    cJSON *node = cJSON_GetArrayItem(array,i);
    if (node!=nullptr && node->type == cJSON_Number) {
        return node->valueint;
    }
    return def;
}


void* MecPreferences::getTree() {
    return jsonData_;
}

void* MecPreferences::getSubTree(const std::string v) {
    if (! jsonData_) return nullptr;
    cJSON * node = cJSON_GetObjectItem((cJSON*) jsonData_, v.c_str());
    if (node!=nullptr && node->type == cJSON_Object) {
        return node;
    }
    return nullptr;
}

bool MecPreferences::exists(const std::string v) {
    if (! jsonData_) return false;
    cJSON * node = cJSON_GetObjectItem((cJSON*) jsonData_, v.c_str());
    return node != nullptr;
}

void MecPreferences::print() {
    if (! jsonData_)  {
        LOG_0("invalid json data");

    } else {
        char* p = cJSON_Print((cJSON*) jsonData_);
        LOG_0(p);
        delete p;
    }
}



