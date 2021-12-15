#include <iostream>
#include <thread>
#include <mutex>
#include "GC_algorithm.cpp"

using namespace std;

bool flag = true;

void write_data(ofstream &file, GC_Generator &generator){
    while(flag) {
        generator.Create_file_with_data(file);
        this_thread::sleep_for(chrono::milliseconds(10000));
    }
}

void do_something(GC_Generator &generator){
    while (flag) {
        generator.Add_some_link();
        this_thread::sleep_for(chrono::milliseconds(5000));
    }
}
void clear_something(GC_Generator &generator){
    while (flag) {
        generator.Delete_some_link();
       this_thread::sleep_for(chrono::milliseconds(100));
    }
}

void add_objects(int quantity, GC_Generator &generator){
    while(flag) {
        generator.Add_objects(quantity);
        this_thread::sleep_for(chrono::milliseconds(10000));
    }
}
int main() {

    //Стартовое число объектов
    int objects_quantity = 5;

    //Создаем генератор и стартовые объекты
    GC_Generator generator;
    generator.Add_objects(objects_quantity);

    //Открываем поток записи данных в файл
    ofstream data_file;
    data_file.open("../data.csv", ios::app);
    data_file <<"Total objects,Total memory in use,Object name,Links,Object size,Links deleted,Date"<<endl;
    generator.Create_file_with_data(data_file);
    if(data_file.is_open()) {
    //Работа с потоками
        thread t1(do_something,ref(generator));

        thread t2(clear_something, ref(generator));

       thread t3(add_objects,3,ref(generator));

        thread t4(write_data, ref(data_file), ref(generator));

        cin.ignore();
        flag = false;

        t1.join();
        t2.join();
        t3.join();
        t4.join();
    }
 generator.Create_file_with_data(data_file);
    data_file.close();

    return 0;
}
