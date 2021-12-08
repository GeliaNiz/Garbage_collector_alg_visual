#include <iostream>
#include <thread>
#include <mutex>
#include "GC_algorithm.cpp"

using namespace std;

mutex m1;
fstream data_file;

void write_data(fstream &file, GC_Generator &generator){
    generator.Create_file_with_data(file);
    this_thread::sleep_for(chrono::milliseconds(100));
}
void do_something(int times, GC_Generator &generator){
    for (int i = 0; i < times; i++) {
        generator.Add_some_link();
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}
void clear_something(int times, GC_Generator &generator){
    for (int i = 0; i < times; i++) {
        generator.Delete_some_link();
        this_thread::sleep_for(chrono::milliseconds(100));
    }
}
void add_objects(int quantity, GC_Generator &generator){
    generator.Add_objects(quantity);
    this_thread::sleep_for(chrono::milliseconds(300));
}
int main() {

    //Стартовое число объектов
    int objects_quantity = 40;

    //Создаем генератор и стартовые объекты
    GC_Generator generator;
    generator.Add_objects(objects_quantity);

    //Открываем поток записи данных в файл

    data_file.open("../data.csv",ios::app| ios::ate);
    data_file <<"Total objects,Total memory in use,Object name,Links,Object size,Links deleted"<<endl;
    generator.Create_file_with_data(data_file);

    if(data_file.is_open()) {

    //Работа с потоками

        thread t1(do_something, 70, ref(generator));

        thread t2(clear_something,30, ref(generator));

        thread t3(add_objects,40,ref(generator));

        thread t4(write_data, ref(data_file), ref(generator));

        t1.join();
        t2.join();
        t3.join();
        t4.join();
    }
    generator.Create_file_with_data(data_file);
    data_file.close();

    return 0;
}
