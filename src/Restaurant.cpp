

//
// Created by orel kakon on 04/11/2018.
//
#include "Restaurant.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <cstdlib>
#include <algorithm>
using namespace std;


Restaurant::Restaurant():open(false), tables(), menu(), actionsLog(){

}
Restaurant::Restaurant(const std::string &configFilePath):open(false), tables(), menu(), actionsLog(){
    std::string File;
    File = configFilePath;
    std::istringstream iss(File);
    for (std::string line; std::getline(iss, line);) { //cuts # comments
        if(line[0]=='#'){
            if(line.find("number of tables")!= std::string::npos ||
               line.find("tables description") != std::string::npos ||
               line.find("Menu") != std::string::npos ){

            } else{
                std::string::size_type i = File.find(line);

                if (i != std::string::npos)
                    File.erase(i, line.length());

            }
        }
    }
    size_t pos;
    while ((pos= File.find("\n\n", 0)) != std::string::npos)
    {
        File.erase(pos, 1);
    }
    std::stringstream file(File);
    std::string segment;
    std::vector<string> splitlist;
    int id=0;
    while(std::getline(file,segment,'#')){ //split the file to 3 pieces
        splitlist.push_back(segment);
    }
    string menu = splitlist[splitlist.size()-1];
    string tableD = splitlist[splitlist.size()-2];
    string numT = splitlist[splitlist.size()-3];
    numT = numT.substr(16);
    int numTA = std::stoi(numT); //num of tables in restaurant

    tableD = tableD.substr(19);
    std::stringstream TD(tableD);
    std::string segment2;
    std::vector<string> splitlist2;
    while(std::getline(TD,segment2,',')){ //splitlist2 is vector of sizes of tables
        splitlist2.push_back(segment2);
    }

    menu = menu.substr(4);
    std::stringstream M(menu);
    std::string segment3;
    std::vector<string> splitlist3;
    while(std::getline(M,segment3,'\n')){ //splitlist3 is vector of dishes
        splitlist3.push_back(segment3);
    }
    std::vector<string> splitFin;
    for(unsigned int m=1;m<splitlist3.size();m++){
        std::stringstream F(splitlist3[m]);
        while(std::getline(F,segment3,',')){ //splitlist3 is vector of dishes
            splitFin.push_back(segment3);
        }
    }
    tables = std::vector<Table*>(numTA); //maybe need new
    splitlist2[splitlist2.size()-1].erase(std::remove(splitlist2[splitlist2.size()-1].begin(),splitlist2[splitlist2.size()-1].end(),'\n'),splitlist2[splitlist2.size()-1].end());
    for(unsigned int i=0;i<((unsigned)this->getNumOfTables());i++){
        tables[i]= new Table(stoi(splitlist2[i]));
    }

    this->menu = std::vector<Dish>();
    for(unsigned int i=0;i<splitFin.size();i=i+3){
        string x = splitFin[i+1];
        DishType DT=DishType();
        if (x == "veg" || x == "VEG") {
            DT = VEG;
        } else if (x == "bvg" || x == "BVG") {
            DT = BVG;
        } else if (x == "alc" || x == "ALC") {
            DT = ALC;
        } else {
            DT = SPC;
        }
        (this->menu).emplace_back(id,splitFin[i],stoi(splitFin[i+2]),DT); //maybe need new
        id++;
    }
    this->actionsLog = std::vector<BaseAction*>(); //??
}
void Restaurant::start() {
    std::cout << "Restaurant is now open!" <<std::endl;
    int custID = 0;
    std::string Input;
    std::getline(cin, Input);
    while (Input != "closeall") {
        string Command = Input.substr(0, Input.find(' '));
        if (Command == "open") {
            int TableNum = stoi(Input.substr(Input.find(' ') + 1, 1));
            std::string rest = Input.substr(Input.find(' ') + 3);
            std::stringstream M(rest);
            std::string segment;
            std::vector<string> restSplit;
            while (std::getline(M, segment, ' ')) {
                restSplit.push_back(segment);
            }
            vector<string> names;
            vector<string> types;
            for (auto &i : restSplit) {
                string name = i.substr(0, i.find(','));
                string type = i.substr(i.find(',') + 1);
                names.push_back(name);
                types.push_back(type);
            }
            std::vector<Customer *> Cust = vector<Customer *>(); // ?! new ?!
            for (unsigned int i = 0; i < names.size(); i++) {
                if (types[i] == "veg" || types[i] == "VEG") {
                    Cust.push_back(new VegetarianCustomer(names[i], custID)); //need NEW ?
                } else if (types[i] == "chp" || types[i] == "CHP") {
                    Cust.push_back(new CheapCustomer(names[i], custID));
                } else if (types[i] == "alc" || types[i] == "ALC") {
                    Cust.push_back(new AlchoholicCustomer(names[i], custID));
                } else {
                    Cust.push_back(new SpicyCustomer(names[i], custID));
                }
                custID++;
                // memory leak
            }
            OpenTable *OT = new OpenTable(TableNum, Cust);
            OT->act(*this); //open the table;
            this->actionsLog.push_back(OT);

        }
        if (Command == "order") {
            int TableNum = stoi(Input.substr(Input.find(' ') + 1, 1));
            Order *O = new Order(TableNum);
            O->act(*this);
            actionsLog.push_back(O);
        }
        if (Command == "move") {
            int fromTableNum = stoi(Input.substr(Input.find(' ') + 1, 1));
            int toTableNum = stoi(Input.substr(Input.find(' ') + 3, 1));
            int customerId = stoi(Input.substr(Input.find(' ') + 5, 1));
            MoveCustomer *MC = new MoveCustomer(fromTableNum, toTableNum, customerId);
            MC->act(*this);
            actionsLog.push_back(MC);
        }
        if (Command == "close") {
            int TableNum = stoi(Input.substr(Input.find(' ') + 1, 1));
            Close *C =  new Close(TableNum);
            C->act(*this);
            actionsLog.push_back(C);
        }
        if (Command == "menu") {
            PrintMenu *PM = new PrintMenu();
            PM->act(*this);
            actionsLog.push_back(PM);
        }
        if (Command == "status") {
            int TableNum = stoi(Input.substr(Input.find(' ') + 1, 1));
            PrintTableStatus *PTS =new PrintTableStatus(TableNum);
            PTS->act(*this);
            actionsLog.push_back(PTS);
        }
        if (Command == "log") {
            PrintActionsLog *PAL = new PrintActionsLog();
            PAL->act(*this);
            actionsLog.push_back(PAL);
        }
        if (Command == "backup") {
            BackupRestaurant *BR = new BackupRestaurant();
            BR->act(*this);
            actionsLog.push_back(BR);

        }
        if (Command == "restore") {
            RestoreRestaurant *RR =new  RestoreRestaurant();
            RR->act(*this);
            actionsLog.push_back(RR);
        }
        std::getline(std::cin, Input);
    }
    CloseAll *CA = new CloseAll();
    CA->act(*this);
    delete CA;


}
int Restaurant::getNumOfTables() const {
    return static_cast<int>(tables.size());
}
Table* Restaurant::getTable(int ind) {
    return tables[ind]; //?? check
}
std::vector<Table*> Restaurant::getTables(){
    return this->tables;
}
const vector<BaseAction*>& Restaurant::getActionsLog() const {
    return this->actionsLog;
}
vector<Dish>& Restaurant::getMenu() {
    return this->menu;
}
void Restaurant::setActionLog(const std::vector<BaseAction*>& other){
    for(unsigned int i=0; i<actionsLog.size(); i++){
        delete actionsLog[i];
        actionsLog[i] = nullptr;
    }
    actionsLog = other;
}
// RULE OF 5:
Restaurant::~Restaurant(){
    for(unsigned int i=0; i<actionsLog.size(); i++){
        delete actionsLog[i];
        actionsLog[i] = nullptr;

    }
    for(unsigned int i=0; i<tables.size(); i++){
        delete tables[i];
        tables[i] = nullptr;

    }
    actionsLog.clear();
    tables.clear();
}
Restaurant::Restaurant(const Restaurant &aRestaurant):open(aRestaurant.open),tables(), menu(),actionsLog(){

    for (auto i : aRestaurant.actionsLog) {
        this->actionsLog.push_back(i->clone());
    }
    for (auto table : aRestaurant.tables) {
        this->tables.push_back(new Table(*(table)));

    }
    for (const auto &i : aRestaurant.menu) {
        this->menu.push_back(i);
    }



}
Restaurant &Restaurant::operator=(const Restaurant &Restaurant) {
    if(this != &Restaurant){
        this->open = Restaurant.open;
        //cout<<"was here"<<endl;
        for(unsigned int i=0; i<this->actionsLog.size();i++){
            delete (actionsLog[i]);
        }
        this->actionsLog.clear();
        for(unsigned int i = 0; i<Restaurant.actionsLog.size();i++){
            this->actionsLog.push_back(Restaurant.actionsLog[i]->clone());
        }
        for(unsigned int i=0; i<this->tables.size();i++){
            delete (tables[i]);
        }
        this->tables.clear();
        for(unsigned int i = 0; i<Restaurant.tables.size();i++){
            this->tables.push_back(new Table(*(Restaurant.tables[i])));
        }
    }
    return *this;
}
Restaurant& Restaurant::operator=(Restaurant&& Restaurant){
    if(this == &Restaurant)
        return *this;
    this->open = Restaurant.open;

    for(unsigned int i=0; i<this->actionsLog.size();i++){
        delete (actionsLog[i]);
    }
    this->actionsLog.clear();
    for(unsigned int i = 0; i<Restaurant.actionsLog.size();i++){
        this->actionsLog.push_back(Restaurant.actionsLog[i]);
    }
    for(unsigned int i = 0; i<Restaurant.actionsLog.size(); i++) {
        Restaurant.actionsLog[i] = nullptr;
    }
    for(unsigned int i=0; i<this->tables.size();i++){
        delete (tables[i]);
    }
    this->tables.clear();
    for(unsigned int i = 0; i<Restaurant.tables.size();i++){
        this->tables.push_back(new Table(*(Restaurant.tables[i])));
    }
    for(unsigned int i = 0; i<Restaurant.tables.size(); i++) {
        Restaurant.tables[i] = nullptr;
    }
    this->menu.clear();
    for(unsigned int i=0; i<Restaurant.menu.size();i++){
        this->menu.push_back(Restaurant.menu[i]);
    }
    return *this;
}
Restaurant::Restaurant(Restaurant &&other):open(other.open),tables(), menu(),actionsLog(){

    for(unsigned int i = 0; i<other.actionsLog.size();i++){
        this->actionsLog.push_back(other.actionsLog[i]);
    }
    for(unsigned int i = 0; i<other.actionsLog.size(); i++) {
        other.actionsLog[i] = nullptr;
    }
    for(unsigned int i = 0; i<other.tables.size();i++){
        this->tables.push_back(new Table(*other.tables[i]));
    }
    for(unsigned int i = 0; i<other.tables.size(); i++) {
        other.tables[i] = nullptr;
    }
    this->menu.clear();
    for(unsigned int i=0; i<other.menu.size();i++){
        this->menu.push_back(other.menu[i]);
    }

}