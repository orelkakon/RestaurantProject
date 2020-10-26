

#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include "Table.h"


using namespace std;


Table::Table(int t_capacity):capacity(t_capacity), open(false), customersList(), orderList(){
    this->orderList = vector<OrderPair>();
}
Table:: ~Table(){
    for (auto &i : customersList) {
        delete i;
        i = nullptr;
    }
    customersList.clear();
}
Table::Table(const Table& other):capacity(other.getCapacity()), open(false), customersList(), orderList(){
    this->customersList = vector<Customer*>(0);
    if(other.open)
        this->open = true;
    else
        this->open = false;
    for (auto i : other.customersList) {

        this->customersList.push_back(i->clone());
    }
    for (const auto &i : other.orderList) {
        this->orderList.push_back(OrderPair(i.first, i.second));
    }
}
Table& Table::operator=(const Table& other){
    if(this == &other)
        return *this;
    this->open = other.open;
    this->capacity = other.capacity;
    for(unsigned int i=0; i<this->customersList.size();i++){
        delete (customersList[i]);
    }
    customersList.clear();
    for (auto i : other.customersList) {
        this->customersList.push_back(i->clone());
    }
    this->orderList.clear();
    for (const auto &i : other.orderList) {
        this->orderList.push_back(OrderPair(i.first, i.second));
    }

    return *this;
}
Table::Table(Table&& other):capacity(other.getCapacity()),open(false), customersList(), orderList(){
    this->customersList = vector<Customer*>(0);
    this->open = other.open;
    for (auto i : other.customersList) {
        this->customersList.push_back(i);
    }
    for (auto &i : other.customersList) {
        i = nullptr;
    }
    this->orderList.clear();
    for (auto &i : other.orderList) {
        this->orderList.push_back(OrderPair(i.first, i.second));
    }

}
Table& Table::operator=(Table&& other){
    if(this == &other)
        return *this;
    this->open = other.open;
    this->capacity = other.capacity;
    for(unsigned int i=0; i<this->customersList.size();i++){
        delete (customersList[i]);
    }
    for (auto i : other.customersList) {
        this->customersList.push_back(i);
    }
    for (auto &i : other.customersList) {
        i = nullptr;
    }
    this->orderList.clear();
    for (auto &i : other.orderList) {
        this->orderList.push_back(OrderPair(i.first, i.second));
    }
    return *this;
}
int Table:: getCapacity() const{
    return this->capacity;
}
void Table::addCustomer(Customer* customer) {
    if (customer != nullptr) {
        bool found = false;
        for (unsigned int i = 0; i < customersList.size() && !found; i++) {
            if (customersList[i]->getId() == customer->getId()) {

                found = true;
            }
        }
        if (!found) {
            customersList.push_back(customer->clone());
        }
    }

}
void Table::removeCustomer(int id){
    for(auto it = customersList.begin(); it != customersList.end(); ++it) {
        if ((*it)->getId() == id) {
            delete *it;
            customersList.erase(it);
            return;
        }
    }
}
Customer* Table:: getCustomer(int id){
    for (auto &it : customersList) {
        if (it->getId() == id) {
            return it;
        }
    }
    return nullptr;
}
std::vector<Customer *>& Table:: getCustomers() {
    return this->customersList;
}
std::vector<OrderPair>& Table:: getOrders(){
    return this->orderList;
}
void Table::order(const std::vector<Dish> &menu){
    vector<int> order1 = vector<int>(0);
    for (auto &it : customersList) {
        order1 = it->order(menu);
        for (auto i : order1) {
            OrderPair op(it->getId() , menu.at(i));
            orderList.push_back(op);
        }
    }
}
void Table:: openTable(){
    this->open = true;
}
void Table::closeTable(){
    this->open = false;
    for(unsigned int i=0; i<this->customersList.size();i++){
        delete (customersList[i]);
    }
    customersList.clear();
    orderList.clear();

}
int Table:: getBill(){
    int bill = 0;
    for (auto &it : orderList) {
        bill = bill+ it.second.getPrice();
    }
    return bill;
}
bool Table:: isOpen(){
    return this->open;
}
Table* Table::clone(){
    return new Table(*this);

}


