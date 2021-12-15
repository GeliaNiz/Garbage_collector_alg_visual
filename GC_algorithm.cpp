#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <fstream>
#include <sys/time.h>
#include <random>
#include <algorithm>

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

    vector<weak_ptr<Simple_object>> simple_objects_weak;
    //Подсчет удаленных ссылок
    int count_deleted = 0;

public:
    //Добавляет стартовый набор объектов
    void Add_objects(int count){
        for(int i = count;i>0;i--){
            simple_objects.push_back(static_cast<const shared_ptr<Simple_object>>(new Simple_object("Object"+to_string(i))));
        }
        for(const auto & simple_object : simple_objects){
            weak_ptr<Simple_object> pointer = simple_object;
            simple_objects_weak.push_back(simple_object);
        }
    }

    //Добавляет новый умный указатели
    void Add_some_link(){
        uniform_int_distribution<> dist(1,simple_objects.size());
        random_device rand;
        int index = dist(rand);
       weak_ptr<Simple_object> pointer = simple_objects[index];
       simple_objects.push_back(pointer.lock());
       simple_objects_weak.push_back(pointer);
    }

    //Удаляет умные указатели. Если число умных указателей shared_ptr, связанных с конкретным объектом при выполнении
    //функции достигает 0, то удаляется и сам объект. Однако shared_ptr указатели не удаляются, если они зациклены.
    // Именно поэтому удаляем и weak_ptr и вместе с ним освобождаем даже память, попавшую в такой цикл.
    void Delete_some_link(){
        uniform_int_distribution<> dist(0,simple_objects.size());
        random_device rand;
        int index = dist(rand);
        simple_objects_weak[index].reset();
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
        int quantity =0;
        for(int i=0;i<simple_objects.size();i++) {
             if(simple_objects[i]!= nullptr){
                quantity+=simple_objects[i].use_count();
            }
        }
            for(const auto & simple_object : simple_objects) {
                if(simple_object != nullptr) {
                    data_file << quantity << "," <<
                              simple_objects.size()*sizeof(simple_object)<< "," << simple_object->getValue() << "," <<
                              simple_object.use_count()<<","<<sizeof(simple_object)<<","<<count_deleted
                              <<","<<time(nullptr);
                    data_file<<endl;
                    data_file.flush();
                }
            }
        }

};


