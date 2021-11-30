#include <iostream>
#include <thread>
#include <mutex>
#include "GC_algorithm.cpp"
using namespace std;

int main() {
//    mutex first_mutex;
int objects_quantity = 10;
    GC_Generator generator;
    generator.Add_objects(objects_quantity);

    for(int i =0;i<100;i++) {
        generator.Add_some_link();
    }
    generator.Create_file_with_data();

    generator.Create_file_with_data();
    for(int i =0;i<50;i++) {
        generator.Delete_some_link();
    }
    generator.Create_file_with_data();

    return 0;
}
