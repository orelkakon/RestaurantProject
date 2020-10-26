
//
// Created by orel kakon on 31/10/2018.
//

#ifndef ACTION_H_
#define ACTION_H_

#include <string>
#include <iostream>
#include "Customer.h"

enum ActionStatus{
    PENDING, COMPLETED, ERROR
};

//Forward declaration
class Restaurant;

class BaseAction{
public:
    std::string GetErrorMsg() const;
    BaseAction();//v
    virtual ~BaseAction();
    ActionStatus getStatus() const;//v
    virtual void act(Restaurant& restaurant) = 0;
    virtual std::string toString() const = 0;
    virtual BaseAction* clone()=0;
protected:
    void complete();
    void error(std::string errorMsg);
    std::string getErrorMsg() const;
private:
    std::string errorMsg;
    ActionStatus status;
};


class OpenTable : public BaseAction {
public:
    OpenTable(int id, std::vector<Customer *> &customersList);
    virtual ~OpenTable();
    OpenTable(const OpenTable& other);
    OpenTable& operator=(const OpenTable& other);
    OpenTable(OpenTable&& other);
    OpenTable& operator=(OpenTable&& other);
    void act(Restaurant &restaurant);
    std::string toString() const ;
    OpenTable* clone();
private:
    const int tableId;
    std::vector<Customer *> customers;
};


class Order : public BaseAction {
public:
    Order(int id);
    virtual ~Order();
    void act(Restaurant &restaurant);
    std::string toString() const;
    Order* clone();

private:
    const int tableId;
};


class MoveCustomer : public BaseAction {
public:
    MoveCustomer(int src, int dst, int customerId);
    virtual ~MoveCustomer();
    void act(Restaurant &restaurant);
    std::string toString() const;
    MoveCustomer* clone();

private:
    const int srcTable;
    const int dstTable;
    const int id;
};


class Close : public BaseAction {
public:
    Close(int id);
    virtual ~Close();
    void act(Restaurant &restaurant);
    std::string toString() const;
    Close* clone();

private:
    const int tableId;
};


class CloseAll : public BaseAction {
public:
    CloseAll();
    virtual ~CloseAll();
    void act(Restaurant &restaurant);
    std::string toString() const;
    CloseAll* clone();

private:
};


class PrintMenu : public BaseAction {
public:
    PrintMenu();
    virtual ~PrintMenu();
    void act(Restaurant &restaurant);
    std::string toString() const;
    PrintMenu* clone();

private:
};


class PrintTableStatus : public BaseAction {
public:
    PrintTableStatus(int id);
    virtual ~PrintTableStatus();
    void act(Restaurant &restaurant);
    std::string toString() const;
    PrintTableStatus* clone();

private:
    const int tableId;
};


class PrintActionsLog : public BaseAction {
public:
    PrintActionsLog();
    virtual ~PrintActionsLog();
    void act(Restaurant &restaurant);
    std::string toString() const;
    PrintActionsLog* clone();

private:
};


class BackupRestaurant : public BaseAction {
public:
    BackupRestaurant();
    virtual ~BackupRestaurant();
    void act(Restaurant &restaurant);
    std::string toString() const;
    BackupRestaurant* clone();

private:
};


class RestoreRestaurant : public BaseAction {
public:
    RestoreRestaurant();
    virtual ~RestoreRestaurant();
    void act(Restaurant &restaurant);
    std::string toString() const;
    RestoreRestaurant* clone();


};


#endif
