
//
// Created by orel kakon on 31/10/2018.
//

#ifndef TABLE_H_
#define TABLE_H_

#include <vector>
#include "Customer.h"
#include "Dish.h"

typedef std::pair<int, Dish> OrderPair;

class Table{
public: //need check all class
    Table(int t_capacity); // V but need check
    virtual ~Table();
    Table(const Table& orher);
    Table& operator=(const Table& other);
    Table(Table&& other);
    Table& operator=(Table&& other);
    int getCapacity() const; // V
    void addCustomer(Customer* customer); // v
    void removeCustomer(int id); // v
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers(); // v
    std::vector<OrderPair>& getOrders(); // v
    void order(const std::vector<Dish> &menu);
    void openTable(); //v
    void closeTable(); //v
    int getBill(); // V
    bool isOpen(); // V
    Table* clone();



private:
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order in a table - (customer_id, Dish)
};


#endif
