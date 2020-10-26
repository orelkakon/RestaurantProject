

#include <utility>

#include <utility>

#include <utility>

#include <utility>

//
// Created by orel kakon on 06/11/2018.
//

//
// Created by mirhaus on 26/10/18.
//
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include "Customer.h"


using namespace std;

Customer::Customer(std::string c_name, int c_id):name(std::move(c_name)), id(c_id), orderList(){
    this->orderList = vector<int>(0);
}
Customer::~Customer(){}
std::vector<int> Customer::getOrderList(){
    return this->orderList;
}
std::string Customer:: getName() const{
    return this->name;
}
int Customer::getId() const{

    return this->id;
}
//VEG
VegetarianCustomer::VegetarianCustomer(std::string name, int id):Customer(std::move(name), id){

}
VegetarianCustomer::~VegetarianCustomer(){}
std::vector<int> VegetarianCustomer:: order(const std::vector<Dish> &menu) {
    vector<int> order = vector<int>();
    int id = -1;
    int bevPrice = -2;
    int bevId=-3;
    bool found = false;

    for( unsigned int i=0; i<menu.size()&&!found; i++){
        if (menu[i].getType() == VEG ) {
            found = true;
            id = menu[i].getId();
        }
    }
    for (const auto &i : menu) {
        if(i.getType()== VEG && i.getId()<id)
            id = i.getId();
    }

    found = false;
    for( unsigned int i=0; i<menu.size()&&!found; i++){
        if(menu[i].getType()==BVG) {
            found = true;
            bevPrice = menu[i].getPrice();
            bevId = menu[i].getId();
        }
    }
    for (const auto &i : menu) {
        if(i.getType()==BVG && (i.getPrice()>bevPrice||(i.getPrice() == bevPrice && i.getId() < bevId))) {
            bevPrice = i.getPrice();
            bevId = i.getId();
        }
    }
    if(id != -1 && bevId != -3){
            order.push_back(id);
            this->getOrderList().push_back(id);
            order.push_back(bevId);
            this->getOrderList().push_back(bevId);

    }

    return order;
}
std::string VegetarianCustomer::toString() const {
    return "veg";
}
Customer* VegetarianCustomer::clone(){
    Customer *ret = new VegetarianCustomer(*this);
    return ret;
}
//CHP
CheapCustomer::CheapCustomer(std::string name, int id):Customer(std::move(name), id), numOfOrders(0){
    this->numOfOrders = 0;
}
CheapCustomer::~CheapCustomer(){}
std::vector<int>  CheapCustomer::order(const std::vector<Dish> &menu){
    vector<int> order = vector<int>();
    if(numOfOrders == 0){
        int id = menu[0].getId();
        int price = menu[0].getPrice();
        for (const auto &i : menu) {
            if (i.getPrice() < price||(i.getPrice()== price&&i.getId()<id)) {
                price = i.getPrice();
                id = i.getId();
            }
        }
        numOfOrders++;
        order.push_back(id);
    }
    return order;
}
std::string CheapCustomer::toString() const{
    return "chp";
}
Customer* CheapCustomer::clone(){
    return new CheapCustomer(*this);

}
//SPC
SpicyCustomer::SpicyCustomer(std::string name, int id):Customer(std::move(name), id), numOfOrders(0){
    this->numOfOrders = 0;

}
SpicyCustomer::~SpicyCustomer(){}
std::vector<int> SpicyCustomer::order(const std::vector<Dish> &menu){

    vector<int> order = vector<int>(0);
    int price = -1;
    int id = -2;
    bool found = false;
    if(numOfOrders > 0){
        for( unsigned int i=0; i<menu.size()&&!found ; i++){
            if (menu[i].getType() == BVG) {
                found = true;
                price = menu[i].getPrice();
                id = menu[i].getId();
            }

        }
        for (const auto &i : menu) {
            if (i.getType() == BVG) {
                if (i.getPrice() < price || (i.getPrice() == price && i.getId() < id)) {
                    price = i.getPrice();
                    id = i.getId();
                }
            }
        }
    }
    else {
        for( unsigned int i=0; i<menu.size()&&!found; i++){
            if (menu[i].getType() == SPC) {
                price = menu[i].getPrice();
                id = menu[i].getId();
                found = true;
            }
        }

        for (const auto &i : menu) {
            if (i.getType() == SPC && (i.getPrice() > price|| (i.getPrice()==price && i.getId()<id))) {
                price = i.getPrice();
                id = i.getId();
            }
        }
    }
    if(id!=-2&&price!=-1){
        numOfOrders = numOfOrders+1;
        order.push_back(id);
    }

    return order;
}
std::string SpicyCustomer::toString() const{
    return "spc";
}
Customer* SpicyCustomer::clone(){
    return new SpicyCustomer(*this);

}
//ALC
AlchoholicCustomer::AlchoholicCustomer(std::string name, int id):Customer(std::move(name), id), lastOrderPrice(-1),lastId(-3),stop(false)  {
    this->lastOrderPrice = -1;
    this->lastId = -3;
    this->stop = false;
}
AlchoholicCustomer::~AlchoholicCustomer(){}
std::vector<int> AlchoholicCustomer::order(const std::vector<Dish> &menu){
    vector<int> order = vector<int>(0);
    if(stop){
        return order;
    }
    int price = -1;
    int id = -2;
    bool found = false;

    for( unsigned int i=0; i<menu.size()&&!found; i++){
        if (menu[i].getType() == ALC && (menu[i].getPrice() > lastOrderPrice ||(menu[i].getPrice() == lastOrderPrice&& menu[i].getId()>lastId))) {
            found = true;
            price = menu[i].getPrice();
            id = menu[i].getId();
        }
    }
    for (const auto &i : menu) {
        if (i.getType() == ALC && (i.getPrice() < price && (i.getPrice() > lastOrderPrice||(i.getPrice() == lastOrderPrice&& i.getId()>lastId )))) {
            price = i.getPrice();
            id = i.getId();
        }
    }
    lastOrderPrice = price;
    lastId = id;
    if(id==-2){
        this->stop=true;
        return order;
    }
    order.push_back(id);
    return order;
}
std::string AlchoholicCustomer::toString() const{
    return "alc";
}
Customer* AlchoholicCustomer::clone(){
    return new AlchoholicCustomer(*this);

}
