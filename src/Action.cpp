


#include <utility>

//
// Created by mirhaus on 27/10/18.
//
#include <string>
#include <algorithm>
#include <iostream>
#include <vector>
#include "Action.h"
#include "Restaurant.h"
extern Restaurant* backup;

using namespace std;
//BASE ACTION
BaseAction::BaseAction():errorMsg(), status(PENDING){

}
BaseAction::~BaseAction(){}
std::string BaseAction::GetErrorMsg() const { //legal ?? or illegal ?!
    return getErrorMsg();
}
ActionStatus BaseAction::getStatus() const{
    return this->status;
}
void BaseAction::complete(){
    this->status = COMPLETED;
}
void BaseAction::error(std::string errorMsg){
    this->errorMsg = std::move(errorMsg);
    this->status = ERROR;
}
std::string BaseAction::getErrorMsg() const{
    return this->errorMsg;
}
//OPEN TABLE
OpenTable::OpenTable(int id, std::vector<Customer *> &customersList):BaseAction(),tableId(id), customers(customersList){

}
OpenTable* OpenTable::clone() {
    return new OpenTable(*this);
}
OpenTable:: ~OpenTable(){
    for (auto &customer : customers) {
        delete customer;
        customer = nullptr;
    }
}
OpenTable::OpenTable(const OpenTable& other):BaseAction(),tableId(other.tableId), customers(other.customers){
    this->customers = vector<Customer*>(0);
    for(unsigned int i=0; i<other.customers.size();i++){
        this->customers.push_back(other.customers[i]->clone());
    }
}
OpenTable::OpenTable(OpenTable&& other):BaseAction(),tableId(other.tableId), customers(other.customers){
    this->customers = vector<Customer*>(0);
    for (auto customer : other.customers) {
        this->customers.push_back(customer);
    }
    for (auto &customer : other.customers) {
        customer = nullptr;
    }
}
void OpenTable::act(Restaurant &restaurant){
    if (restaurant.getNumOfTables() < tableId || restaurant.getTable(tableId)->isOpen()) {
        std::cout << "Error: Table does not exist or is already open" <<std::endl;
        this->error("Table does not exist or is already open");
    }
    else {
        (restaurant.getTable(tableId))->openTable();
        for (auto customer : customers) {
            restaurant.getTable(tableId)->addCustomer(customer);
        }
        this->complete();
    }
}
std::string OpenTable::toString() const{
    std::string cust;
    for (auto customer : customers) {
        cust += customer->getName() +","+ customer->toString()+" ";
    }
    return "open "+std::to_string(tableId)+" "+cust;
}
//ORDER
Order::Order(int id):BaseAction(), tableId(id){

}
Order::~Order() {}
Order* Order::clone() {
    return new Order(*this);
}
void Order::act(Restaurant &restaurant){
    if (restaurant.getNumOfTables() < tableId || !(restaurant.getTable(tableId))->isOpen()) {
        std::cout << "Error: Table does not exist or is not open"<<std::endl;
        this->error("Table does not exist or is not open");
    }
    else {
        int num = restaurant.getTable(tableId)->getOrders().size();
        (restaurant).getTable(tableId)->order(restaurant.getMenu()); //start the order to table
        for (unsigned int i = num ;
             i < restaurant.getTable(tableId)->getOrders().size(); i++) {
            //run the orders of this table
            std::cout<<restaurant.getTable(tableId)->getCustomer(restaurant.getTable(tableId)->getOrders()[i].first)->getName()
                     << " ordered " << restaurant.getTable(tableId)->getOrders()[i].second.getName() << endl;
        }
        this->complete();
    }
}
std::string Order::toString() const{
    return "order "+ std::to_string(tableId)+" ";
}
//MOVE CUSTOMER
MoveCustomer::MoveCustomer(int src, int dst, int customerId):BaseAction(),srcTable(src),dstTable(dst),id(customerId){

}
MoveCustomer::~MoveCustomer() {}
MoveCustomer* MoveCustomer::clone() {
    return new MoveCustomer(*this);
}
void MoveCustomer::act(Restaurant &restaurant) {
    if (restaurant.getNumOfTables() < srcTable || !restaurant.getTable(srcTable)->isOpen() ||
        restaurant.getNumOfTables() < dstTable || !restaurant.getTable(dstTable)->isOpen() ||
        restaurant.getTable(dstTable)->getCapacity() - restaurant.getTable(dstTable)->getCustomers().size() == 0 ||
        restaurant.getTable(srcTable)->getCustomer(id) == nullptr) {
        std::cout << "Error: Cannot move customer" << std::endl;
        this->error("Cannot move customer");

    } else {
        std::vector<OrderPair> op = vector<OrderPair>();
        for(unsigned int i=0;i<restaurant.getTable(srcTable)->getOrders().size();i++){
            if((restaurant.getTable(srcTable)->getOrders()[i]).first == id) {
                OrderPair OP = OrderPair(id, (restaurant.getTable(srcTable)->getOrders()[i]).second);
                restaurant.getTable(dstTable)->getOrders().push_back(OP);
            }
            else{
                //whose need to stay
                OrderPair OP = OrderPair(restaurant.getTable(srcTable)->getOrders()[i].first, (restaurant.getTable(srcTable)->getOrders()[i]).second);
                op.push_back(OP);
            }
        }

        while(!restaurant.getTable(srcTable)->getOrders().empty()){
            restaurant.getTable(srcTable)->getOrders().pop_back();
        }

        for (const auto &i : op) {
            restaurant.getTable(srcTable)->getOrders().push_back(i);
        }
        restaurant.getTable(dstTable)->addCustomer(restaurant.getTable(srcTable)->getCustomer(id));//add customer
        restaurant.getTable(srcTable)->removeCustomer(id);
        if (restaurant.getTable(srcTable)->getCustomers().empty()) {
            restaurant.getTable(srcTable)->closeTable();
            int Bill = restaurant.getTable(id)->getBill();
            std::cout << "Table " << id << " was closed. Bill " << Bill << "NIS"<<endl;
        }
        this->complete();
    }
}
std::string MoveCustomer::toString() const{
    return "move "+std::to_string(srcTable)+" "+std::to_string(dstTable)+" "+std::to_string(id)+" ";
}
//CLOSE
Close::Close(int id):BaseAction(),tableId(id) {

}
Close::~Close() {}
Close* Close::clone() {
    return new Close(*this);
}
void Close::act(Restaurant &restaurant) {
    if (restaurant.getNumOfTables() < tableId || !restaurant.getTable(tableId)->isOpen()) {
        std::cout << "Error: Table does not exist or is not open" << std::endl;
        this->error("Table does not exist or is not open");
    }
    else {
        int Bill = restaurant.getTable(tableId)->getBill();
        std::cout << "Table " << tableId << " was closed. Bill " << Bill << "NIS" << endl;
        restaurant.getTable(tableId)->closeTable();
        this->complete();
    }
}
std::string Close::toString() const{
    return "close "+ std::to_string(tableId)+" ";
}
//CLOSEALL
CloseAll::CloseAll():BaseAction() {

}
CloseAll::~CloseAll() {}
CloseAll* CloseAll::clone() {
    return new CloseAll(*this);
}
void CloseAll::act(Restaurant &restaurant) {
    bool first = false;
    int id=0;
    for (auto &it : restaurant.getTables()) {
        if(it->isOpen()){
            if(first) {
                std::cout << endl;
            }
            first = true;
            std::cout << "Table "<< id <<" was closed. Bill "<< it->getBill()<<"NIS";
            it->closeTable();
        }
        id++;
    }
    this->complete();
}
std::string CloseAll::toString() const{
    return "closeall ";
}
//PRINT MENU
PrintMenu::PrintMenu():BaseAction() {

}
PrintMenu::~PrintMenu() {}
PrintMenu* PrintMenu::clone() {
    return new PrintMenu(*this);
}
void PrintMenu::act(Restaurant &restaurant) {
    for (auto &i : restaurant.getMenu()) {
        if(i.getType()==0) {
            std::cout << i.getName() << " " << "VEG"
                      << " " << i.getPrice() << "NIS" << endl;
        }
        if(i.getType()==1) {
            std::cout << i.getName() << " " << "SPC"
                      << " " << i.getPrice() << "NIS" << endl;
        }
        if(i.getType()==2) {
            std::cout << i.getName() << " " << "BVG"
                      << " " << i.getPrice() << "NIS" << endl;
        }
        if(i.getType()==3) {
            std::cout << i.getName() << " " << "ALC"
                      << " " << i.getPrice() << "NIS" << endl;
        }
    }

    this->complete();
}
std::string PrintMenu::toString() const {
    return "menu ";
}
// STATUS
PrintTableStatus::PrintTableStatus(int id):BaseAction(),tableId(id) {

}
PrintTableStatus::~PrintTableStatus() {}
PrintTableStatus* PrintTableStatus::clone() {
    return new PrintTableStatus(*this);
}
void PrintTableStatus::act(Restaurant &restaurant) {
    if(restaurant.getTable(tableId)->isOpen()) {
        std::cout << "Table " << this->tableId << " status: open"<<endl;
        std::cout << "Customers:"<<endl;
        for (auto &i : restaurant.getTable(tableId)->getCustomers()) {
            std::cout << i->getId()<< " " << i->getName()<<endl;
        }
        std::cout << "Orders:"<<endl;
        for (auto &i : restaurant.getTable(tableId)->getOrders()) {
            std::cout << i.second.getName()<< " " << i.second.getPrice()<< "NIS "<< i.first<<endl;
        }
        std::cout << "Current Bill: " << restaurant.getTable(tableId)->getBill()<<"NIS"<<endl;
    }
    else{
        std::cout << "Table " << this->tableId << " status: closed"<<endl;
    }
    this->complete();
}
std::string PrintTableStatus::toString() const {
    return "status " + std::to_string(tableId)+" ";
}
//ACTION LOG
PrintActionsLog::PrintActionsLog():BaseAction() {

}
PrintActionsLog::~PrintActionsLog() {}
PrintActionsLog* PrintActionsLog::clone() {
    return new PrintActionsLog(*this);
}
void PrintActionsLog::act(Restaurant &restaurant) {
    for (auto i : restaurant.getActionsLog()) {
        if(i->getStatus() == COMPLETED) {
            std::cout << i->toString() << "Completed" << endl;
        }
        if(i->getStatus() == ERROR){
            std::cout << i->toString() <<"Error: "
            << i->GetErrorMsg() <<endl;
        }
    }
    this->complete();
}
std::string PrintActionsLog::toString() const {
    return  "log ";
}
//BACKUP RESTAURANT
BackupRestaurant::BackupRestaurant():BaseAction() {

}
BackupRestaurant::~BackupRestaurant() {}
BackupRestaurant* BackupRestaurant::clone() {
    return new BackupRestaurant(*this);
}
void BackupRestaurant::act(Restaurant &restaurant) {
    backup = new Restaurant(restaurant);
        this->complete();
}
std::string BackupRestaurant::toString() const {
    return "backup ";
}
//RESTORE RESTAURANT
RestoreRestaurant::RestoreRestaurant():BaseAction() {
}
RestoreRestaurant::~RestoreRestaurant() {}
RestoreRestaurant* RestoreRestaurant::clone() {
    return new RestoreRestaurant(*this);
}
void RestoreRestaurant::act(Restaurant &restaurant) {
    if (backup == nullptr){
        std::cout << "Error: No backup available" <<std::endl;
        this->error("No backup available");
    }
    else {
        restaurant = (*backup);
        this->complete();
    }
}
std::string RestoreRestaurant::toString() const {
    return "restore ";
}


