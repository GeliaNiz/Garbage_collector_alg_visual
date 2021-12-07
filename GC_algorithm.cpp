#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <fstream>
#include <windows.h>
#include <cstdio>
#include <psapi.h>

#define DIV 1048576
#define WIDTH 7


using namespace std;

class Simple_object {
private:
    string value;


public:
    explicit Simple_object(string value) : value(std::move(value)) {}

    const string &getValue() const {
        return value;
    }

    void setValue(const string &value) {
        Simple_object::value = value;
    }

};

class GC_Generator{
private:
    vector<shared_ptr<Simple_object>> simple_objects;
    int count_deleted;

public:
    void Add_objects(int count){
        for(int i = count;i>0;i--){
            simple_objects.push_back(static_cast<const shared_ptr<Simple_object>>(new Simple_object("Object"+to_string(i))));
        }
    }

    void Add_some_link(){
        int index = rand()%simple_objects.size();
       shared_ptr<Simple_object> pointer = simple_objects[index];
       simple_objects.push_back(pointer);
    }

    void Delete_some_link(){
        int index = rand()%simple_objects.size();
        simple_objects.erase(simple_objects.begin()+index);
        count_deleted++;
    }

    const vector<shared_ptr<Simple_object>> &getSimpleObjects() const {
        return simple_objects;
    }

    void setSimpleObjects(const vector<shared_ptr<Simple_object>> &simpleObjects) {
        simple_objects = simpleObjects;
    }

    int getCountDeleted() const {
        return count_deleted;
    }

    void setCountDeleted(int countDeleted) {
        count_deleted = countDeleted;
    }

    void Show_content(){
        for(const auto & simple_object : simple_objects){
            cout << simple_object->getValue()<<" "<< simple_object.use_count() << endl;
        }
    }

    void Create_file_with_data(ofstream& data_file){
        MEMORYSTATUSEX statex;
        statex.dwLength = sizeof (statex);
        GlobalMemoryStatusEx (&statex);
            for(const auto & simple_object : simple_objects) {
                data_file<<simple_objects.size()<<","<<statex.dwMemoryLoad<<","<<
                statex.ullAvailPhys<<","<<simple_object->getValue()<<","<<
                simple_object.use_count()<<","<<count_deleted;
                data_file<<endl;
                data_file.flush();
            }
            count_deleted = 0;
//        data_file.flush();
        }

};


