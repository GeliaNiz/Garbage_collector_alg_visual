#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include "mutex"
#include <shared_mutex>
#include <fstream>
#include <windows.h>
#include <sys/time.h>



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
    int count_deleted = 0;
    mutex my_mutex;

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
       cout<<"add "<<simple_objects[index]<<endl;
    }

    void Delete_some_link(){
        int index = rand()%simple_objects.size();
            simple_objects[index].reset();
        cout<<"delete "<<simple_objects[index]<<"size "<<simple_objects.size()<<endl;
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

    void Create_file_with_data(fstream& data_file){
        MEMORYSTATUSEX statex;
        statex.dwLength = sizeof (statex);
        GlobalMemoryStatusEx (&statex);
            for(const auto & simple_object : simple_objects) {
                if(simple_object != nullptr) {
                    data_file << simple_objects.size() << "," <<
                              statex.dwMemoryLoad << "," <<
                              simple_objects.size()*sizeof(simple_object)<< "," << simple_object->getValue() << "," <<
                              simple_object.use_count()<<","<<sizeof(simple_objects)<<","<<count_deleted;
                    data_file<<endl;
                    data_file.flush();
                }
            }
            count_deleted = 0;
        }

};


