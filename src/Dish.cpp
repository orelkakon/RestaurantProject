

#include <utility>

//
// Created by orel kakon on 01/11/2018.
//
using namespace std;
#include "Dish.h"

Dish::Dish(int d_id, std::string d_name, int d_price, DishType d_type): id(d_id),name(std::move(d_name)),price(d_price),type(d_type) {

}

int Dish::getId() const {
    return this->id;
}
string Dish::getName() const{
    return this->name;
}
int Dish::getPrice() const {
    return this->price;
}
DishType Dish::getType() const {
    return this->type;
}

