//
// Created by mim on 24/10/20.
//

#include "SystemNetwork.h"

using namespace std;

SystemNetwork::SystemNetwork() {
    highways = new Highways();
    movements = new Movements();
    vehicles = new Vehicles();
    utils = new Utils();
    employees = new Employees();
}

void SystemNetwork::ungetstr(ifstream &f){
    char c='a';
    char c1='a';
    int n=0;
    vector<char> cv;
    while((c!=' ')&&(n<3)){
        f.unget();
        f.unget();
        f>>c;
        if(c==c1){
            n++;
        }
        c1=c;
    }
    if(n==3){
        f.unget();
    }
    for(int i=0;i<cv.size();i++){
        cout<<cv[i];
    }
}

void SystemNetwork::write(){
    vector<Toll *> t;
    vector<Lane *> l;
    ofstream f("systemNetworks.txt");

    f<<"HIGHWAYS"<<endl;
    for (int i = 0;i<highways->highways.size();i++){
        f<< "Highway nr"<<i+1<<": "<< highways->highways[i]->getName() <<endl;
        t= highways->highways[i]->tolls;
        for(int j=0;j<t.size();j++){
            f<< "  Toll nr"<<j+1<<endl;
            f<<"  "<<t[j]->getInfo()<<endl;
            l=t[j]->lanes;
            for(int k=0;k<l.size();k++){
                f<< "    Lane nr:"<<l[k]->getLaneNumber()<<endl;
                f<<"    "<<l[k]->getInfo()<<endl;
            }
        }
    }

    f<<endl<<"VEHICLES"<<endl;
    f<<"Taxes:";
    for (int j=0;j<4;j++){
        f<<" "<<vehicles->taxes[j];
    }
    f<<endl;
    for(int i=0;i<vehicles->vehicles.size();i++){
        f<<"Vehicle nr"<<i+1<<": "<<endl;
        f<<vehicles->vehicles[i]->getInfo()<<endl;
    }

    f<<endl<<"MOVEMENTS"<<endl;
    for(int i=0;i<movements->movements.size();i++){
        if(movements->movements[i]->type) {
            f<<"Movement nr"<<i+1<<": "<<endl;
            f <<movements->movements[i]->getInfo()<<endl;
        }
    }



    f.close();
}

void SystemNetwork::read(string file) {
    ifstream f(file);

    int number,code;
    bool tf,movement_type;
    string s,name,geolocal,plate,date1,date2;
    float highway_kilometer,tax1,tax2,tax3,tax4;

    Toll t;
    Lane l;

    f>>s;
    if( s=="HIGHWAYS") {
        f>>s;
        while (s == "Highway") {
            f >> s;//discard
            f >> s;
            Highway h = Highway(s);
            f >> s;
            while (s == "Toll") {//Toll
                f >> s;//discard (nr:)
                f >> name;
                f >> s;//discard (-)
                f >> geolocal;
                f >> s;//discard (-)
                f >> highway_kilometer;
                f >> s;//discard (-)
                f >> tf;
                if (tf) {
                    t = TollOut(name, geolocal, highway_kilometer);
                } else {
                    t = TollEntrance(name, geolocal, highway_kilometer);
                }
                f >> s;
                while (s == "Lane") {//Lane
                    f >> s;//discard (nr:)
                    f >> number;
                    f >> s;//discard (-)
                    f >> tf;
                    f >> s;
                    if (s == "-") {
                        f >> name;
                        f >> s;//discard (-)
                        f >> code;
                        Employee e = Employee(name, number);
                        employees->addEmployee(&e);
                        l = LaneEmployee(number, tf, &e);
                        t.addLane(&l);
                        f>>s;
                    } else {
                        l = Lane(number, tf);
                        t.addLane(&l);
                    }

                    if(s=="Lane"){
                        continue;
                    }
                    else {
                        break;
                    }

                }
                h.addToll(&t);
                if(s=="Toll"){
                    continue;
                }
                else if((s=="Highway")||(s=="VEHICLES")){
                    break;
                }
                else{
                    f>>s;
                }

            }
            highways->addHighway(&h);
            for(int i=0;i<h.getNumTolls();i++){
                cout<<h.getTollIndex(i)->getInfo()<<endl;
            }

            if(s=="Highway"){
                continue;
            }
            else if(s=="VEHICLES"){
                break;
            }
            else{
                f>>s;
            }
        }
    }


    if(s=="VEHICLES"){
        f>>s;
        f>>tax1;
        f>>tax2;
        f>>tax3;
        f>>tax4;
        vehicles->setTaxes(tax1,tax2,tax3,tax4);
        f>>s;
        while(s=="Vehicle"){
            f>>s; //discard
            f>>plate;
            f>>s; //discard
            f>>number;
            f>>s; //discard
            f>>tf;
            vehicles->addVehicle(plate,number);
            f>>s;
        }
    }

    if(s=="MOVEMENTS"){
        f>>s;
        while(s=="Movement"){
            f>>s; //discard
            f>>date1;
            f>>date2;
            s=date1+" "+date2;
            Date d = Date(s);
            f>>s; //discard
            f>>s;//movement_type(mov out sempre)
            f>>s; //discard
            f>>name;
            Highway h=*(highways->getHighway(name));
            f>>s; //discard
            f>>name;
            Toll t = *(h.getToll(name));
            f>>s; //discard
            f>>s;
            f>>s; //discard
            f>>s;
            f>>s; //discard
            f>>s;
            f>>s; //discard
            f>>number;
            f>>s; //discard
            f>>s;
            f>>s; //discard
            Lane l=*(t.getLane(number));
            if(l.getEmployee()!=nullptr){
                f>>s;
                f>>s; //discard
                f>>s;
                f>>s; //discard
            }
            name="";
            f>>s;
            name+=s;
            f>>s; //discard
            name+=s;
            f>>s;
            name+=s;
            f>>s; //discard
            name+=s;
            f>>s;
            name+=s;
            Vehicle v=*(vehicles->getVehicle(name));
            f>>s;//discard
            float distance;
            f>>distance;
            float price;
            f>>price;

            f>>s;//discard
            f>>date1;
            f>>date2;
            s=date1+" "+date2;
            Date dd = Date(s);
            f>>s; //discard
            f>>s;//movement_type(mov out sempre)
            f>>s; //discard
            f>>name;
            Highway hh=*(highways->getHighway(name));
            f>>s; //discard
            f>>name;
            Toll tt = *(h.getToll(name));
            f>>s; //discard
            f>>s;
            f>>s; //discard
            f>>s;
            f>>s; //discard
            f>>s;
            f>>s; //discard
            f>>number;
            f>>s; //discard
            f>>s;
            f>>s; //discard
            Lane ll=*(t.getLane(number));
            if(l.getEmployee()!=nullptr){
                f>>s;
                f>>s; //discard
                f>>s;
                f>>s; //discard
            }
            name="";
            f>>s;
            name+=s;
            f>>s; //discard
            name+=s;
            f>>s;
            name+=s;
            f>>s; //discard
            name+=s;
            f>>s;
            name+=s;
            Vehicle vv=*(vehicles->getVehicle(name));
            MovementEntry me=MovementEntry(&vv,&hh,&tt,&ll,&dd);
            if(!movements->addMovement(&me)){
                cout<<"write movement failed!";
            }
            MovementOut mo=MovementOut(&v,&h,&t,&l,&d,&me);
            if(!movements->addMovement(&mo)){
                cout<<"write movement failed!";
            }
            f>>s;
        }
    }

    f.close();



    /*vector<Toll *> tolls;
    vector<Lane *> lanes;

    cout<<"HIGHWAYS"<<endl;
    for (int i = 0;i<highways->highways.size();i++){
        cout<< "Highway nr"<<i+1<<": "<< highways->highways[i]->getName() <<endl;
        tolls= highways->highways[i]->tolls;
        for(int j=0;j<tolls.size();j++){
            cout<< "  Toll nr"<<j+1<<endl;
            cout<<"  "<<tolls[j]->getInfo()<<endl;
            lanes=tolls[j]->lanes;
            for(int k=0;k<lanes.size();k++){
                cout<< "    Lane nr"<<lanes[k]->getLaneNumber()<<endl;
                cout<<"    "<<lanes[k]->getInfo()<<endl;
            }
        }
    }

    cout<<endl<<"VEHICLES"<<endl;
    cout<<"Taxes:";
    for (int j=0;j<4;j++){
        cout<<" "<<vehicles->taxes[j];
    }
    cout<<endl;
    for(int i=0;i<vehicles->vehicles.size();i++){
        cout<<"Vehicle nr"<<i+1<<": "<<endl;
        cout<<vehicles->vehicles[i]->getInfo()<<endl;
    }

    cout<<endl<<"MOVEMENTS"<<endl;
    for(int i=0;i<movements->movements.size();i++){
        if(movements->movements[i]->type) {
            cout<<"Movement nr"<<i+1<<": "<<endl;
            cout<<movements->movements[i]->getInfo()<<endl;
        }
    }*/
}

void SystemNetwork::manageHighways() {
    int index;
    do {
        index = utils->ShowMenu({"Create Highway", "Update Highway", "Delete Highway", "Read Highways", "Manage Highway"});
        switch(index) {
            case 1:
                utils->clrScreen();
                createHighway();
                break;
            case 2:
                utils->clrScreen();
                updateHighway();
                break;
            case 3:
                utils->clrScreen();
                deleteHighway();
                break;
            case 4:
                utils->clrScreen();
                readHighways();
                break;
            case 5:
                utils->clrScreen();
                auto *h1 = chooseHighway();
                if (h1 == nullptr)
                    break;
                manageHighway(h1);
                break;
        }
    } while(index);
}

void SystemNetwork::manageHighway(Highway* highway) {
    int index;
    do {
        index = utils->ShowMenu({"Create Toll", "Update Toll", "Delete Toll", "Read Tolls","Manage Toll"});
        switch(index) {
            case 1:
                utils->clrScreen();
                createToll(highway);
                break;
            case 2:
                utils->clrScreen();
                updateToll(highway);
                break;
            case 3:
                utils->clrScreen();
                deleteToll(highway);
                break;
            case 4:
                utils->clrScreen();
                readTolls(highway);
                break;
            case 5:
                utils->clrScreen();
                auto *t1 = chooseToll(highway);
                if (t1 == nullptr)
                    break;
                manageToll(t1);
                break;
        }
    } while(index);
}

void SystemNetwork::manageToll(Toll *toll) {
    int index;
    do {
        index = utils->ShowMenu({"Create Lane", "Update Lane", "Delete Lane", "Read Lanes"});
        switch(index) {
            case 1:
                utils->clrScreen();
                createLane(toll);
                break;
            case 2:
                utils->clrScreen();
                updateLane(toll);
                break;
            case 3:
                utils->clrScreen();
                deleteLane(toll);
                break;
            case 4:
                utils->clrScreen();
                readLanes(toll);
                break;
        }
    } while(index);
}

void SystemNetwork::manageEmployee() {
    int index;
    do {
        index = utils->ShowMenu({"Create Employee", "Update Employee", "Delete Employee", "Read Employees"});
        switch(index) {
            case 1:
                utils->clrScreen();
                createEmployee();
                break;
            case 2:
                utils->clrScreen();
                updateEmployee();
                break;
            case 3:
                utils->clrScreen();
                deleteEmployee();
                break;
            case 4:
                utils->clrScreen();
                readEmployees();
                break;
        }
    } while(index);
}

void SystemNetwork::manageVehicle() {
    int index;
    do {
        index = utils->ShowMenu({"Register Vehicle", "Update Vehicle", "Delete Vehicle", "Read Vehicles"});
        switch(index) {
            case 1:
                utils->clrScreen();
                createVehicle();
                break;
            case 2:
                utils->clrScreen();
                updateVehicle();
                break;
            case 3:
                utils->clrScreen();
                deleteVehicle();
                break;
            case 4:
                utils->clrScreen();
                readVehicles();
                break;
        }
    } while(index);
}

void SystemNetwork::manageMovements() {
    int index;
    do {
        index = utils->ShowMenu({"Add a entry movement on a highway", "Add a exit movement of the highway", "Read Movements"});
        switch(index) {
            case 1:
                utils->clrScreen();
                addEntryMovement();
                break;
            case 2:
                utils->clrScreen();
                addExitMovement();
                break;
            case 3:
                utils->clrScreen();
                manageReadMovements();
                break;
        }
    } while(index);
}

void SystemNetwork::manageReadMovements() {
    int index;
    Highway* a1;
    Toll* t1;
    do {
        index = utils->ShowMenu({"All movements", "Highway movements", "Toll movements", "Vehicle Movements", "Employee Movements"});
        switch (index) {
            case 1:
                utils->clrScreen();
                manageMovementsHighway();
                //cout << index << " asdasd" << endl;
                break;
            case 2:
                utils->clrScreen();
                manageMovementsToll();
                break;
            case 3:
                utils->clrScreen();
                manageMovementsLane();
                break;
            case 4:
                utils->clrScreen();
                manageMovementsCar();
                break;
            case 5:
                utils->clrScreen();
                showEmployeeMovements();
                break;
        }
    } while (index);
}

void SystemNetwork::manageMovementsHighway() {
    int index;
    do {
        index = utils->ShowMenu({"By highway name", "By price", "By date"});
        switch (index) {
            case 1:
                utils->clrScreen();
                showMovementsByHighwayName();
                break;
            case 2:
                utils->clrScreen();
                showMovementsByHighwayPrice();
                break;
            case 3:
                utils->clrScreen();
                showMovementsByDate();
                break;
        }
    } while (index);
}

void SystemNetwork::manageMovementsToll() {
    int index;
    Highway* a1;
    do {
        index = utils->ShowMenu({"By toll name", "By toll worth", "By toll type"});
        switch (index) {
            case 1:
                utils->clrScreen();
                a1 = chooseHighway();
                if (a1 == nullptr) break;
                showMovementsByTollName(a1);
                break;
            case 2:
                utils->clrScreen();
                a1 = chooseHighway();
                if (a1 == nullptr) break;
                showMovementsByTollPrice(a1);
                break;
            case 3:
                utils->clrScreen();
                a1 = chooseHighway();
                if (a1 == nullptr) break;
                showMovementsByTollType(a1);
                break;
        }
    } while (index);
}

void SystemNetwork::manageMovementsLane() {
    int index;
    Highway* a1;
    Toll* t1;
    do {
        index = utils->ShowMenu({"By lane number", "By lane price"});
        switch (index) {
            case 1:
                utils->clrScreen();
                a1 = chooseHighway();
                if (a1 == nullptr) break;
                t1 = chooseToll(a1);
                if (t1 == nullptr) break;
                showMovementsbyLaneNumber(t1);
                break;
            case 2:
                utils->clrScreen();
                a1 = chooseHighway();
                if (a1 == nullptr) break;
                t1 = chooseToll(a1);
                if (t1 == nullptr) break;
                showMovementsbyLanePrice(t1);
                break;
        }
    } while(index);
}

void SystemNetwork::manageMovementsCar() {
    int index;
    do {
        index = utils->ShowMenu({"By price", "By car distance", "By date"});
        switch (index) {
            case 1:
                utils->clrScreen();
                showCarMovementsbyPrice();
                break;
            case 2:
                utils->clrScreen();
                showCarMovementsbyDistance();
                break;
            case 3:
                utils->clrScreen();
                showCarMovementsbyDate();
                break;
        }
    } while(index);
}

void SystemNetwork::manageStatistics() {
    int index;
    do {
        index = utils->ShowMenu({"Best Worth Highway", "Best Worth Toll", "Best Worth Lane",
                                 "Vehicle that spend most money","Highway with more movements", "Toll with more movements",
                                 "Lane with more movements"});
        switch (index) {
            case 1:
                utils->clrScreen();
                BestWorthHighway();
                break;
            case 2:
                utils->clrScreen();
                BestWorthToll();
                break;
            case 3:
                utils->clrScreen();
                BestWorthLane();
                break;
            case 4:
                utils->clrScreen();
                carSpentMoreMoney();
                break;
            case 5:
                utils->clrScreen();
                HighwayMoreMoves();
                break;
            case 6:
                utils->clrScreen();
                TollMoreMoves();
                break;
            case 7:
                utils->clrScreen();
                LaneMoreMoves();
                break;
        }
    } while(index);
}

void SystemNetwork::createHighway() {
    string s_name;
    while (s_name != "EXIT") {
        cout << "Input the highway name: (if you want to exit without create any highway please input EXIT)" << endl;
        getline(cin, s_name);
        if (s_name != "EXIT" && highways->addHighway(s_name)) {
            cout << "Highway created with success!" << endl;
            utils->waitForInput();
            break;
        }
        cout << "ERROR: name of highway already exist." << endl;
    }
}

void SystemNetwork::readHighways() {
    cout << "Highways: " << highways->getNumHighways() << endl;

    for (int i = 0; i < highways->getNumHighways(); i++) {
        cout << i+1 << " - " << highways->getHighwayIndex(i)->getName() << endl;
    }
    utils->waitForInput();
}

void SystemNetwork::updateHighway() {
    string s_index, s_name, s_menu;
    int index;
    index = chooseIndexHighway();
    if (index > -1)
        while (s_name != "EXIT") {
            cout << "Input the highway name: (if you want to exit without create any highway please input EXIT)" << endl;
            getline(cin, s_name);
            if (s_name != "EXIT" && highways->checkHighwayName(s_name)) {
                highways->getHighwayIndex(index)->setName(s_name);
                cout << "Highway updated with success!" << endl;
                utils->waitForInput();
                break;
            }
            cout << "ERROR: name of highway already exist." << endl;
        }
}

void SystemNetwork::deleteHighway() {
    string s_index, s_name;
    int index;
    index = chooseIndexHighway();
    if (index > -1)
        if (highways->removeHighway(index)) {
            cout << "Highway deleted with success!" << endl;
            utils->waitForInput();
        }
}

Highway* SystemNetwork::chooseHighway() {
    int index;
    index = chooseIndexHighway();
    cout << index << endl;
    while((index < -1)) {
        cout << "Invalid Number." << endl;
        index = utils->getNumber(highways->getNumHighways()) -1;
    }
    if (index == -1) {
        cout << "certo" << endl;
        return nullptr;}
    return highways->getHighwayIndex(index);
}

int SystemNetwork::chooseIndexHighway() const {
    string s_menu;
    vector<string> v1;
    for (int i = 0; i < highways->getNumHighways(); i++) {
        s_menu = to_string(i+1) + " - " + highways->getHighwayIndex(i)->getName();
        v1.push_back(s_menu);
    }
    return utils->ShowMenu(v1)-1;
}

void SystemNetwork::createToll(Highway *highway) {
    string s_name, s_geolocal;
    float kilometer;
    int i_type;
    bool type;
    while (!(s_name == "EXIT" || kilometer == -1 || i_type == 0)) {
        cout << "Input the toll's name: (if you want to exit without create any toll please input EXIT)" << endl;
        getline(cin, s_name);
        if (s_name != "EXIT" && highway->checkTollName(s_name)) {
            cout << "Input the toll's geographic location: (if you want to exit without create any toll please input EXIT)"<< endl;
            getline(cin, s_geolocal);
            if (s_geolocal != "EXIT") {
                cout << "Input the toll's highway kilometer: (if you want to exit without create any toll please input EXIT)"<< endl;
                kilometer = utils->getFloat();
                if (kilometer != -1) {
                    while(!(i_type == 1 || i_type == 2 || i_type == 0)) {
                        i_type = utils->ShowMenu({"Entrance - type","Exit - type"});
                    }
                    type = (i_type == 2);
                    if (i_type != 0) {
                        highway->addToll(s_name, s_geolocal, kilometer, type);
                        cout << "Toll created!" << endl;
                        utils->waitForInput();
                        break;
                    }
                }
            }
        }
        if(!(s_name == "EXIT" || kilometer == -1 || i_type == 0))
            cout << "ERROR: name of highway already exist." << endl;
    }
}



void SystemNetwork::readTolls(Highway* highway) {
    string s_type;
    cout << "Tolls: " << highway->getNumTolls() << endl;
    for (size_t i = 0; i < highway->getNumTolls(); i++) {
        s_type = highway->getTollIndex(i)->getType() ? "exit" : "entrance";
        cout << i+1 << " - " << highway->getTollIndex(i)->getName() << " " << highway->getTollIndex(i)->getGeolocal() << " " << highway->getTollIndex(i)->getKilometer() << " " + s_type << endl;
    }
    utils->waitForInput();
}

void SystemNetwork::updateToll(Highway *highway) {
    int index, toll_index;
    float kilometer;
    string s_name;
    toll_index = chooseIndexToll(highway);
    index = utils->ShowMenu({"Toll's name","Toll's geographic location", "Toll's highway kilometer", "Toll's type"});
    switch (index) {
        case 1:
            while (s_name != "EXIT") {
                cout << "Input the toll name: (if you want to exit without create any toll please input EXIT)" << endl;
                getline(cin, s_name);
                if (s_name != "EXIT" && highway->checkTollName(s_name)) {
                    highway->getTollIndex(toll_index)->setName(s_name);
                    cout << "Toll's name updated with success!" << endl;
                    utils->waitForInput();
                    break;
                }
                if (s_name!="EXIT")
                    cout << "ERROR: toll's name already exist." << endl;
            }
            break;
        case 2:
            cout << "Input the toll's geographic location: (if you want to exit without update any toll please input EXIT)" << endl;
            getline(cin, s_name);
            if (s_name != "EXIT") {
                highway->getTollIndex(toll_index)->setGeolocal(s_name);
                cout << "Toll's geographic location updated with success!" << endl;
                utils->waitForInput();
                break;
            }
            break;
        case 3:
            kilometer = utils->getFloat();
            if (kilometer != -1)
                highway->getTollIndex(toll_index)->setKilometer(kilometer);
            break;
        case 4:
            if (highway->getTollIndex(toll_index)->getNumLanes() == 0) {
                cout << "Not possible to update toll's type because the toll have at least one lane" << endl;
                utils->waitForInput();
            }
            else {
                do {
                    index = utils->ShowMenu({"Entrance", "Exit"});
                } while (!(index == 1 || index == 2 || index == 0));
                if (index != 0) {
                    highway->getTollIndex(toll_index)->setType(index == 2);
                    cout << "Toll's type updated with success!" << endl;
                    utils->waitForInput();
                }
            }
    }
}

void SystemNetwork::deleteToll(Highway *highway) {
    int toll_index;
    toll_index = chooseIndexToll(highway);
    if (highway->removeToll(toll_index)) {
        cout << "Toll removed with success!" << endl;
        utils->waitForInput();
    }
}

Toll * SystemNetwork::chooseToll(Highway* highway) {
    int index;
    index = chooseIndexToll(highway);
    while((index < -1)) {
        cout << "Invalid Number." << endl;
        index = utils->getNumber(highway->getNumTolls()) -1;
    }
    if (index == -1)
        return nullptr;
    return highway->getTollIndex(index);
}

int SystemNetwork::chooseIndexToll(Highway* highway) const {
    string s_type, s_menu;
    vector<string> v1;
    cout << "Tolls: " << highway->getNumTolls() << endl;
    for (size_t i = 0; i < highway->getNumTolls(); i++) {
        s_type = highway->getTollIndex(i)->getType() ? "exit" : "entrance";
        s_menu = to_string(i+1) + " - " + highway->getTollIndex(i)->getName() + " " + highway->getTollIndex(i)->getGeolocal() + " " + to_string(highway->getTollIndex(i)->getKilometer()) + " " + s_type;
        v1.push_back(s_menu);
    }
    return utils->ShowMenu(v1)-1;
}

void SystemNetwork::createLane(Toll *toll) {
    int index;
    if (!toll->getType()) {
        toll->addLane();
        cout << "Lane created with success!" << endl;
        utils->waitForInput();
    }
    else {
        index = utils->ShowMenu({"Normal lane", "Green lane"});
        if (index == 2) {
            toll->addLane();
            cout << "Lane created with success!" << endl;
            utils->waitForInput();
        }
        else if (index == 1) {
            if (employees->getNumEmployees() == 0) {
                cout << "ERROR: It's necessary to have at least one employee to add a normal lane in a exit toll." << endl;
                utils->waitForInput();
            }
            else {
                readEmployees();
                index = utils->getNumber(employees->getNumEmployees())-1;
                if (index > -1) {
                    toll->addLane(employees->getEmployeeIndex(index));
                    cout << "Lane created with success!" << endl;
                    utils->waitForInput();
                }
            }
        }
    }
}

void SystemNetwork::readLanes(Toll *toll) {
    string s_type;
    cout << "Lanes: " << toll->getNumLanes() << endl;
    for (size_t i = 0; i < toll->getNumLanes(); i++) {
        s_type = toll->getLane(i)->getGreenLane() ? "Green lane" : "Normal lane";
        if (toll->getLane(i)->getEmployee() == nullptr)
            cout << i+1 << " - " << toll->getLane(i)->getLaneNumber() << " " << s_type << endl;
        else
            cout << i+1 << " - " << toll->getLane(i)->getLaneNumber() << " " << s_type << " " << toll->getLane(i)->getEmployee()->getName() << " " << toll->getLane(i)->getEmployee()->getCode() << endl;
    }
    utils->waitForInput();
}

void SystemNetwork::updateLane(Toll *toll) {
    int lane_index, index;
    if (toll->getType()) {
        lane_index = chooseIndexLane(toll);
        if (toll->getLane(lane_index)->getGreenLane()) {
            index = utils->ShowMenu({"Green to normal lane"});
            if (index == 1) {
                index = chooseIndexEmployee();
                toll->setGreenLaneFalse(toll->getLane(lane_index),employees->getEmployeeIndex(index));
                cout << "Lane updated with success!" << endl;
                utils->waitForInput();
            }
        }
        else {
            index = utils->ShowMenu({"Normal to GreenLane", "Lane's employee"});
            if (index == 1) {
                toll->setGreenLaneTrue(toll->getLane(lane_index));
                cout << "Lane updated with success!" << endl;
                utils->waitForInput();
            }
            else if (index == 2) {
                index = chooseIndexEmployee();
                if (index > -1) {
                    toll->getLane(lane_index)->setEmployee(employees->getEmployeeIndex(index));
                    cout << "Lane's employee updated with success!" << endl;
                    utils->waitForInput();
                }
            }
        }
    }
    else {
        cout << "This is a entrance toll so there is nothing to update." << endl;
    }
}

void SystemNetwork::deleteLane(Toll *toll) {
    int lane_index;
    lane_index = chooseIndexLane(toll);
    if (toll->removeLane(lane_index)) {
        cout << "Toll removed with success!" << endl;
        utils->waitForInput();
    }
}

Lane* SystemNetwork::chooseLane(Toll* toll) {
    int index, min ,counter = 0;
    index = chooseIndexLane(toll);
    while((index < -1)) {
        cout << "Invalid Number." << endl;
        index = utils->getNumber(toll->getNumLanes()) -1;
    }
    if (index == -1)
        return nullptr;
    return toll->getLane(index);
}

int SystemNetwork::chooseIndexLane(Toll* toll) const {
    string s_type, s_menu;
    vector<string> v1;
    cout << "Lanes: " << toll->getNumLanes() << endl;
    for (size_t i = 0; i < toll->getNumLanes(); i++) {
        s_type = toll->getLane(i)->getGreenLane() ? "Green lane" : "Normal lane";
        if (toll->getLane(i)->getEmployee() == nullptr)
            s_menu = to_string(i+1) + " - " + to_string(toll->getLane(i)->getLaneNumber()) + " " + s_type;
        else
            s_menu =  to_string(i+1) + " - " + to_string(toll->getLane(i)->getLaneNumber()) + " " + s_type + " " + toll->getLane(i)->getEmployee()->getName() + " " + to_string(toll->getLane(i)->getEmployee()->getCode());
        v1.push_back(s_menu);
    }
    return utils->ShowMenu(v1)-1;
}

void SystemNetwork::createEmployee() {
    string s_name;
    while (s_name != "EXIT") {
        cout << "Input the employee name: (if you want to exit without create any employee please input EXIT)" << endl;
        getline(cin, s_name);
        if (s_name != "EXIT" && employees->addEmployee(s_name)) {
            cout << "employee created with success!" << endl;
            utils->waitForInput();
            break;
        }
        if (s_name!= "EXIT")
            cout << "ERROR: name of employee already exist." << endl;
    }
}

void SystemNetwork::updateEmployee() {
    string s_index, s_name;
    int index;
    index = chooseIndexEmployee();
    if (index > -1)
        while (s_name != "EXIT") {
            cout << "Input the employee name: (if you want to exit without create any employee please input EXIT)" << endl;
            getline(cin, s_name);
            if (s_name != "EXIT" && employees->checkEmployeeName(s_name)) {
                employees->getEmployeeIndex(index)->setName(s_name);
                cout << "Employee updated with success!" << endl;
                utils->waitForInput();
                break;
            }
            if (s_name != "EXIT")
                cout << "ERROR: name of employee already exist." << endl;
        }
}

void SystemNetwork::deleteEmployee() {
    int index;
    index = chooseIndexEmployee();
    if (index != -1) {
        employees->removeEmployee(index);
        cout << "Employee deleted with success!" << endl;
        utils->waitForInput();
    }
}

void SystemNetwork::readEmployees() {

    cout << "Employees: " << employees->getNumEmployees() << endl;

    for (int i = 0; i <employees->getNumEmployees(); i++) {
        cout << i+1 << " - " << employees->getEmployeeIndex(i)->getName() << " " << employees->getEmployeeIndex(i)->getCode() << endl;
    }
    utils->waitForInput();
}

int SystemNetwork::chooseIndexEmployee() const {
    string s_menu;
    vector<string> v1;
    cout << "Employees: " << employees->getNumEmployees() << endl;
    for (int i = 0; i <employees->getNumEmployees(); i++) {
        s_menu = to_string(i+1) + " - " + employees->getEmployeeIndex(i)->getName() + " " + to_string(employees->getEmployeeIndex(i)->getCode());
        v1.push_back(s_menu);
    }
    return utils->ShowMenu(v1)-1;
}

void SystemNetwork::createVehicle() {
    string s_plate;
    int v_class, greenlane;
    while (s_plate != "EXIT") {
        cout << "Input the vehicle plate: (if you want to exit without create any vehicle please input EXIT)" << endl;
        getline(cin, s_plate);
        if (s_plate != "EXIT") {

            cout << "Choose one of the options of vehicle class: " << endl;
            v_class = utils->ShowMenu({"Classe 1 - Motas", "Classe 2 - Light vehicle (passengers or goods)", "Class 3 - Bus", "Class 4 - Heavy goods vehicle "});
            if (v_class != 0) {
                cout << "Choose one of the options: " << endl;
                greenlane = utils->ShowMenu({"To travel on Green lanes or Normal lanes", "To travel just on Normal lanes"});
                if (greenlane != 0) {
                    if (greenlane == 1) {
                        vehicles->addVehicle(s_plate,v_class,true);
                    }
                    else {
                        vehicles->addVehicle(s_plate,v_class);
                    }
                    cout << "Vehicle created with success!" << endl;
                    utils->waitForInput();
                    break;
                }
            }

        }
        if (!(s_plate == "EXIT" || v_class == 0 || greenlane == 0))
            cout << "ERROR: name of employee already exist." << endl;
    }
}

void SystemNetwork::updateVehicle() {
    string s_index, s_plate;
    int v_index,index;
    v_index = chooseIndexVehicle();
    if (v_index > -1) {
        index = utils->ShowMenu({"Plate", "Vehicle class", "Type of lanes to travel"});
        switch (index) {
            case 1:
                while (s_plate != "EXIT") {
                    cout
                            << "Input the vehicle plate: (if you want to exit without create any vehicle please input EXIT)"
                            << endl;
                    getline(cin, s_plate);
                    if (s_plate != "EXIT" && vehicles->checkPlate(s_plate)) {
                        vehicles->getVehicleIndex(v_index)->setPlate(s_plate);
                        cout << "Vehicle updated with success!" << endl;
                        utils->waitForInput();
                        break;
                    }
                    if (s_plate != "EXIT")
                        cout << "ERROR: name of employee already exist." << endl;
                }
                break;
            case 2:
                cout << "Choose one of the options of vehicle class: " << endl;
                index = utils->ShowMenu({"Classe 1 - Motas", "Classe 2 - Light vehicle (passengers or goods)", "Class 3 - Bus", "Class 4 - Heavy goods vehicle "});
                if (index != 0) {
                    vehicles->getVehicleIndex(v_index)->setVehicleClass(index);
                    cout << "Vehicle updated with success!" << endl;
                    utils->waitForInput();
                }
                break;
            case 3:
                cout << "Choose one of the options: " << endl;
                index = utils->ShowMenu({"To travel on Green lanes or Normal lanes", "To travel just on Normal lanes"});
                if (index == 1) {
                    vehicles->getVehicleIndex(v_index)->setGreenLaneTrue();
                    cout << "Vehicle updated with success!" << endl;
                    utils->waitForInput();
                }
                else if (index == 2) {
                    vehicles->getVehicleIndex(v_index)->setGreenLaneFalse();
                    cout << "Vehicle updated with success!" << endl;
                    utils->waitForInput();
                }
                break;
        }
    }
}

void SystemNetwork::deleteVehicle() {
    int index;
    index = chooseIndexVehicle();
    if (index > -1) {
        vehicles->removeVehicle(index);
        cout << "Vehicle deleted with success!" << endl;
        utils->waitForInput();
    }
}

void SystemNetwork::readVehicles() {
    string string1;
    cout << "Vehicles: " << vehicles->getNumVehicles() << endl;
    for (int i = 0; i < vehicles->getNumVehicles(); i++) {
        if (vehicles->getVehicleIndex(i)->getGreenLaneBool())
            string1 = "GreenLane";
        else string1 = "NonGreenLane";
        cout << i+1 << " - " << vehicles->getVehicleIndex(i)->getPlate() << " " << vehicles->getVehicleIndex(i)->getClass() << " " << string1 << " " << vehicles->getVehicleIndex(i)->getTax()<< endl;
    }
}

int SystemNetwork::chooseIndexVehicle() const {
    string string1, s_menu;
    vector<string> v1;
    cout << "Vehicles: " << vehicles->getNumVehicles() << endl;
    for (int i = 0; i < vehicles->getNumVehicles(); i++) {
        if (vehicles->getVehicleIndex(i)->getGreenLaneBool())
            string1 = "Exit";
        else string1 = "Entrance";
        s_menu = to_string(i+1) + " - " + vehicles->getVehicleIndex(i)->getPlate() + " " + to_string(vehicles->getVehicleIndex(i)->getClass()) + " " + string1 + " " + to_string(vehicles->getVehicleIndex(i)->getTax());
        v1.push_back(s_menu);
    }
    return utils->ShowMenu(v1)-1;
}

void SystemNetwork::getTaxesFromUser() {
    float taxe[4];
    for (int i = 0; i < 4; i++) {
        cout << "Input the tax (euro per kilometer) for the class " << i+1 << endl;
        float a = utils->getFloat();
        if (a == -1)
            break;
        else taxe[i] = a;
    }
    vehicles->setTaxes(taxe[0],taxe[1],taxe[2],taxe[3]);
}

int SystemNetwork::adviceEntryLane(Toll * toll, Date * date) {
    int min = -1, counter = 0, index;
    for (size_t i = 0; i < toll->getNumLanes(); i++) {
        counter = 0;
        for (size_t j = 0; j < movements->getNumMovements(); j++) {
            if (*movements->getMovementIndex(j)->getToll() ==  *toll) {
                if (*(movements->getMovementIndex(j)->getDate()) == *date)
                    if (*movements->getMovementIndex(j)->getLane() == *toll->getLane(i))
                    counter++;
            }
        }
        if (min == -1) {
            min = counter;
            index = i;
        }
        else if (counter < min) {
            min = counter;
            index = i;
        }
    }
    return index;
}

int SystemNetwork::adviceOutLane(Vehicle* vehicle, Toll * toll, Date * date) {
    int min = -1, counter = 0, index;
    for (size_t i = 0; i < toll->getNumLanes(); i++) {
        if (!vehicle->getGreenLaneBool() && toll->getLane(i)->getGreenLane()) {
            continue;
        }
        counter = 0;
        for (size_t j = 0; j < movements->getNumMovements(); j++) {
            if (*movements->getMovementIndex(j)->getToll() ==  *toll) {
                if (*(movements->getMovementIndex(j)->getDate()) == *date)
                    if (*movements->getMovementIndex(j)->getLane() == *toll->getLane(i))
                        counter++;
            }
        }
        if (min == -1) {
            min = counter;
            index = i;
        }
        else if (counter < min) {
            min = counter;
            index = i;
        }
    }
    return index;
}

void SystemNetwork::addEntryMovement() {

    string s_plate;
    int index = -1, lane_index;
    Vehicle* vehicle = nullptr;

    Highway* highway = nullptr;
    Toll *toll = nullptr;

    time_t timer = time(0);
    tm *now = localtime(&timer);
    string s_date = to_string(now->tm_mday) + "/" + to_string(now->tm_mon) + "/" + to_string(now->tm_year+1900)  + " " + to_string(now->tm_hour) + ":" + to_string(now->tm_min) + ":" + to_string(now->tm_sec);
    Date* date = new Date(s_date);
    while (s_plate != "EXIT" && index != 0) {
        if (vehicles->getTaxes(1) == -1 || vehicles->getTaxes(2) == -1 || vehicles->getTaxes(3) == -1 || vehicles->getTaxes(4) == -1) {
            cout << "ERROR: Before this you have to input the taxes for the vehicle's classes" << index << endl;
            s_plate = "EXIT";
            utils->waitForInput();
            continue;
        }
        cout << "Please input the plate of the vehicle: (if you want to exit without create any vehicle please input EXIT)" << endl;
        getline(cin, s_plate);
        while (s_plate.empty()) {
            cout << "ERROR: The plate can't be a empty string." << endl;
            getline(cin, s_plate);
        }
        if (s_plate == "EXIT") continue;
        if (!vehicles->checkPlate(s_plate)) {
            cout << "Vehicle is not registed. To advance input vehicle class" << endl;
            index = utils->ShowMenu({"Classe 1 - Motas", "Classe 2 - Light vehicle (passengers or goods)", "Class 3 - Bus", "Class 4 - Heavy goods vehicle "});
            if (index != 0)
                vehicle = new Vehicle(s_plate,index,vehicles->getTaxes(index));
            else continue;
        }
        else {
            for (int i = movements->getNumMovements() - 1; i > -1; i--) {
                if (movements->getMovementIndex(i)->getVehicle()->getPlate() == s_plate) {
                    if (movements->getMovementIndex(i)->getType()) {
                        cout << "entrei" << endl;
                        vehicle = vehicles->getVehicle(s_plate);
                    }
                    break;
                }
            }
        }
        if (vehicle == nullptr) {
            cout << "ERROR: This vehicle is already in a highway." << endl;
            continue;
        }
        //else vehicle = vehicles->getVehicle(s_plate);
        do {
            highway = chooseHighway();
            if (highway == nullptr) {
                s_plate = "EXIT";
                utils->waitForInput();
                break;
            }
            if (highway->getNumTolls() == 0) {
                cout << "ERROR: This highway don't have any toll." << endl;
            }
        } while (highway->getNumTolls() == 0);
        if (s_plate == "EXIT") continue;
        do {
            toll = chooseToll(highway);
            if (toll == nullptr) {
                s_plate = "EXIT";
                utils->waitForInput();
                break;
            }
            if (toll->getType()) {
                cout << "ERROR: This is a exit toll." << endl;
            }
            if (toll->getNumLanes() == 0) {
                cout << "ERROR: This toll don't have any lane." << endl;
            }
        } while(toll->getNumLanes() == 0 || toll->getType());
        if (s_plate == "EXIT") continue;
        lane_index = adviceEntryLane(toll, date);
        cout << "Our advice: Lane " << lane_index << ". (Lane with less traffic)" << endl;
        Lane *lane = chooseLane(toll);
        if (lane == nullptr) {
            s_plate = "EXIT";
            continue;
        }

        auto* m1 = new MovementEntry(vehicle, highway, toll, lane, date);
        movements->addMovement(m1);
        break;
    }
}

void SystemNetwork::addExitMovement() {
    string s_plate;
    int lane_index;
    Movement* entry = nullptr;
    Highway* highway = nullptr;
    Toll *toll = nullptr;
    Lane *lane = nullptr;
    while (s_plate != "EXIT") {
        if (movements->getNumMovements() == 0) {
            cout << "ERROR: Before you do this you have to add a entry movement" << endl;
            utils->waitForInput();
            s_plate = "EXIT";
            continue;
        }
        cout << "Please input the plate of the vehicle: (if you want to exit without create any vehicle please input EXIT)" << endl;
        getline(cin, s_plate);
        while (s_plate.empty()) {
            cout << "ERROR: The plate can't be a empty string." << endl;
            getline(cin, s_plate);
        }
        if (s_plate == "EXIT") continue;
        //cout << movements->getNumMovements() << endl;
        for (int i = movements->getNumMovements() - 1; i > -1; i-- ) {
            cout << "entrei" << endl;
            cout << movements->getMovementIndex(i)->getDate()->getInfo();
            if (s_plate == movements->getMovementIndex(i)->getVehicle()->getPlate()) {
                if (!movements->getMovementIndex(i)->getType()) {
                    entry = movements->getMovementIndex(i);
                    break;
                }
            }
        }
        if (entry == nullptr) {
            cout << "ERROR: Vehicle isn't in any highway." << endl;
            utils->waitForInput();
            continue;
        }
        highway = entry->getHighway();
        do {
            toll = chooseToll(highway);
            if (toll == nullptr) {
                s_plate = "EXIT";
                break;
            }
            if (!toll->getType()) {
                cout << "ERROR: This is a entrance toll." << endl;
            }
            if (toll->getNumLanes() == 0) {
                cout << "ERROR: This toll don't have any lane." << endl;
            }
        } while(toll->getNumLanes() == 0 || !toll->getType());
        if (s_plate == "EXIT") continue;
        time_t timer = time(0);
        tm *now = localtime(&timer);
        string s_date = to_string(now->tm_mday) + "/" + to_string(now->tm_mon) + "/" + to_string(now->tm_year+1900)  + " " + to_string(now->tm_hour) + ":" + to_string(now->tm_min) + ":" + to_string(now->tm_sec);
        cout << s_date << endl;
        Date* date = new Date(s_date);
        lane_index = adviceOutLane(entry->getVehicle(), toll, date);
        cout << "Our advice: Lane " << lane_index << ". (Lane with less traffic)" << endl;
        do {
            lane = chooseLane(toll);
            if (lane == nullptr) {
                s_plate = "EXIT";
                break;
            }
            if (!entry->getVehicle()->getGreenLaneBool() && lane->getGreenLane()) {
                cout << "ERROR: This is a green lane and your car is a non green lane." << endl;
            }
        } while(!entry->getVehicle()->getGreenLaneBool() && lane->getGreenLane());

        if (lane == nullptr) {
            s_plate = "EXIT";
            utils->waitForInput();
            continue;
        }

        auto *m1 = new MovementOut(entry->getVehicle(),highway,toll,lane,date,entry);
        movements->addMovement(m1);
        break;
    }
}

void SystemNetwork::showMovementsByHighwayName() {
    //if (highways->getNumHighways() == 0) throw exception;
    if (movements->getNumMovements() == 0)
        cout << "There are no Highways" << endl;
    if (movements->getNumMovements() == 0)
        cout << "There are no movements" << endl;
    vector<Movement*> v1 = movements->getMovements();
    sort(v1.begin(),v1.end(),[](Movement* m1,Movement* m2){
        return (m1->getHighway()->getName() < m2->getHighway()->getName());
    });
    for (size_t i = 0; i < v1.size(); i++) {
        cout << i+1 << " - " << v1[i]->getInfo() << endl;
    }
}

void SystemNetwork::showMovementsByHighwayPrice() {
    //if (highways->getNumHighways() == 0) throw exception;
    if (movements->getNumMovements() == 0)
        cout << "There are no Highways" << endl;
    if (movements->getNumMovements() == 0)
        cout << "There are no movements" << endl;
    vector<Movement*> v1;
    for (size_t i = 0; i < movements->getNumMovements(); i++) {
        if (movements->getMovementIndex(i)->getType()==1)
            v1.push_back(movements->getMovementIndex(i));
    }
    sort(v1.begin(),v1.end(),[](Movement* m1,Movement* m2){
        return (m1->getPrice() > m2->getPrice());
    });
    for (size_t i = 0; i < v1.size(); i++) {
        cout << i+1 << " - " << v1[i]->getInfo() << endl;
    }
}

void SystemNetwork::showMovementsByDate() {
    vector<Movement*> v1 = movements->getMovements();
    sort(v1.begin(),v1.end(),[](Movement* m1,Movement* m2){
        return (*(m1->getDate()) > *(m2->getDate()));
    });
    for (size_t i = 0; i < v1.size(); i++) {
        cout << i+1 << " - " << v1[i]->getInfo() << endl;
    }
}

void SystemNetwork::showMovementsByTollName(Highway* highway) {
    vector<Movement*> v1;
    for (size_t i = 0; i < movements->getNumMovements(); i++) {
        if (movements->getMovementIndex(i)->getHighway()==highway)
            v1.push_back(movements->getMovementIndex(i));
    }
    sort(v1.begin(),v1.end(),[](Movement* m1,Movement* m2){
        return (m1->getToll()->getName() < m2->getToll()->getName());
    });
    for (size_t i = 0; i < v1.size(); i++) {
        cout << i+1 << " - " << v1[i]->getInfo() << endl;
    }
}

void SystemNetwork::showMovementsByTollPrice(Highway *highway) {
    vector<Movement*> v1;
    for (size_t i = 0; i < movements->getNumMovements(); i++) {
        if (movements->getMovementIndex(i)->getHighway()==highway && movements->getMovementIndex(i)->getType())
            v1.push_back(movements->getMovementIndex(i));
    }
    sort(v1.begin(),v1.end(),[](Movement* m1,Movement* m2){
        return (m1->getPrice() > m2->getPrice());
    });
    for (size_t i = 0; i < v1.size(); i++) {
        cout << i+1 << " - " << v1[i]->getInfo() << endl;
    }
}

void SystemNetwork::showMovementsByTollType(Highway *highway) {
    vector<Movement*> v1;
    for (size_t i = 0; i < movements->getNumMovements(); i++) {
        if (movements->getMovementIndex(i)->getHighway()==highway)
            v1.push_back(movements->getMovementIndex(i));
    }
    sort(v1.begin(),v1.end(),[](Movement* m1,Movement* m2){
        return (m1->getToll()->getType() < m2->getToll()->getType());
    });
    for (size_t i = 0; i < v1.size(); i++) {
        cout << i+1 << " - " << v1[i]->getInfo() << endl;
    }
}

void SystemNetwork::showMovementsbyLaneNumber(Toll *toll) {
    vector<Movement*> v1;
    for (size_t i = 0; i < movements->getNumMovements(); i++) {
        if (movements->getMovementIndex(i)->getToll()==toll )
            v1.push_back(movements->getMovementIndex(i));
    }
    sort(v1.begin(),v1.end(),[](Movement* m1,Movement* m2){
        return (m1->getLane()->getLaneNumber() < m2->getLane()->getLaneNumber());
    });
    for (size_t i = 0; i < v1.size(); i++) {
        cout << i+1 << " - " << v1[i]->getInfo() << endl;
    }
}

void SystemNetwork::showMovementsbyLanePrice(Toll *toll) {
    vector<Movement*> v1;
    for (size_t i = 0; i < movements->getNumMovements(); i++) {
        if (movements->getMovementIndex(i)->getToll() == toll && movements->getMovementIndex(i)->getType())
            v1.push_back(movements->getMovementIndex(i));
    }
    sort(v1.begin(),v1.end(),[](Movement* m1,Movement* m2){
        return (m1->getPrice() > m2->getPrice());
    });
    for (size_t i = 0; i < v1.size(); i++) {
        cout << i+1 << " - " << v1[i]->getInfo() << endl;
    }
}

void SystemNetwork::showCarMovementsbyDate() {
    string s_plate;
    int v_class, greenlane;
    bool check = false;
    while (s_plate != "EXIT") {
        cout << "Input the vehicle plate: (if you want to exit without create any vehicle please input EXIT)" << endl;
        getline(cin, s_plate);
        if (s_plate != "EXIT") {
            for (size_t i = 0; i < vehicles->getNumVehicles(); i++) {
                if (vehicles->getVehicleIndex(i)->getPlate() == s_plate) {
                    check = true;
                    break;
                }
            }
            if (!check) {
                //throw exception
                break;
            }
            if (check) break;
        }
    }
    if (s_plate != "EXIT") {
        vector<Movement*> v1;
        for (size_t i = 0; i < movements->getNumMovements(); i++) {
            if (movements->getMovementIndex(i)->getVehicle()->getPlate() == s_plate)
                v1.push_back(movements->getMovementIndex(i));
        }
        sort(v1.begin(),v1.end(),[](Movement* m1,Movement* m2){
            return (*(m1->getDate()) > *(m2->getDate()));
        });
        for (size_t i = 0; i < v1.size(); i++) {
            cout << i+1 << " - " << v1[i]->getInfo() << endl;
        }
    }

}

void SystemNetwork::showCarMovementsbyDistance() {
    string s_plate;
    int v_class, greenlane;
    bool check = false;
    while (s_plate != "EXIT") {
        cout << "Input the vehicle plate: (if you want to exit without create any vehicle please input EXIT)" << endl;
        getline(cin, s_plate);
        if (s_plate != "EXIT") {
            for (size_t i = 0; i < vehicles->getNumVehicles(); i++) {
                if (vehicles->getVehicleIndex(i)->getPlate() == s_plate) {
                    check = true;
                    break;
                }
            }
            if (!check) {
                //throw exception
                break;
            }
            if (check) break;
        }
    }
    if (s_plate != "EXIT") {
        vector<Movement*> v1;
        for (size_t i = 0; i < movements->getNumMovements(); i++) {
            if (movements->getMovementIndex(i)->getType() && movements->getMovementIndex(i)->getVehicle()->getPlate() == s_plate)
                v1.push_back(movements->getMovementIndex(i));
        }
        sort(v1.begin(),v1.end(),[](Movement* m1,Movement* m2){
            return (m1->getDistance() > m2->getDistance());
        });
        for (size_t i = 0; i < v1.size(); i++) {
            cout << i+1 << " - " << v1[i]->getInfo() << endl;
        }
    }

}

void SystemNetwork::showCarMovementsbyPrice() {
    string s_plate;
    int v_class, greenlane;
    bool check = false;
    while (s_plate != "EXIT") {
        cout << "Input the vehicle plate: (if you want to exit without create any vehicle please input EXIT)" << endl;
        getline(cin, s_plate);
        if (s_plate != "EXIT") {
            for (size_t i = 0; i < vehicles->getNumVehicles(); i++) {
                if (vehicles->getVehicleIndex(i)->getPlate() == s_plate) {
                    check = true;
                    break;
                }
            }
            if (!check) {
                //throw exception
                break;
            }
            if (check) break;
        }
    }
    if (s_plate != "EXIT") {
        vector<Movement*> v1;
        for (size_t i = 0; i < movements->getNumMovements(); i++) {
            if (movements->getMovementIndex(i)->getType() && movements->getMovementIndex(i)->getVehicle()->getPlate() == s_plate)
                v1.push_back(movements->getMovementIndex(i));
        }
        sort(v1.begin(),v1.end(),[](Movement* m1,Movement* m2){
            return (m1->getDistance() > m2->getDistance());
        });
        for (size_t i = 0; i < v1.size(); i++) {
            cout << i+1 << " - " << v1[i]->getInfo() << endl;
        }
    }

}

void SystemNetwork::showEmployeeMovements() {
    string s_name;
    bool check = false;
    while (s_name != "EXIT") {
        cout << "Input the employee name: (if you want to exit without create any employee please input EXIT)" << endl;
        getline(cin, s_name);
        if (s_name != "EXIT") {
            for (size_t i = 0; i < employees->getNumEmployees(); i++) {
                if (employees->getEmployeeIndex(i)->getName() == s_name) {
                    check = true;
                    break;
                }
            }
            if (!check) {
                //throw exception
                break;
            }
            if (check) break;
        }
    }
    if (s_name != "EXIT") {
        vector<Movement*> v1;
        for (size_t i = 0; i < movements->getNumMovements(); i++) {
            if (movements->getMovementIndex(i)->getType() && !movements->getMovementIndex(i)->getLane()->getGreenLane()
                                        && movements->getMovementIndex(i)->getLane()->getEmployee()->getName() == s_name)
                v1.push_back(movements->getMovementIndex(i));
        }
        sort(v1.begin(),v1.end(),[](Movement* m1,Movement* m2){
            return (m1->getDate() > m2->getDate());
        });
        for (size_t i = 0; i < v1.size(); i++) {
            cout << i+1 << " - " << v1[i]->getInfo() << endl;
        }
    }
}

void SystemNetwork::carSpentMoreMoney() {
    Vehicle* vehicle;
    float total_price = 0, price;
    //if (vehicles->getNumVehicles() == 0)
        //throw exception
    for (size_t i = 0; i <vehicles->getNumVehicles();i++) {
        price = 0;
        for (size_t j = 0; j < movements->getNumMovements(); j++) {
            if (movements->getMovementIndex(j)->getType() && movements->getMovementIndex(j)->getVehicle()->getPlate() == vehicles->getVehicleIndex(i)->getPlate())
                price += movements->getMovementIndex(j)->getPrice();
        }
        if (price > total_price) {
            total_price = price;
            vehicle = vehicles->getVehicleIndex(i);
        }
    }
    cout << vehicle->getInfo() << " - Total price = " << total_price << endl;
}

void SystemNetwork::BestWorthHighway() {
    Highway* highway;
    float total_price = 0, price;
    //if (highway->getNumHighway() == 0)
    //throw exception
    for (size_t i = 0; i <highways->getNumHighways();i++) {
        price = 0;
        for (size_t j = 0; j < movements->getNumMovements(); j++) {
            if (movements->getMovementIndex(j)->getType() && movements->getMovementIndex(j)->getHighway()->getName() == highways->getHighwayIndex(i)->getName())
                price += movements->getMovementIndex(j)->getPrice();
        }
        if (price > total_price) {
            total_price = price;
            highway = highways->getHighwayIndex(i);
        }
    }
    cout << highway->getInfo() << " - Total price = " << total_price << endl;
}

void SystemNetwork::BestWorthToll() {
    Toll* toll;
    float total_price = 0, price;
    //if (highway->getNumHighway() == 0)
    //throw exception
    for (size_t i = 0; i <highways->getNumHighways();i++) {
        for (size_t k = 0; k < highways->getHighwayIndex(i)->getNumTolls(); k++) {
            price = 0;
            for (size_t j = 0; j < movements->getNumMovements(); j++) {
                if (movements->getMovementIndex(j)->getType() &&
                    movements->getMovementIndex(j)->getToll()->getName() == highways->getHighwayIndex(i)->getTollIndex(k)->getName())
                    price += movements->getMovementIndex(j)->getPrice();
            }
            if (price > total_price) {
                total_price = price;
                toll = highways->getHighwayIndex(i)->getTollIndex(k);
            }
        }
    }
    cout << toll->getInfo() << " - Total price = " << total_price << endl;
}

void SystemNetwork::BestWorthLane() {
    Lane* lane;
    float total_price = 0, price;
    //if (highway->getNumHighway() == 0)
    //throw exception
    for (size_t i = 0; i <highways->getNumHighways();i++) {
        for (size_t k = 0; k < highways->getHighwayIndex(i)->getNumTolls(); k++) {
            for (size_t l = 0; l < highways->getHighwayIndex(i)->getTollIndex(k)->getNumLanes(); l++) {
                price = 0;
                for (size_t j = 0; j < movements->getNumMovements(); j++) {
                    if (movements->getMovementIndex(j)->getType() &&
                        movements->getMovementIndex(j)->getLane() ==
                        highways->getHighwayIndex(i)->getTollIndex(k)->getLane(l))
                        price += movements->getMovementIndex(j)->getPrice();
                }
                if (price > total_price) {
                    total_price = price;
                    lane = highways->getHighwayIndex(i)->getTollIndex(k)->getLane(l);
                }
            }
        }
    }
    cout << lane->getInfo() << " - Total price = " << total_price << endl;
}

void SystemNetwork::HighwayMoreMoves() {
    Highway* highway;
    int total_moves = 0, moves;
    //if (highway->getNumHighway() == 0)
    //throw exception
    for (size_t i = 0; i <highways->getNumHighways();i++) {
        moves = 0;
        for (size_t j = 0; j < movements->getNumMovements(); j++) {
            if (movements->getMovementIndex(j)->getHighway()->getName() == highways->getHighwayIndex(i)->getName())
                moves++;
        }
        if (moves > total_moves) {
            total_moves = moves;
            highway = highways->getHighwayIndex(i);
        }
    }
    cout << highway->getInfo() << " - Total moves = " << total_moves << endl;
}

void SystemNetwork::TollMoreMoves() {
    Toll* toll;
    int total_moves = 0, moves;
    //if (highway->getNumHighway() == 0)
    //throw exception
    for (size_t i = 0; i <highways->getNumHighways();i++) {
        for (size_t k = 0; k < highways->getHighwayIndex(i)->getNumTolls(); k++) {
            moves = 0;
            for (size_t j = 0; j < movements->getNumMovements(); j++) {
                if (movements->getMovementIndex(j)->getToll()->getName() == highways->getHighwayIndex(i)->getTollIndex(k)->getName())
                    moves++;
            }
            if (moves > total_moves) {
                total_moves = moves;
                toll = highways->getHighwayIndex(i)->getTollIndex(k);
            }
        }
    }
    cout << toll->getInfo() << " - Total moves = " << total_moves << endl;
}

void SystemNetwork::LaneMoreMoves() { // tem erros
    Lane* lane;
    int total_moves = 0, moves;
    //if (highway->getNumHighway() == 0)
    //throw exception
    for (size_t i = 0; i <highways->getNumHighways();i++) {
        for (size_t k = 0; k < highways->getHighwayIndex(i)->getNumTolls(); k++) {
            for (size_t l = 0; l < highways->getHighwayIndex(i)->getTollIndex(k)->getNumLanes(); l++) {
                moves = 0;
                for (size_t j = 0; j < movements->getNumMovements(); j++) {
                    if (*movements->getMovementIndex(j)->getLane() ==
                        *highways->getHighwayIndex(i)->getTollIndex(k)->getLane(l))
                        moves += movements->getMovementIndex(j)->getPrice();
                }
                if (moves > total_moves) {
                    total_moves = moves;
                    lane = highways->getHighwayIndex(i)->getTollIndex(k)->getLane(l);
                }
            }
        }
    }
    cout << lane->getInfo() << " - Total moves = " << total_moves << endl;
}