#include <iostream>
#include <thread>
#include <mutex>
#include <windows.h>
#include <stdio.h>
#include <psapi.h>
#include "GC_algorithm.cpp"
using namespace std;

int main() {
    ofstream data_file;
    data_file.open("../data.csv",ios_base::app);
    data_file <<"Total objects,Memory in use in %, Available memory,Object name,Links,Links deleted"<<endl;
    int objects_quantity = 100;
    GC_Generator generator;
    generator.Add_objects(objects_quantity);
    if(data_file.is_open()) {
        for (int i = 0; i < 100; i++) {
            generator.Add_some_link();
        }
        generator.Create_file_with_data(data_file);
        for (int i = 0; i < 200; i++) {
            generator.Add_some_link();
        }
        generator.Create_file_with_data(data_file);
        for (int i = 0; i < 50; i++) {
            generator.Delete_some_link();
        }
        generator.Create_file_with_data(data_file);
    }
    data_file.close();
    return 0;
}
