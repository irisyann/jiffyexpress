#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>
#include <fstream>
#include <chrono>

static double haversine(double lat1, double lon1, double lat2, double lon2);
void getCoordinates(char type, std::string name, double& lat, double& lon);
char identifyUser();
void sender_makeOrder();
void calculateDistance(char senderLocation, char receiverState, char& receiverLocation, float& distance_senderToBusTerminal, float& distance_busTerminalToReceiver, float& distance_terminalToTerminal);
float calculateRiderCost(float distance);
float calculateBusCost(char receiverLocation);
float calculateTotalCost(float cost_senderToBusTerminal, float cost_busTerminalToReceiver, float cost_terminalToTerminal, char itemProperty, char insuranceOption, float& parcelCost);

int main()
{
    std::cout << "Welcome to JIFFY Express!\n";
    char userType = identifyUser();

    if (userType == '1') 
    {
        sender_makeOrder();
    } 

}

static double haversine(double lat1, double lon1, double lat2, double lon2)
{
    double PI = 4.0 * atan(1.0);

    //main code inside the class
    double dlat1 = lat1 * (PI / 180);

    double dlong1 = lon1 * (PI / 180);
    double dlat2 = lat2 * (PI / 180);
    double dlong2 = lon2 * (PI / 180);

    double dLong = dlong1 - dlong2;
    double dLat = dlat1 - dlat2;

    double aHarv = pow(sin(dLat / 2.0), 2.0) + cos(dlat1) * cos(dlat2) * pow(sin(dLong / 2), 2);
    double cHarv = 2 * atan2(sqrt(aHarv), sqrt(1.0 - aHarv));

    // The IUGG value for the equatorial radius of the Earth is 6378.137 km 
    const double earth = 6378.137;
    double distance = earth * cHarv;
    return distance;
}

void getCoordinates(char type, std::string locationCode, double& lat, double& lon)
{
    std::fstream file;
    file.open("coordinates.csv", std::ios::in);
    std::string line;
    std::string str_type(1, type);

    while (std::getline(file, line, ','))
    {
        if (line == str_type) // if type matches (receiver/terminal/sender)
        {
            std::getline(file, line, ',');
            if (line == locationCode) // if location name matches
            {
                std::getline(file, line, ',');
                lat = stod(line); // get latitude
                std::getline(file, line);
                lon = stod(line); // get longitude
                break;
            }
            else std::getline(file, line); // skip to next line if no match
        } 
        else std::getline(file, line); // skip to next line if no match
    }
}

char identifyUser() {
    char user = 0;

    std::cout << "\n >> Enter your identity:"
        << "\n[1] Sender (I want to deliver a parcel)"
        << "\n[2] Rider"
        << "\n[3] Bus driver\n";
    std::cin >> user;

    while (user != '1' && user != '2' && user != '3')
    {
        std::cout << "\n[X] Invalid input! Please  try again.";
        std::cout << "\n >> Enter your identity:"
            << "\n[1] Sender (I want to deliver a parcel)"
            << "\n[2] Rider"
            << "\n[3] Bus driver\n";
        std::cin >> user;
    }
    return user;
}

void sender_makeOrder()
{
    std::string senderName, senderContact, senderEmail;
    std::string receiverName, receiverContact, receiverEmail;
    char senderLocation, receiverState, receiverLocation;
    char itemSize = 'S'; // default is S (smallest size)

    float distance_senderToBusTerminal = 0;
    float distance_busTerminalToReceiver = 0;
    float distance_terminalToTerminal = 0;
    float parcelCost = 0;
    float totalDistance = 0;
    std::string itemType;
    char itemProperty;
    char insuranceOption;

    std::fstream newOrder;
    // generate file name with unique identification number
    const auto p1 = std::chrono::system_clock::now();
    std::string randomNum = std::to_string(std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count());
    std::string filename = "order_";
    filename.append(randomNum);
    filename.append(".txt");
    newOrder.open(filename, std::ios::out);

    std::cout 
        << "\n===================================="
        << "\nPlease input sender details below."
        << "\n===================================="
        << "\n>> Sender name: ";
    std::cin.ignore();
    std::getline(std::cin, senderName);

    std::cout << "\n>> Sender contact number: ";
    std::cin >> senderContact;

    std::cout << "\n>> Sender e-mail: ";
    std::cin >> senderEmail;

    std::cout << "\n>> Sender location: "
        << "\n[A] Ampang"
        << "\n[K] Kepong\n";
    std::cin >> senderLocation;
    senderLocation = toupper(senderLocation);
    while (senderLocation != 'A' && senderLocation != 'K')
    {
        std::cout << "\n[X] Invalid input! Please  try again.";
        std::cout << "\n>> Sender location: "
            << "\n[A] Ampang"
            << "\n[K] Kepong\n";
        std::cin >> senderLocation;
        senderLocation = toupper(senderLocation);
    }
    std::string senderLocationFull;
    if (senderLocation == 'A') senderLocationFull = "Ampang";
    else if (senderLocation == 'K') senderLocationFull = "Kepong";

    std::cout
        << "\n===================================="
        << "\nPlease input receiver details below."
        << "\n===================================="
        << "\n>> Receiver name: ";
    std::cin.ignore();
    std::getline(std::cin, receiverName);

    std::cout << "\n>> Receiver contact number: ";
    std::cin >> receiverContact;

    std::cout << "\n>> Receiver e-mail: ";
    std::cin >> receiverEmail;

    std::cout << "\n>> Receiver state: "
        << "\n[P] Perak"
        << "\n[J] Johor\n";
    std::cin >> receiverState;
    receiverState = toupper(receiverState);
    while (receiverState != 'P' && receiverState != 'J')
    {
        std::cout << "\n[X] Invalid input! Please  try again.";
        std::cout << "\n>> Receiver state: "
            << "\n[P] Perak"
            << "\n[J] Johor\n";
        std::cin >> receiverState;
        receiverState = toupper(receiverState);
    }
    std::string receiverStateFull;
    if (receiverState == 'P') receiverStateFull = "Perak";
    else if (receiverState == 'J') receiverStateFull = "Johor";

    calculateDistance(senderLocation, receiverState, receiverLocation, distance_senderToBusTerminal, distance_busTerminalToReceiver, distance_terminalToTerminal);

    std::string receiverLocationFull;
    if (receiverLocation == 'T') receiverLocationFull = "Taiping";
    else if (receiverLocation == 'I') receiverLocationFull = "Ipoh";
    else if (receiverLocation == 'J') receiverLocationFull = "Johor Bahru";
    else if (receiverLocation == 'B') receiverLocationFull = "Batu Pahat";

    float cost_senderToBusTerminal = calculateRiderCost(distance_senderToBusTerminal);
    float cost_busTerminalToReceiver = calculateRiderCost(distance_busTerminalToReceiver);
    float cost_terminalToTerminal = calculateBusCost(receiverLocation);

    std::cout << "\n\n>> Item type (food, document, gift etc): ";
    std::cin.ignore();
    std::getline(std::cin, itemType);

    std::cout
        << "\n\n>> Size & weight of item: "
        << "\n[S] 34cm x 25cm x 8cm - min 2kg (RM5.00)"
        << "\n[M] 34cm x 25cm x 15cm - min 5kg (+ RM5.00)"
        << "\n[L] 38cm x 32cm x 20cm - min 10kg (+ RM10.00)\n";
    std::cin >> itemProperty;
    itemProperty = toupper(itemProperty);
    while (itemProperty != 'S' && itemProperty != 'M' && itemProperty != 'L')
    {
        std::cout << "\n[X] Invalid input! Please  try again.";
        std::cout
            << "\n\n>> Size & weight of item: "
            << "\n[S] 34cm x 25cm x 8cm - min 2kg (RM10.00)"
            << "\n[M] 34cm x 25cm x 15cm - min 5kg (+ RM5.00)"
            << "\n[L] 38cm x 32cm x 20cm - min 10kg (+ RM10.00)\n";
        std::cin >> itemProperty;
        itemProperty = toupper(itemProperty);
    }

    std::cout << "\n\n>> Would you like to purchase JIFFY Protection for only RM3?"
        << "\nIn any case of broken or missing item, JIFFY Protection will compensate up to RM500."
        << "\n[Y] Yes"
        << "\n[N] No\n";
    std::cin >> insuranceOption;
    insuranceOption = toupper(insuranceOption);
    while (insuranceOption != 'Y' && insuranceOption != 'N')
    {
        std::cout << "\n[X] Invalid input! Please  try again.";
        std::cout << "\n\nWould you like to purchase JIFFY Protection for only RM3?"
            << "\nIn any case of broken or missing item, JIFFY Protection will compensate up to RM500."
            << "\n[Y] Yes"
            << "\n[N] No\n";
        std::cin >> insuranceOption;
        insuranceOption = toupper(insuranceOption);
    }

    float totalCost = calculateTotalCost(cost_senderToBusTerminal, cost_busTerminalToReceiver, cost_terminalToTerminal, itemProperty, insuranceOption, parcelCost);

    newOrder
        << "==================\n"
        << "ORDER DETAILS\n"
        << "==================\n"
        << "\nSENDER NAME: " << senderName
        << "\nSENDER CONTACT: " << senderContact
        << "\nSENDER EMAIL: " << senderEmail
        << "\nSENDER LOCATION: " << senderLocationFull
        << "\n\nRECEIVER NAME: " << receiverName
        << "\nRECEIVER CONTACT: " << receiverContact
        << "\nRECEIVER EMAIL: " << receiverEmail
        << "\nRECEIVER STATE: " << receiverStateFull
        << "\nRECEIVER LOCATION: " << receiverLocationFull
        << "\n\nITEM TYPE: " << itemType
        << "\nJIFFY PROTECTION: " << insuranceOption
        << "\n\nCOST FROM [" << senderLocationFull << "] TO [TERMINAL BERSEPADU SELATAN]: RM" << std::fixed << std::setprecision(2) << cost_senderToBusTerminal
        << "\nCOST OF BUS TRAVEL: RM" << std::fixed << std::setprecision(2) << cost_terminalToTerminal
        << "\nCOST FROM [TAIPING TERMINAL] TO [" << receiverLocationFull << "]: RM" << std::fixed << std::setprecision(2) << cost_busTerminalToReceiver
        << "\nCOST OF PARCEL PACKAGE: RM" << std::fixed << std::setprecision(2) << parcelCost;
    
    if (insuranceOption == 'Y') newOrder << "\nJIFFY PROTECTION COST: RM3.00";

    newOrder << "\n\nTOTAL COST: RM" << std::fixed << std::setprecision(2) << totalCost;

    std::cout << "\nYour order has been made successfully!"
        << "\nA rider has been assigned to collect your package. Thank you for your patience.";
        
}

float calculateTotalCost(float cost_senderToBusTerminal, float cost_busTerminalToReceiver, float cost_terminalToTerminal, char itemProperty, char insuranceOption, float& parcelCost)
{
    float totalCost = 0;
    
    if (itemProperty == 'S') parcelCost = 10.0;
    else if (itemProperty == 'M') parcelCost = 15.0;
    else if (itemProperty == 'L') parcelCost = 20.0;
    
    if (insuranceOption == 'Y') totalCost += 3.0;

    totalCost += parcelCost + cost_senderToBusTerminal + cost_busTerminalToReceiver + cost_terminalToTerminal;

    return totalCost;
}

float calculateBusCost(char receiverLocation)
{
    // take half of bus passenger ticket price
    float cost = 0;
    if (receiverLocation == 'T') // Taiping
        cost = 27.0 / 2.0;
    else if (receiverLocation == 'I') // Ipoh
        cost = 22.0 / 2.0;
    else if (receiverLocation == 'J') // Johor
        cost = 30.0 / 2.0;
    else if (receiverLocation == 'B') // Batu Pahat
        cost = 24.0 / 2.0;
    
    return cost;
}

float calculateRiderCost(float distance)
{
    const float BASE_FARE = 5.0;
    float cost = 0;
    cost += BASE_FARE;
    /*
    * rate: RM1.50 per km up to 30km
    *       RM1.00 per km for distances >30km
    */
    if (distance <= 30)
        cost += distance * 1.5;
    else cost += distance * 1.0;

    return cost;
}

void calculateDistance(char senderLocation, char receiverState, char& receiverLocation, float& distance_senderToBusTerminal, float& distance_busTerminalToReceiver, float& distance_terminalToTerminal)
{
    float totalDistance = 0;
    std::string locationName;
    double r_lat = 0, r_lon = 0;
    double s_lat = 0, s_lon = 0;
    double t1_lat = 0, t1_lon = 0;
    double t2_lat = 0, t2_lon = 0;

    std::string r_terminal = "TBS";
    std::string s_terminal = "Taiping Terminal";

    std::string str_senderLocation(1, senderLocation); // convert senderLocation code to string from char
    getCoordinates('S', str_senderLocation, s_lat, s_lon); // coords of sender location (Ampang)
    getCoordinates('T', r_terminal, t1_lat, t1_lon); // coords of TBS
    
    distance_senderToBusTerminal = haversine(s_lat, s_lon, t1_lat, t1_lon);

    if (receiverState == 'P') // Perak
    {
        std::cout << "\n>> Receiver location: "
            << "\n[T] Taiping"
            << "\n[I] Ipoh\n";
        std::cin >> receiverLocation;
        receiverLocation = toupper(receiverLocation);
        while (receiverLocation != 'T' && receiverLocation != 'I')
        {
            std::cout << "\n[X] Invalid input! Please  try again.";
            std::cout << "\n>> Receiver location: "
                << "\n[T] Taiping"
                << "\n[I] Ipoh\n";
            std::cin >> receiverLocation;
            receiverLocation = toupper(receiverLocation);
        }
    }
    else if (receiverState == 'J') // Johor
    {
        std::cout << "\n>> Receiver location: "
            << "\n[J] Johor Bahru"
            << "\n[B] Batu Pahat\n";
        std::cin >> receiverLocation;
        receiverLocation = toupper(receiverLocation);
        while (receiverLocation != 'J' && receiverLocation != 'B')
        {
            std::cout << "\n[X] Invalid input! Please  try again.";
            std::cout << "\n>> Receiver location: "
                << "\n[J] Johor Bahru"
                << "\n[B] Batu Pahat\n";
            std::cin >> receiverLocation;
            receiverLocation = toupper(receiverLocation);
        }
    }

    getCoordinates('T', s_terminal, t2_lat, t2_lon); // coords of Taiping terminal
    distance_terminalToTerminal = haversine(t1_lat, t1_lon, t2_lat, t2_lon);

    std::string str_receiverLocation(1, receiverLocation);  // convert receiverLocation to string from char
    getCoordinates('R', str_receiverLocation, r_lat, r_lon); // coords of receiver location (Taiping)

    distance_busTerminalToReceiver = haversine(r_lat, r_lon, t2_lat, t2_lon);



    /* base fare: rm2
 * rm1 per km <30km
 * rm1.5 per km >30km
 * ampang: 3.1491° N, 101.7625° E
 * TBS: 3.0781° N, 101.7111° E
 * ampang to TBS: 12.5km - 2+12.5= RM14.5////////
 *
 */

}

