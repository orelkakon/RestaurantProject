

//
// Created by orel kakon on 31/10/2018.
//
#ifndef RESTAURANT_H_
#define RESTAURANT_H_

#include <vector>
#include <string>
#include "Dish.h"
#include "Table.h"
#include "Action.h"


class Restaurant{
public:
    Restaurant();
    Restaurant(const std::string &configFilePath);
    void start();
    int getNumOfTables() const;
    Table* getTable(int ind);
    const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    void setActionLog(const std::vector<BaseAction*>& other);
    std::vector<Dish>& getMenu();
    std::vector<Table*> getTables();
    virtual ~Restaurant(); // DCtor
    Restaurant & operator=(const Restaurant &Restaurant); //Ass OP
    Restaurant(const Restaurant &aRestaurant); //copy
    Restaurant(Restaurant &&other); //Move C
    Restaurant& operator=(Restaurant &&other); // Move ASS=


private:
    bool open;
    std::vector<Table*> tables;
    std::vector<Dish> menu;
    std::vector<BaseAction*> actionsLog;
};

#endif

