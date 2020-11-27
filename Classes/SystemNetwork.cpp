#include "SystemNetwork.h"

using namespace std;

SystemNetwork::SystemNetwork() {
    highways = new Highways();
    movements = new Movements();
    vehicles = new Vehicles();
    utils = new Utils();
    employees = new Employees();
}



void SystemNetwork::write(){
    vector<Toll *> t;
    vector<Lane *> l;
    ofstream f("../Cache/systemNetworks.txt");

    f<<"HIGHWAYS"<<endl;
    for (int i = 0;i<highways->getNumHighways();i++){
        f<< "Highway nr"<<i+1<<": "<< highways->getHighwayIndex(i)->getInfo() <<endl;
        for(int j=0;j<highways->getHighwayIndex(i)->getNumTolls();j++){
            f<< "  Toll nr"<<j+1<<":"<<endl;
            f<<"  "<<highways->getHighwayIndex(i)->getTollIndex(j)->getInfo()<<endl;
            for(int k=0;k<highways->getHighwayIndex(i)->getTollIndex(j)->getNumLanes();k++){
                f<< "    Lane nr"<<k+1<<":"<<endl;
                f<<"    "<<highways->getHighwayIndex(i)->getTollIndex(j)->getLane(k)->getInfo()<<endl;
            }
        }
    }

    f<<endl<<"VEHICLES"<<endl;
    f<<"Taxes:";
    for (int j=0;j<4;j++){
        f<<" "<<vehicles->getTaxes(j+1);
    }
    f<<endl;
    for(int i=0;i<vehicles->getNumVehicles();i++){
        f<<"Vehicle nr"<<i+1<<": "<<endl;
        f<<vehicles->getVehicleIndex(i)->getInfo()<<endl;
    }

    int n=1;
    f<<endl<<"MOVEMENTS"<<endl;
    for(int i=0;i<movements->getNumMovements();i++){
        if(movements->getMovements()[i]->getType()) {
            f<<"Movement nr"<<n<<": "<<endl;
            f <<movements->getMovementIndex(i)->getInfo()<<endl;
            n++;
        }
    }
    f<<endl<<"EMPLOYEES"<<endl;
    for(int i=0;i<employees->getNumEmployees();i++){
        f<<"Employee nr"<<i+1<<":"<<endl;
        f<<employees->getEmployeeIndex(i)->getInfo()<<endl;
    }

    f.close();
}

void SystemNetwork::read(string file) {
    ifstream f(file);

    int number,code;
    bool tf,movement_type;
    string s,name,geolocal,plate,date1,date2,date;
    float highway_kilometer,tax1,tax2,tax3,tax4;

    Toll* t;
    Lane* l;
    Vehicle* v;
    Date *d;
    Highway* h;
    Employee* e;

    Toll* tt;
    Lane* ll;
    Vehicle* vv;
    Date *dd;
    Highway* hh;

    MovementEntry* me;
    MovementOut* mo;
    float distance;
    float price;

    f>>s;
    if( s=="HIGHWAYS") {
        f>>s;
        while (s == "Highway") {
            f >> s;//discard
            f >> s;
            name = "";
            while (!(s == "-" || s == "Lane" ||s == "Toll" || s == "Highway" || s == "VEHICLE" || s == "MOVEMENTS" || s == "EMPLOYEES")) {
                name += s + " ";
                f >> s;
            }
            name.pop_back();
            h = new Highway(name);
            while (s == "Toll") {//Toll
                f >> s;//discard (nr:)
                f >> s;
                name = "";
                while (!(s == "-" || s == "Lane" ||s == "Toll" || s == "Highway" || s == "VEHICLE" || s == "MOVEMENTS" || s == "EMPLOYEES")) {
                    name += s + " ";
                    f >> s;
                }
                name.pop_back();
                f >> s;
                geolocal = "";
                while (!(s == "-" || s == "Lane" ||s == "Toll" || s == "Highway" || s == "VEHICLE" || s == "MOVEMENTS" || s == "EMPLOYEES")) {
                    geolocal += s + " ";
                    f >> s;
                }
                geolocal.pop_back();
                f >> highway_kilometer;
                f >> s;//discard (-)
                f >> tf;
                if (tf) {
                    t = new TollOut(name, geolocal, highway_kilometer);
                } else {
                    t = new TollEntrance(name, geolocal, highway_kilometer);
                }
                f >> s;
                while (s == "Lane") {//Lane
                    f >> s;//discard (nr:)
                    f >> number;
                    f >> s;//discard (-)
                    f >> tf;
                    f >> s;
                    if (s == "-") {
                        f >> s;
                        name = "";
                        while (s != "-") {
                            name += s + " ";
                            f >> s;
                        }
                        name.pop_back();
                        f >> code;
                        if (code >= employees->getCode()) {
                            code++;
                            employees->setCode(code);
                            code--;
                        }
                        e = new Employee(name, code);
                        employees->addEmployee(e);
                        l = new LaneEmployee(number, tf, e);
                        t->addLane(l);
                        f>>s;
                    } else {
                        l = new Lane(number, tf);
                        t->addLane(l);
                    }

                    if(s=="Lane"){
                        continue;
                    }
                    else {
                        break;
                    }

                }
                h->addToll(t);
                if(s=="Toll"){
                    continue;
                }
                else {
                    break;
                }

            }
            highways->addHighway(h);
            if(s=="Highway"){
                continue;
            }
            else {
                break;
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
            vehicles->addVehicle(plate,number,tf);
            f>>s;
        }
    }


    if(s=="MOVEMENTS"){
        f>>s;
        while(s=="Movement"){
            f>>s; //discard
            f>>date1;
            f>>date2;
            date=date1+" "+date2;
            d = new Date(date);
            f>>s; //discard

            f>>name;
            h = highways->getHighway(name);
            f>>s; //discard

            f>>name;
            t = h->getToll(name);
            f>>s; //discard

            f>>number;
            l= t->getLane(number);
            f>>s; //discard

            f>>name;
            v=vehicles->getVehicle(name);
            f>>s;//discard

            f>>distance;
            f>>s;//discard

            f>>price;
            f>>s;//discard

            f>>date1;
            f>>date2;
            date=date1+" "+date2;
            dd =new Date(date);
            f>>s; //discard

            f>>name;
            hh=highways->getHighway(name);
            f>>s; //discard

            f>>name;
            tt = h->getToll(name);
            f>>s; //discard

            f>>number;
            ll= t->getLane(number);
            f>>s; //discard

            f>>name;
            vv=vehicles->getVehicle(name);
            me = new MovementEntry(vv,hh,tt,ll,dd);
            if(!movements->addMovement(me)){
                cout<<"write movement failed!";
            }
            mo = new MovementOut(v,h,t,l,d,me,price);
            if(!movements->addMovement(mo)){
              cout<<"write movement failed!";
            }
            f>>s;
        }
    }
    if(s=="EMPLOYEES"){
        f>>s;
        while(s=="Employee"){
            bool check = false;
            f>>s;//discard
            name = "";
            while (s != "-") {
                f >> s;
                name += s + " ";
            }
            name = name.substr(0,name.size()-3);
            for(int i=0;i<employees->getNumEmployees();i++){
                if(name==employees->getEmployeeIndex(i)->getName()){
                    f>>s;
                    f>>s;
                    check = true;
                }
            }
            if (check)
                continue;
            f>>code;
            if (code >= employees->getCode()) {
                code++;
                employees->setCode(code);
                code--;
            }
            e=new Employee(name,code);
            employees->addEmployee(e);
            if(f.eof()){
                break;
            }
            f>>s;
        }
    }

    f.close();

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
                try {
                    showEmployeeMovements();
                }
                catch (DontExistAnyMovement &e) {
                    cout << "EXCEPTION: There are no employee movements." << endl;
                }
                catch (EmployeeDoesNotExist &e) {
                    cout << "EXCEPTION: There are no employees with the name " << e.getName() << "." << endl;
                }
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
                try {
                    showMovementsByHighwayName();
                }
                catch (DontExistAnyMovement &e) {
                    cout << "EXCEPTION: There are no movements." << endl;
                }
                break;
            case 2:
                utils->clrScreen();
                try {
                    showMovementsByHighwayPrice();
                }
                catch (DontExistAnyExitMovement &e) {
                    cout << "EXCEPTION: There are no exit movements." << endl;
                }
                break;
            case 3:
                utils->clrScreen();
                try {
                    showMovementsByDate();
                }
                catch (DontExistAnyMovement &e) {
                    cout << "EXCEPTION: There are no exit movements." << endl;
                }
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
                try {
                    showMovementsByTollName(a1);
                }
                catch (DontExistAnyMovement &e) {
                    cout << "EXCEPTION: There are no movements." << endl;
                }
                break;
            case 2:
                utils->clrScreen();
                a1 = chooseHighway();
                if (a1 == nullptr) break;
                try {
                    showMovementsByTollPrice(a1);
                }
                catch (DontExistAnyExitMovement &e) {
                    cout << "EXCEPTION: There are no exit movements." << endl;
                }
                break;
            case 3:
                utils->clrScreen();
                a1 = chooseHighway();
                if (a1 == nullptr) break;
                try {
                    showMovementsByTollType(a1);
                }
                catch (DontExistAnyMovement &e) {
                    cout << "EXCEPTION: There are no movements." << endl;
                }
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
                try {
                    showMovementsbyLaneNumber(t1);
                }
                catch (DontExistAnyMovement &e) {
                    cout << "EXCEPTION: There are no movements." << endl;
                }
                break;
            case 2:
                utils->clrScreen();
                a1 = chooseHighway();
                if (a1 == nullptr) break;
                t1 = chooseToll(a1);
                if (t1 == nullptr) break;
                try {
                    showMovementsbyLanePrice(t1);
                }
                catch (DontExistAnyExitMovement &e) {
                    cout << "EXCEPTION: There are no exit movements." << endl;
                }
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
                try {
                    showCarMovementsbyPrice();
                }
                catch (DontExistAnyExitMovement &e) {
                    cout << "EXCEPTION: There are no exit movements." << endl;
                }
                catch (VehicleDoesNotExist &e) {
                    cout << "EXCEPTION: There are no vehicles with the plate " << e.getPlate() << "." << endl;
                }
                break;
            case 2:
                utils->clrScreen();
                try {
                    showCarMovementsbyDistance();
                }
                catch (DontExistAnyExitMovement &e) {
                    cout << "EXCEPTION: There are no exit movements." << endl;
                }
                catch (VehicleDoesNotExist &e) {
                    cout << "EXCEPTION: There are no vehicles with the plate " << e.getPlate() << "." << endl;
                }
                break;
            case 3:
                utils->clrScreen();
                try {
                    showCarMovementsbyDate();
                }
                catch (DontExistAnyMovement &e) {
                    cout << "EXCEPTION: There are no movements." << endl;
                }
                catch (VehicleDoesNotExist &e) {
                    cout << "EXCEPTION: There are no vehicles with the plate " << e.getPlate() << "." << endl;
                }
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
                try {
                    BestWorthHighway();
                }
                catch (DontExistAnyExitMovement &e) {
                    cout << "EXCEPTION: There are no exit movements." << endl;
                }
                break;
            case 2:
                utils->clrScreen();
                try {
                    BestWorthToll();
                }
                catch (DontExistAnyExitMovement &e) {
                    cout << "EXCEPTION: There are no exit movements." << endl;
                }
                break;
            case 3:
                utils->clrScreen();
                try {
                    BestWorthLane();
                }
                catch (DontExistAnyExitMovement &e) {
                    cout << "EXCEPTION: There are no exit movements." << endl;
                }
                break;
            case 4:
                utils->clrScreen();
                try {
                    carSpentMoreMoney();
                }
                catch (DontExistAnyExitMovement &e) {
                    cout << "EXCEPTION: There are no exit movements." << endl;
                }
                break;
            case 5:
                utils->clrScreen();
                try {
                    HighwayMoreMoves();
                }
                catch (DontExistAnyMovement &e) {
                    cout << "EXCEPTION: There are no movements." << endl;
                }
                break;
            case 6:
                utils->clrScreen();
                try {
                    TollMoreMoves();
                }
                catch (DontExistAnyMovement &e) {
                    cout << "EXCEPTION: There are no movements." << endl;
                }
                break;
            case 7:
                utils->clrScreen();
                try {
                    LaneMoreMoves();
                }
                catch (DontExistAnyMovement &e) {
                    cout << "EXCEPTION: There are no movements." << endl;
                }
                break;
        }
    } while(index);
}



void SystemNetwork::createHighway() {
    string s_name;
    while (s_name != "EXIT") {
        cout << "Input the highway name: (if you want to exit without creating a highway please input EXIT)" << endl;
        getline(cin, s_name);
        if (s_name != "EXIT" && highways->addHighway(s_name)) {
            cout << "Highway created with success!" << endl;
            utils->waitForInput();
            break;
        }
        cout << "ERROR: name of highway already exists." << endl;
    }
}

void SystemNetwork::readHighways() {
    cout << "Highways: " << highways->getNumHighways() << endl;

    for (int i = 0; i < highways->getNumHighways(); i++) {
        cout << i+1 << " - " << highways->getHighwayIndex(i)->showHighway() << endl;
    }
    utils->waitForInput();
}

void SystemNetwork::updateHighway() {
    string s_index, s_name, s_menu;
    int index;
    index = chooseIndexHighway();
    if (index > -1)
        while (s_name != "EXIT") {
            cout << "Input the highway name: (if you want to exit without updating a highway please input EXIT)" << endl;
            getline(cin, s_name);
            if (s_name != "EXIT" && highways->checkHighwayName(s_name)) {
                highways->getHighwayIndex(index)->setName(s_name);
                cout << "Highway updated with success!" << endl;
                utils->waitForInput();
                break;
            }
            cout << "ERROR: name of highway already exists." << endl;
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
    while((index < -1)) {
        cout << "Invalid Number." << endl;
        index = utils->getNumber(highways->getNumHighways()) -1;
    }
    if (index == -1)
        return nullptr;
    return highways->getHighwayIndex(index);
}

int SystemNetwork::chooseIndexHighway() const {
    string s_menu;
    vector<string> v1;
    for (int i = 0; i < highways->getNumHighways(); i++) {
        s_menu = highways->getHighwayIndex(i)->showHighway();
        v1.push_back(s_menu);
    }
    utils->clrScreen();
    return utils->ShowMenu(v1)-1;
}



void SystemNetwork::createToll(Highway *highway) {
    string s_name, s_geolocal;
    float kilometer;
    int i_type;
    bool type;
    while (!(s_name == "EXIT" || kilometer == -1 || i_type == 0)) {
        cout << "Input the toll's name: (if you want to exit without creating a toll please input EXIT)" << endl;
        getline(cin, s_name);
        if (s_name != "EXIT" && highway->checkTollName(s_name)) {
            cout << "Input the toll's geographic location: (if you want to exit without creating a toll please input EXIT)"<< endl;
            getline(cin, s_geolocal);
            if (s_geolocal != "EXIT") {
                cout << "Input the toll's highway kilometer: (if you want to exit without creating a toll please input EXIT)"<< endl;
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
            cout << "ERROR: name of highway already exists." << endl;
    }
}

void SystemNetwork::readTolls(Highway* highway) {
    string s_type;
    cout << "Tolls: " << highway->getNumTolls() << endl;
    for (size_t i = 0; i < highway->getNumTolls(); i++) {
        cout << highway->getTollIndex(i)->showToll() << endl;
    }
    utils->waitForInput();
}

void SystemNetwork::updateToll(Highway *highway) {
    int index=0, toll_index;
    float kilometer;
    string s_name;
    toll_index = chooseIndexToll(highway);
    if (toll_index!= -1) index = utils->ShowMenu({"Toll's name","Toll's geographic location", "Toll's highway kilometer", "Toll's type"});
    switch (index) {
        case 0:
            break;
        case 1:
            while (s_name != "EXIT") {
                cout << "Input the toll name: (if you want to exit without creating a toll please input EXIT)" << endl;
                getline(cin, s_name);
                if (s_name != "EXIT" && highway->checkTollName(s_name)) {
                    highway->getTollIndex(toll_index)->setName(s_name);
                    cout << "Toll's name updated with success!" << endl;
                    utils->waitForInput();
                    break;
                }
                if (s_name!="EXIT")
                    cout << "ERROR: toll's name already exists." << endl;
            }
            break;
        case 2:
            cout << "Input the toll's geographic location: (if you want to exit without updating a toll please input EXIT)" << endl;
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
                cout << "Not possible to update toll's type because the toll doesn't have at least one lane" << endl;
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
        s_menu = highway->getTollIndex(i)->showToll();
        v1.push_back(s_menu);
    }
    utils->clrScreen();
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
        cout << toll->getLane(i)->showLane() << endl;
    }
    utils->waitForInput();
}

void SystemNetwork::updateLane(Toll *toll) {
    int lane_index, index;
    if (toll->getType()) {
        lane_index = chooseIndexLane(toll);
        if (lane_index == -1) {}
        else if (toll->getLane(lane_index)->getGreenLane()) {
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
        utils->waitForInput();
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
        s_menu = toll->getLane(i)->showLane();
        v1.push_back(s_menu);
    }
    utils->clrScreen();
    return utils->ShowMenu(v1)-1;
}



void SystemNetwork::createEmployee() {
    string s_name;
    while (s_name != "EXIT") {
        cout << "Input the employee name: (if you want to exit without creating a employee please input EXIT)" << endl;
        getline(cin, s_name);
        if (s_name != "EXIT" && employees->addEmployee(s_name)) {
            cout << "employee created with success!" << endl;
            utils->waitForInput();
            break;
        }
        if (s_name!= "EXIT")
            cout << "ERROR: name of employee already exists." << endl;
    }
}

void SystemNetwork::updateEmployee() {
    string s_index, s_name;
    int index;
    index = chooseIndexEmployee();
    if (index > -1)
        while (s_name != "EXIT") {
            cout << "Input the employee name: (if you want to exit without updating a employee please input EXIT)" << endl;
            getline(cin, s_name);
            if (s_name != "EXIT" && employees->checkEmployeeName(s_name)) {
                employees->getEmployeeIndex(index)->setName(s_name);
                cout << "Employee updated with success!" << endl;
                utils->waitForInput();
                break;
            }
            if (s_name != "EXIT")
                cout << "ERROR: name of employee already exists." << endl;
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
        cout << i+1 << " - " << employees->getEmployeeIndex(i)->showEmployee() << endl;
    }
    utils->waitForInput();
}

int SystemNetwork::chooseIndexEmployee() const {
    string s_menu;
    vector<string> v1;
    cout << "Employees: " << employees->getNumEmployees() << endl;
    for (int i = 0; i <employees->getNumEmployees(); i++) {
        s_menu = employees->getEmployeeIndex(i)->showEmployee();
        v1.push_back(s_menu);
    }
    utils->clrScreen();
    return utils->ShowMenu(v1)-1;
}



void SystemNetwork::createVehicle() {
    string s_plate;
    int v_class, greenlane;
    while (s_plate != "EXIT") {
        s_plate = utils->getPlate();
        if (s_plate != "EXIT") {
            if (!vehicles->checkPlate(s_plate)) {
                cout << "Choose the vehicle class: " << endl;
                v_class = utils->ShowMenu(
                        {"Classe 1 - Motas", "Classe 2 - Light vehicle (passengers or goods)", "Class 3 - Bus",
                         "Class 4 - Heavy goods vehicle "});
                if (v_class != 0) {
                    cout << "Choose one of the options: " << endl;
                    greenlane = utils->ShowMenu(
                            {"To travel on Green lanes or Normal lanes", "To travel just on Normal lanes"});
                    if (greenlane != 0) {
                        if (greenlane == 1) {
                            vehicles->addVehicle(s_plate, v_class, true);
                        } else {
                            vehicles->addVehicle(s_plate, v_class);
                        }
                        cout << "Vehicle created with success!" << endl;
                        utils->waitForInput();
                        break;
                    } else if (greenlane == 0)
                        s_plate = "EXIT";
                } else if (v_class == 0)
                    s_plate = "EXIT";
            }
        }
        if (!(s_plate == "EXIT" || v_class == 0 || greenlane == 0))
            cout << "ERROR: plate of vehicle already exists." << endl;
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
                    s_plate = utils->getPlate();
                    if (s_plate != "EXIT" && !vehicles->checkPlate(s_plate)) {
                        vehicles->getVehicleIndex(v_index)->setPlate(s_plate);
                        cout << "Vehicle updated with success!" << endl;
                        utils->waitForInput();
                        break;
                    }
                    if (s_plate != "EXIT")
                        cout << "ERROR: plate of vehicle already exists." << endl;
                }
                break;
            case 2:
                cout << "Choose the vehicle class: " << endl;
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
        cout << vehicles->getVehicleIndex(i)->showVehicle() << endl;
    }
    utils->waitForInput();
}

int SystemNetwork::chooseIndexVehicle() const {
    string string1, s_menu;
    vector<string> v1;
    cout << "Vehicles: " << vehicles->getNumVehicles() << endl;
    for (int i = 0; i < vehicles->getNumVehicles(); i++) {
        s_menu = vehicles->getVehicleIndex(i)->showVehicle();
        v1.push_back(s_menu);
    }
    return utils->ShowMenu(v1)-1;
}



void SystemNetwork::getTaxesFromUser() {
    float taxe[4];
    bool exit = false;
    for (int i = 0; i < 4; i++) {
        cout << "Input the tax (euro per kilometer) for the class " << i+1 << ". (if you want to exit without setting a tax please input EXIT) " << endl;
        float a = utils->getFloat();
        if (a == -1) {
            exit = true;
            break;
        }
        else taxe[i] = a;
    }
    if (!exit) {
        vehicles->setTaxes(taxe[0], taxe[1], taxe[2], taxe[3]);
        for (size_t i = 0; i < vehicles->getNumVehicles(); i++) {
            switch (vehicles->getVehicleIndex(i)->getClass()) {
                case 1:
                    vehicles->getVehicleIndex(i)->setTax(taxe[0]);
                    break;
                case 2:
                    vehicles->getVehicleIndex(i)->setTax(taxe[1]);
                    break;
                case 3:
                    vehicles->getVehicleIndex(i)->setTax(taxe[2]);
                    break;
                case 4:
                    vehicles->getVehicleIndex(i)->setTax(taxe[3]);
                    break;
            }
        }
        cout << "Taxes updated with success!" << endl;
        utils->waitForInput();
    }
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
            cout << "ERROR: Before this you have to input the taxes for each vehicle class" << index << endl;
            s_plate = "EXIT";
            utils->waitForInput();
            continue;
        }
        s_plate = utils->getPlate();
        if (s_plate == "EXIT") continue;
        if (!vehicles->checkPlate(s_plate)) {
            cout << "Vehicle is not registed. To advance, input vehicle class" << endl;
            index = utils->ShowMenu({"Classe 1 - Motas", "Classe 2 - Light vehicle (passengers or goods)", "Class 3 - Bus", "Class 4 - Heavy goods vehicle "});
            if (index != 0)
                vehicle = new Vehicle(s_plate,index,vehicles->getTaxes(index));
            else continue;
        }
        else {
            int counter = 0;
            for (int i = 0; i < movements->getNumMovements(); i++){
                if (movements->getMovementIndex(i)->getVehicle()->getPlate() == s_plate) {
                    counter++;
                }
            }
            if (counter % 2 == 1 || counter == 0) {
                vehicle = nullptr;
            }
            else if (counter % 2 == 0 && counter != 0) {
                vehicle = vehicles->getVehicle(s_plate);
            }
        }
        if (vehicle == nullptr) {
            cout << "ERROR: This vehicle is already in a highway." << endl;
            continue;
        }
        do {
            highway = chooseHighway();
            if (highway == nullptr) {
                s_plate = "EXIT";
                utils->waitForInput();
                break;
            }
            if (highway->getNumTolls() == 0) {
                cout << "ERROR: This highway doesn't have a toll." << endl;
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
                cout << "ERROR: This is an exit toll." << endl;
            }
            if (toll->getNumLanes() == 0) {
                cout << "ERROR: This toll doesn't have a lane." << endl;
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
        cout << "Entry movement created with success!" << endl;
        break;
    }
    utils->waitForInput();
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
        s_plate = utils->getPlate();
        if (s_plate == "EXIT") continue;

        int counter = 0;
        for (int i = 0; i < movements->getNumMovements(); i++){
            if (movements->getMovementIndex(i)->getVehicle()->getPlate() == s_plate) {
                counter++;
            }
        }
        if (counter % 2 == 0 || counter == 0) {
            entry = nullptr;
        }
        if (counter % 2 == 1 && counter != 0) {
            for (int i = movements->getNumMovements() - 1; i > -1; i-- ) {
                if (s_plate == movements->getMovementIndex(i)->getVehicle()->getPlate()) {
                    if (!movements->getMovementIndex(i)->getType()) {
                        entry = movements->getMovementIndex(i);
                        break;
                    }
                }
            }
        }
        if (entry == nullptr) {
            cout << "ERROR: Vehicle isn't in a highway." << endl;
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
                cout << "ERROR: This toll doesn't have a lane." << endl;
            }
        } while(toll->getNumLanes() == 0 || !toll->getType());
        if (s_plate == "EXIT") continue;
        time_t timer = time(0);
        tm *now = localtime(&timer);
        string s_date = to_string(now->tm_mday) + "/" + to_string(now->tm_mon) + "/" + to_string(now->tm_year+1900)  + " " + to_string(now->tm_hour) + ":" + to_string(now->tm_min) + ":" + to_string(now->tm_sec);
        //cout << s_date << endl;
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
                cout << "ERROR: This is a green lane and your car only travels on normal lanes." << endl;
            }
        } while(!entry->getVehicle()->getGreenLaneBool() && lane->getGreenLane());

        if (lane == nullptr) {
            s_plate = "EXIT";
            utils->waitForInput();
            continue;
        }

        auto *m1 = new MovementOut(entry->getVehicle(),highway,toll,lane,date,entry);
        movements->addMovement(m1);
        cout << "Exit movement created with success!" << endl;
        break;
    }
    utils->waitForInput();
}

void SystemNetwork::showMovementsByHighwayName() {
    if (movements->getNumMovements() == 0) throw DontExistAnyMovement();
    if (movements->getNumMovements() == 0)
        cout << "There are no Highways" << endl;
    if (movements->getNumMovements() == 0)
        cout << "There are no movements" << endl;
    vector<Movement*> v1 = movements->getMovements();
    sort(v1.begin(),v1.end(),[](Movement* m1,Movement* m2){
        return (m1->getHighway()->getInfo() < m2->getHighway()->getInfo());
    });
    for (size_t i = 0; i < v1.size(); i++) {
        cout << i+1 << " - " << v1[i]->showMovement() << endl;
    }
    utils->waitForInput();
}

void SystemNetwork::showMovementsByHighwayPrice() {
    if (highways->getNumHighways() <= 1) throw DontExistAnyExitMovement();
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
        cout << i+1 << " - " << v1[i]->showMovement() << endl;
    }
    utils->waitForInput();
}

void SystemNetwork::showMovementsByDate() {
    if (movements->getNumMovements() == 0) throw DontExistAnyMovement();
    vector<Movement*> v1 = movements->getMovements();
    sort(v1.begin(),v1.end(),[](Movement* m1,Movement* m2){
        return (*(m1->getDate()) > *(m2->getDate()));
    });
    for (size_t i = 0; i < v1.size(); i++) {
        cout << i+1 << " - " << v1[i]->showMovement() << endl;
    }
    utils->waitForInput();
}

void SystemNetwork::showMovementsByTollName(Highway* highway) {
    if (movements->getNumMovements() == 0) throw DontExistAnyMovement();
    vector<Movement*> v1;
    for (size_t i = 0; i < movements->getNumMovements(); i++) {
        if (movements->getMovementIndex(i)->getHighway()==highway)
            v1.push_back(movements->getMovementIndex(i));
    }
    sort(v1.begin(),v1.end(),[](Movement* m1,Movement* m2){
        return (m1->getToll()->getName() < m2->getToll()->getName());
    });
    for (size_t i = 0; i < v1.size(); i++) {
        cout << i+1 << " - " << v1[i]->showMovement() << endl;
    }
    utils->waitForInput();
}

void SystemNetwork::showMovementsByTollPrice(Highway *highway) {
    if (movements->getNumMovements() <= 1) throw DontExistAnyExitMovement();
    vector<Movement*> v1;
    for (size_t i = 0; i < movements->getNumMovements(); i++) {
        if (movements->getMovementIndex(i)->getHighway()==highway && movements->getMovementIndex(i)->getType())
            v1.push_back(movements->getMovementIndex(i));
    }
    sort(v1.begin(),v1.end(),[](Movement* m1, Movement* m2){
        return (m1->getPrice() > m2->getPrice());
    });
    for (size_t i = 0; i < v1.size(); i++) {
        cout << i+1 << " - " << v1[i]->showMovement() << endl;
    }
    utils->waitForInput();
}

void SystemNetwork::showMovementsByTollType(Highway *highway) {
    if (movements->getNumMovements() == 0) throw DontExistAnyMovement();
    vector<Movement*> v1;
    for (size_t i = 0; i < movements->getNumMovements(); i++) {
        if (movements->getMovementIndex(i)->getHighway()==highway)
            v1.push_back(movements->getMovementIndex(i));
    }
    sort(v1.begin(),v1.end(),[](Movement* m1,Movement* m2){
        return (m1->getToll()->getType() < m2->getToll()->getType());
    });
    for (size_t i = 0; i < v1.size(); i++) {
        cout << i+1 << " - " << v1[i]->showMovement() << endl;
    }
    utils->waitForInput();
}

void SystemNetwork::showMovementsbyLaneNumber(Toll *toll) {
    if (movements->getNumMovements() == 0) throw DontExistAnyMovement();
    vector<Movement*> v1;
    for (size_t i = 0; i < movements->getNumMovements(); i++) {
        if (movements->getMovementIndex(i)->getToll()==toll )
            v1.push_back(movements->getMovementIndex(i));
    }
    sort(v1.begin(),v1.end(),[](Movement* m1,Movement* m2){
        return (m1->getLane()->getLaneNumber() < m2->getLane()->getLaneNumber());
    });
    for (size_t i = 0; i < v1.size(); i++) {
        cout << i+1 << " - " << v1[i]->showMovement() << endl;
    }
    utils->waitForInput();
}

void SystemNetwork::showMovementsbyLanePrice(Toll *toll) {
    if (movements->getNumMovements() <= 1) throw DontExistAnyExitMovement();
    vector<Movement*> v1;
    for (size_t i = 0; i < movements->getNumMovements(); i++) {
        if (movements->getMovementIndex(i)->getToll() == toll && movements->getMovementIndex(i)->getType())
            v1.push_back(movements->getMovementIndex(i));
    }
    sort(v1.begin(),v1.end(),[](Movement* m1,Movement* m2){
        return (m1->getPrice() > m2->getPrice());
    });
    for (size_t i = 0; i < v1.size(); i++) {
        cout << i+1 << " - " << v1[i]->showMovement() << endl;
    }
    utils->waitForInput();
}

void SystemNetwork::showCarMovementsbyDate() {
    if (movements->getNumMovements() == 0) throw DontExistAnyMovement();
    string s_plate;
    int v_class, greenlane;
    bool check = false;
    while (s_plate != "EXIT") {
        s_plate = utils->getPlate();
        if (s_plate != "EXIT") {
            for (size_t i = 0; i < vehicles->getNumVehicles(); i++) {
                if (vehicles->getVehicleIndex(i)->getPlate() == s_plate) {
                    check = true;
                    break;
                }
            }
            if (!check) {
                throw VehicleDoesNotExist(s_plate);
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
            cout << i+1 << " - " << v1[i]->showMovement() << endl;
        }
    }
    utils->waitForInput();
}

void SystemNetwork::showCarMovementsbyDistance() {
    if (movements->getNumMovements() <= 1) throw DontExistAnyExitMovement();
    string s_plate;
    int v_class, greenlane;
    bool check = false;
    while (s_plate != "EXIT") {
        s_plate = utils->getPlate();
        if (s_plate != "EXIT") {
            for (size_t i = 0; i < vehicles->getNumVehicles(); i++) {
                if (vehicles->getVehicleIndex(i)->getPlate() == s_plate) {
                    check = true;
                    break;
                }
            }
            if (!check) {
                throw VehicleDoesNotExist(s_plate);
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
            cout << i+1 << " - " << v1[i]->showMovement() << endl;
        }
    }
    utils->waitForInput();
}

void SystemNetwork::showCarMovementsbyPrice() {
    if (movements->getNumMovements() <= 1) throw DontExistAnyExitMovement();
    string s_plate;
    int v_class, greenlane;
    bool check = false;
    while (s_plate != "EXIT") {
        s_plate = utils->getPlate();
        if (s_plate != "EXIT") {
            for (size_t i = 0; i < vehicles->getNumVehicles(); i++) {
                if (vehicles->getVehicleIndex(i)->getPlate() == s_plate) {
                    check = true;
                    break;
                }
            }
            if (!check) {
                throw VehicleDoesNotExist(s_plate);
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
            cout << i+1 << " - " << v1[i]->showMovement() << endl;
        }
    }
    utils->waitForInput();
}

void SystemNetwork::showEmployeeMovements() {
    if (movements->getNumMovements() == 0) throw DontExistAnyMovement();
    string s_name;
    bool check = false;
    while (s_name != "EXIT") {
        cout << "Input the employee name: (if you want to exit without creating an employee please input EXIT)" << endl;
        getline(cin, s_name);
        if (s_name != "EXIT") {
            for (size_t i = 0; i < employees->getNumEmployees(); i++) {
                if (employees->getEmployeeIndex(i)->getName() == s_name) {
                    check = true;
                    break;
                }
            }
            if (!check) {
                throw EmployeeDoesNotExist(s_name);
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
            cout << i+1 << " - " << v1[i]->showMovement() << endl;
        }
    }
    utils->waitForInput();
}

void SystemNetwork::carSpentMoreMoney() {
    if (movements->getNumMovements() <= 1) throw DontExistAnyExitMovement();
    Vehicle* vehicle;
    float total_price = 0, price;
    for (size_t i = 0; i <vehicles->getNumVehicles();i++) {
        price = 0;
        for (size_t j = 0; j < movements->getNumMovements(); j++) {
            if (movements->getMovementIndex(j)->getType() &&
                movements->getMovementIndex(j)->getVehicle()->getPlate() == vehicles->getVehicleIndex(i)->getPlate()) {
                cout << movements->getMovementIndex(j)->getType() << " "<<movements->getMovementIndex(j)->getPrice() << "------" <<endl;
                price += movements->getMovementIndex(j)->getPrice();
                cout << price << endl;
            }
        }
        if (price > total_price) {
            total_price = price;
            vehicle = vehicles->getVehicleIndex(i);
        }
    }
    cout << vehicle->showVehicle() << " - Total price = " << total_price << endl;
    utils->waitForInput();
}

void SystemNetwork::BestWorthHighway() {
    if (movements->getNumMovements() <= 1) throw DontExistAnyExitMovement();
    Highway* highway;
    float total_price = 0, price;
    for (size_t i = 0; i <highways->getNumHighways();i++) {
        price = 0;
        for (size_t j = 0; j < movements->getNumMovements(); j++) {
            if (movements->getMovementIndex(j)->getType() && movements->getMovementIndex(j)->getHighway()->getInfo() == highways->getHighwayIndex(i)->getInfo())
                price += movements->getMovementIndex(j)->getPrice();
        }
        if (price > total_price) {
            total_price = price;
            highway = highways->getHighwayIndex(i);
        }
    }
    cout << highway->showHighway() << " - Total price = " << total_price << endl;
    utils->waitForInput();
}

void SystemNetwork::BestWorthToll() {
    if (movements->getNumMovements() <= 1) throw DontExistAnyExitMovement();
    Toll* toll;
    float total_price = 0, price;
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
    cout << toll->showToll() << " - Total price = " << total_price << endl;
    utils->waitForInput();
}

void SystemNetwork::BestWorthLane() {
    if (movements->getNumMovements() <= 1) throw DontExistAnyExitMovement();
    Lane* lane;
    float total_price = 0, price;
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
    cout << lane->showLane() << " - Total price = " << total_price << endl;
    utils->waitForInput();
}

void SystemNetwork::HighwayMoreMoves() {
    if (movements->getNumMovements() == 0) throw DontExistAnyMovement();
    Highway* highway;
    int total_moves = 0, moves;
    for (size_t i = 0; i <highways->getNumHighways();i++) {
        moves = 0;
        for (size_t j = 0; j < movements->getNumMovements(); j++) {
            if (movements->getMovementIndex(j)->getHighway()->getInfo() == highways->getHighwayIndex(i)->getInfo())
                moves++;
        }
        if (moves > total_moves) {
            total_moves = moves;
            highway = highways->getHighwayIndex(i);
        }
    }
    cout << highway->showHighway() << " - Total moves = " << total_moves << endl;
    utils->waitForInput();
}

void SystemNetwork::TollMoreMoves() {
    if (movements->getNumMovements() == 0) throw DontExistAnyMovement();
    Toll* toll;
    Highway* highway;
    int total_moves = 0, moves;
    for (size_t i = 0; i <highways->getNumHighways();i++) {
        for (size_t k = 0; k < highways->getHighwayIndex(i)->getNumTolls(); k++) {
            moves = 0;
            for (size_t j = 0; j < movements->getNumMovements(); j++) {
                if (movements->getMovementIndex(j)->getToll() == highways->getHighwayIndex(i)->getTollIndex(k))
                    moves++;
            }
            if (moves > total_moves) {
                total_moves = moves;
                toll = highways->getHighwayIndex(i)->getTollIndex(k);
                highway = highways->getHighwayIndex(i);
            }
        }
    }
    cout << highway->showHighway() << " - " << toll->showToll() << " - Total moves = " << total_moves << endl;
    utils->waitForInput();
}

void SystemNetwork::LaneMoreMoves() {
    if (movements->getNumMovements() == 0) throw DontExistAnyMovement();
    Lane* lane;
    Highway* highway;
    Toll* toll;
    int total_moves = 0, moves;
    for (size_t i = 0; i <highways->getNumHighways();i++) {
        for (size_t k = 0; k < highways->getHighwayIndex(i)->getNumTolls(); k++) {
            for (size_t l = 0; l < highways->getHighwayIndex(i)->getTollIndex(k)->getNumLanes(); l++) {
                moves = 0;
                for (size_t j = 0; j < movements->getNumMovements(); j++) {
                    if (movements->getMovementIndex(j)->getLane() ==
                        highways->getHighwayIndex(i)->getTollIndex(k)->getLane(l))
                        moves++;
                }
                if (moves > total_moves) {
                    total_moves = moves;
                    lane = highways->getHighwayIndex(i)->getTollIndex(k)->getLane(l);
                    toll = highways->getHighwayIndex(i)->getTollIndex(k);
                    highway = highways->getHighwayIndex(i);
                }
            }
        }
    }
    cout << highway->showHighway() << " - Toll name: "<< toll->getName() << " - " << lane->showLane() << " - Total moves = " << total_moves << endl;
    utils->waitForInput();
}
