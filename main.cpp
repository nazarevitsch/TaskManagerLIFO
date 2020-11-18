#include <iostream>
#include <mutex>
#include <thread>
#include <chrono>
#include <string>

using namespace std;

struct Exercise{
    uint16_t time;
};

struct Node {
    Node* prev;
    Exercise* exercise;
};

class Stack {
private:
    int size;
    Node* last;
public:
    Stack();
    void add(Exercise* exercise);
    Exercise* getLast();
    Exercise* deleteLast();
    int getSize();
    bool isEmpty();
    void display();
};

Stack::Stack() {
    this->size = 0;
    this->last = NULL;
}

void Stack::add(Exercise* exercise){
    Node* node = (Node*)(malloc(sizeof(Node)));
    node->exercise = exercise;
    if (size == 0) {
        node->prev = NULL;
        this->last = node;
    } else {
        node->prev = last;
        this->last = node;
    }
    size++;
}

Exercise* Stack::getLast() {
    if (size != 0) {
        return last->exercise;
    } else return NULL;
}

Exercise* Stack::deleteLast() {
    if (size != 0) {
        Exercise *ex = this->last->exercise;
        this->last = last->prev;
        size--;
        return ex;
    } else return NULL;
}

int Stack::getSize() {
    return this->size;
}

bool Stack::isEmpty() {
    return this->size == 0 ? true : false;
}

void Stack::display() {
    Node* node = this->last;
    for (int i = 0; i < this->size; i++) {
        cout << "I: " << i << ", T: " << node->exercise->time << endl;
        node = node->prev;
    }
}

mutex mtx;

class TaskManager {
public:
    Stack stack;
    void addTasks(int times, int randomTime);
    void doingTask();
    void outer(string text);
};


void TaskManager::doingTask() {
    while (!stack.isEmpty()) {
        Exercise* e = stack.deleteLast();
        outer("Task was done: " + to_string(e->time));
        this_thread::sleep_for((1ms) * e->time);
    }
}

void TaskManager::addTasks(int times, int randomTime) {
    srand((unsigned) time(0));
    for (int i = 0; i < times; i++) {
        Exercise* exercise = (Exercise*)(malloc(sizeof(Exercise)));
        exercise->time = (rand() % randomTime);
        stack.add(exercise);
        this_thread::sleep_for(250ms);
        outer("Task was added with time: " + to_string(exercise->time));
    }
}

void TaskManager::outer(string text) {
    mtx.lock();
    cout << text << endl;
    mtx.unlock();
}


int main() {
    TaskManager exercisesList;
    exercisesList.addTasks(10, 850);
    std::thread solver(&TaskManager::doingTask, &exercisesList);
    std::thread adder(&TaskManager::addTasks, &exercisesList, 20, 600);
    solver.join();
    adder.join();
    return 0;
}