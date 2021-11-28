#include <iostream>
#include <thread>
#include <mutex>
#include "GC_algorithm.cpp"
using namespace std;

void perform_task(){
    cout << "Task perform!" << endl;

}
int main() {
//    mutex first_mutex;
    GC_Generator generator;
    generator.Add_objects(10);
    generator.Show_content();
    cout<<endl;

    generator.Add_some_link();
    generator.Add_some_link();
    generator.Show_content();
    cout<<endl;

    generator.Delete_some_link();
    generator.Show_content();

//    thread gc_thread(Garbage_collector::gc_algorithm);
//    gc_thread.join();
//    thread task_thread(perform_task);
//    task_thread.detach();
    return 0;
}
