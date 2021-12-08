#include <iostream>
#include <thread>
#include <mutex>
#include "GC_algorithm.cpp"

using namespace std;

mutex m1;

void do_something(int times, GC_Generator &generator){
    for (int i = 0; i < times; i++) {
        generator.Add_some_link();
    }
}
void clear_something(int times, GC_Generator &generator){
    for (int i = 0; i < times; i++) {
        generator.Delete_some_link();
    }
}

int main() {

    //Стартовое число объектов
    int objects_quantity = 100;

    //Создаем генератор и стартовые объекты
    GC_Generator generator;
    generator.Add_objects(objects_quantity);

    //Открываем поток записи данных в файл
    fstream data_file;
    data_file.open("../data.csv",ios::app);
    data_file <<"Total objects,Total memory in use,Object name,Links,Object size,Links deleted"<<endl;

    generator.Create_file_with_data(data_file);

    if(data_file.is_open()) {

    //Работа с потоками

        m1.lock();
        thread t1(do_something, 1000, ref(generator));
        this_thread::sleep_for(chrono::milliseconds(100));
        m1.unlock();

        generator.Create_file_with_data(data_file);

        m1.lock();
        thread t2(clear_something, 200, ref(generator));
        this_thread::sleep_for(chrono::milliseconds(100));
        m1.unlock();

        generator.Create_file_with_data(data_file);

        t1.join();
        t2.join();

    }
    data_file.close();

    return 0;
}
