#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <fstream>
#include <sys/time.h>

using namespace std;

//Класс объектов, которыми оперируем в программе
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


//Класс, имулирующий работу сборщика мусора
class GC_Generator{
private:
    //Вектор из умных указателей на наши объекты. Shared_ptr реализует алгоритм "подсчет ссылок" по умолчанию.
    vector<shared_ptr<Simple_object>> simple_objects;
    //Подсчет удаленных ссылок
    int count_deleted = 0;

public:
    //Добавляет стартовый набор объектов
    void Add_objects(int count){
        for(int i = count;i>0;i--){
            simple_objects.push_back(static_cast<const shared_ptr<Simple_object>>(new Simple_object("Object"+to_string(i))));
        }
    }

    //Добавляет новый умный указатели
    void Add_some_link(){
        int index = rand()%simple_objects.size();
       shared_ptr<Simple_object> pointer = simple_objects[index];
       simple_objects.push_back(pointer);
    }

    //Удаляет умный указатель. Если число умных указателей, связанных с конкретным объектом при выполнении
    //функции достигает 0, то удаляется и сам объект
    void Delete_some_link(){
        int index = rand()%simple_objects.size();
        simple_objects[index].reset();
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

    //Записывает параметры в файл
    void Create_file_with_data(ofstream& data_file){
            for(const auto & simple_object : simple_objects) {
                if(simple_object != nullptr) {
                    data_file << simple_objects.size() << "," <<
                              simple_objects.size()*sizeof(simple_object)<< "," << simple_object->getValue() << "," <<
                              simple_object.use_count()<<","<<sizeof(simple_objects)<<","<<count_deleted
                              <<","<<time(nullptr);
                    data_file<<endl;
                    data_file.flush();
                }
            }
        }

};


