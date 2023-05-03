#include <iostream>
#include "BigInt.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <queue>
#include <cstdlib>
#include <time.h>

using namespace std;

short patientID = 1;
ofstream out("protokolas.txt");

struct Patient
{
    short ID;
    bool isIndividual; // is patient in group therapy
    bool firstTime = true;
    bool willHealFaster;            // will patient heal faster
    unsigned int timeInTherapy = 0; // how long patient is in therapy
    unsigned int neededTherapyTime; // how long patient needs to be in therapy
    unsigned int whenToReturn = 0;  // when patient will return
};

struct GroupT
{                               // group therapy
    bool isForFirstTime = true; // is group therapy for first time patients
    bool isFull;                // is group therapy full
    bool hasStarted = false;    // has group therapy started
    unsigned int startTime;     // when group therapy started
    unsigned int endTime;       // when group therapy ends
    vector<Patient> patients;   // patients in group therapy
};

struct IndividualT
{                            // individual therapy
    bool isFull;             // is individual therapy full
    bool hasStarted = false; // has individual therapy started
    unsigned int startTime;  // when individual therapy started
    unsigned int endTime;    // when individual therapy ends
    Patient patient;         // patient in individual therapy
};

struct State
{
    unsigned int n;         // specialists who form therapy groups out of k people
    unsigned int k;         // number of people in a group
    unsigned int nK;        // number of patients in a group (not first time)
    unsigned int m;         // specialists who work individually with patients
    unsigned int d;         // number of time it takes for a patient to come back
    unsigned int InT;       // InT - Individual therapy time in hours
    unsigned int GrT;       // GrT - Group therapy time in hours
    unsigned short p1;      // probability of a new patient arriving
    unsigned short p2;      // probability of a patient recovering faster
    unsigned short h;       // how much faster patients recover in percentage
    unsigned short p3;      // probability of a patient coming to individual therapy
    unsigned int GrN;       // GrN - number of group therapies required
    unsigned int InN;       // InN - number of individual therapies required
    BigInt InK;             // InK - price of individual therapy
    BigInt GrK;             // GrK - price of group therapy
    unsigned int sim_hours; // how many hours to simulate
};

struct Clinic
{
    vector<GroupT> groupTherapies;           // group therapies
    vector<IndividualT> individualTherapies; // individual therapies
    queue<Patient> patients;                 // patients in queue
    queue<Patient> indivWaiting;             // patients waiting for individual therapy
    queue<Patient> groupWaiting;             // patients waiting for group therapy
    queue<Patient> returningP;               // patients who will return
    int isWell = 0;                          // number of patients who are well
};

struct Revenue
{
    BigInt InR; // InR - revenue from individual therapy
    BigInt GrR; // GrR - revenue from group therapy
};

bool rand_prob(short prob)
{
    return ((rand() % 100) < prob) ? true : false;
}

void getClinicData(Clinic clinic)
{
    int size = clinic.patients.size();
    for (int i = 0; i < size; i++)
    {
        out << "Patient " << i + 1 << " isIndividual: " << clinic.patients.front().isIndividual << " firstTime: " << clinic.patients.front().firstTime << endl;
        clinic.patients.pop();
    }
    size = clinic.individualTherapies.size();
    for (int i = 0; i < size; i++)
    {
        out << "Individual therapy " << i + 1 << " isFull: " << clinic.individualTherapies[i].isFull << endl;
    }
    size = clinic.groupTherapies.size();
    for (int i = 0; i < size; i++)
    {
        out << "Group therapy " << i + 1 << " isFull: " << clinic.groupTherapies[i].isFull << endl;
    }
}

int maxGroup(State state, Clinic clinic, bool isForNew) // returns group number with the biggest number of patients
{
    int max = 0;
    int maxGroup = 0;
    for (int i = 0; i < state.n; i++)
    {
        if (clinic.groupTherapies[i].isForFirstTime == isForNew && clinic.groupTherapies[i].patients.size() > max && !clinic.groupTherapies[i].isFull)
        {
            max = clinic.groupTherapies[i].patients.size();
            maxGroup = i;
        }
    }
    return maxGroup;
}

void writeStateData(State state)
{
    out << "Olen Rackauskas. 3 grupe, 2 pogrupis." << endl;
    out << "ADS 3 uzduotis. 10 variantas: Apsilankymas pas psichologa." << endl
         << endl;
    out << "DALIS 1. Ivesti rodikliai." << endl;
    // out << "Pradiniai duomenys: " << endl;
    out << "Proceso trukme:\t\t\t\t\t\t\t" << state.sim_hours << " val." << endl;
    out << "Privaciu psichologu skaicius:\t\t\t\t\t" << state.m << '.' << endl;
    out << "Grupes psichologu skaicius:\t\t\t\t\t" << state.n << '.' << endl;
    out << "Privacios terapijos trukme:\t\t\t\t\t" << state.InT << " val." << endl;
    out << "Grupines terapijos trukme:\t\t\t\t\t" << state.GrT << " val." << endl;
    out << "Zmoniu skaicius grupeje (nauji pacientai):\t\t\t" << state.k << '.' << endl;
    out << "Zmoniu skaicius grupeje (pakartotinai ateinantys pacientai):\t" << state.nK << '.' << endl;
    out << "Po kiek laiko grizta pacientai:\t\t\t\t\t" << state.d << " val." << endl;
    out << "Sansas, kad ateis naujas pacientas:\t\t\t\t" << state.p1 << "%." << endl;
    out << "Sansas, kad pacientas pasveiks greiciau:\t\t\t" << state.p2 << "%." << endl;
    out << "Procentas, ant kiek greiciau pasveiks pacientas:\t\t" << state.h << "%." << endl;
    out << "Sansas, kad pacientas pasirinks privacia terapija:\t\t" << state.p3 << "%." << endl;
    out << "Kiek kartu reikia nueiti i grupines terapijas:\t\t\t" << state.GrN << '.' << endl;
    out << "Kiek kartu reikia nueiti i individualias terapijas:\t\t" << state.InN << '.' << endl;
    out << "Privaciu terapiju kaina:\t\t\t\t\t" << state.InK << " eur." << endl;
    out << "Grupiniu terapiju kaina:\t\t\t\t\t" << state.GrK << " eur." << endl
         << endl;

    out << "DALIS 2. Protokolas." << endl;
}

void getWaitAndReturnList(State state, Clinic clinic)
{
    int gr = 0, in = 0;
    queue<Patient> temp(clinic.returningP);
    int size = temp.size();

    out << "DALIS 3. Rezultatai." << endl;
    out << "Atejusiu pacientu skaicius \t\t\t\t= " << patientID << '.' << endl;
    out << "Pasveikusiu pacientu skaicius \t\t\t\t= " << clinic.isWell << '.' << endl;
    out << "Laukianciu pacientu skaicius (grupines terapijos) \t= " << clinic.groupWaiting.size() << '.' << endl;
    out << "Laukianciu pacientu skaicius (privacios terapijos) \t= " << clinic.indivWaiting.size() << '.' << endl;

    if (size > 0)
        for (int i = 0; i < size; i++)
        {
            if (temp.front().isIndividual)
                in++;
            else
                gr++;
            temp.pop();
        }

    out << "Sugriztanciu pacientu skaicius (grupines terapijos) \t= " << gr << '.' << endl;
    // out << "Sugriztanciu pacientu skaicius (privacios terapijos): " << in << '.' << endl;

    // out << endl << "Pasveiko pacientu: " << clinic.isWell << endl;
}

void printNeededSpecialists(State state, Clinic clinic)
{
    int in = 0, gr = 0;
    queue<Patient> temp(clinic.returningP);
    int size = temp.size();

    if (size > 0)
        for (int i = 0; i < size; i++)
        {
            if (temp.front().isIndividual)
                in++;
            else
                gr++;
            temp.pop();
        }

    if (clinic.indivWaiting.size() + in > clinic.groupWaiting.size() + gr)
        out << "Labiau apsimoka eiti i grupines terapijas." << endl;
    else
        out << "Labiau apsimoka eiti i privacias terapijas." << endl;
    out << endl;

    int num_additional_specialists = ceil((clinic.indivWaiting.size() + in) * state.InN / (0.186 * state.sim_hours));
    int num_additional_g_specialists = ceil((clinic.groupWaiting.size() + gr) * state.GrN * state.nK / (0.4 * state.sim_hours));

    if (num_additional_specialists > 0)
        out << "Reikia dar " << num_additional_specialists << " privaciu specialistu." << endl;
    if (num_additional_g_specialists > 0)
        out << "Reikia dar " << num_additional_g_specialists << " grupiniu specialistu." << endl;
}

void printReturning(Clinic clinic)
{
    int size = clinic.returningP.size();
    queue<Patient> temp(clinic.returningP);
    for (int i = 0; i < size; i++)
    {
        out << "P" << temp.front().ID << " sugrizta i klinika.  |  ";
        temp.pop();
    }
}

Clinic checkIndividualPatients(State state, Clinic clinic, int time)
{
    bool wasAdded = false;
    int size;
    size = clinic.indivWaiting.size(); // firstly checking the waiting list and adding patients through it
    if (size > 0)
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < state.m; j++)
            {
                if (!clinic.individualTherapies[j].isFull)
                {
                    // out << "P" << clinic.indivWaiting.front().ID << " pridetas prie privacios terapijos. Specialisto ID: " << i + 1 << endl;
                    clinic.individualTherapies[j].patient = clinic.indivWaiting.front();
                    clinic.individualTherapies[j].isFull = true;
                    // out << "Popping ID from waiting: " << clinic.indivWaiting.front().ID << endl;
                    clinic.indivWaiting.pop();
                    wasAdded = true;
                    break;
                }
            }
            if (!wasAdded)
            {
                // out << "Pacientai is laukimo eiles nebegali buti priimti." << endl;
                break;
            }
        }
    }
    wasAdded = false;
    size = clinic.patients.size(); // checking the main queue and adding patients through it
    if (size > 0)
    {
        // individual therapy
        if (clinic.patients.front().isIndividual == 1)
        {
            out << "\tAtvyko P" << clinic.patients.front().ID << ". Pacientas nori privacios terapijos." << endl;
            // bool wasAdded = false;
            for (int i = 0; i < state.m; i++)
            {
                // out << "Is full: " << clinic.individualTherapies[i].isFull << endl;
                if (!clinic.individualTherapies[i].isFull)
                {
                    clinic.individualTherapies[i].patient = clinic.patients.front();
                    clinic.individualTherapies[i].isFull = true;
                    // out << "Popping ID: " << clinic.patients.front().ID << endl;
                    clinic.patients.pop();
                    wasAdded = true;
                    // out << "Patient added" << endl;
                    break;
                }
            }
            if (!wasAdded)
            {
                out << "\tP" << clinic.patients.front().ID << " turi laukti eileje." << endl;
                clinic.indivWaiting.push(clinic.patients.front());
                // out << "Popping ID into Waiting: " << clinic.patients.front().ID << endl;
                clinic.patients.pop();
            }
        }
    }

    wasAdded = false;
    size = clinic.returningP.size(); // checking the returning queue and adding patients through it
    if (size > 0)
    {
        // printReturning(clinic);
        if (clinic.returningP.front().isIndividual)
        {
            // out << "When to return? " << clinic.returningP.front().whenToReturn << " time: " << time << endl;
            if (clinic.returningP.front().whenToReturn == time)
                out << "\tAtvyko P" << clinic.returningP.front().ID << "(pakartotinai). Pacientas nori privacios terapijos." << endl;
            for (int i = 0; i < state.m; i++)
            {
                // out << clinic.returningP.front().ID << " vazineja for'e" << endl;
                if (!clinic.individualTherapies[i].isFull && clinic.returningP.front().whenToReturn == time)
                {
                    // out << "Pakartotinai added?" << endl;
                    clinic.individualTherapies[i].patient = clinic.returningP.front();
                    clinic.individualTherapies[i].isFull = true;
                    // out << "Popping return ID: " << clinic.returningP.front().ID << endl;
                    clinic.returningP.pop();
                    wasAdded = true;
                    // out << "Adding returning indiv patient to " << i + 1 << "th therapist" << endl;
                    break;
                }
            }
            // out << "Was added? " << wasAdded << endl;
            if (!wasAdded && clinic.returningP.front().whenToReturn == time)
            {
                out << "\tP" << clinic.returningP.front().ID << " turi laukti eileje." << endl;
                clinic.indivWaiting.push(clinic.returningP.front());
                // out << "Popping return ID into waiting: " << clinic.returningP.front().ID << endl;
                clinic.returningP.pop();
            }
        }
    }

    return clinic;
}

void printIndividualQueue(Clinic clinic)
{
    queue<Patient> temp(clinic.indivWaiting);
    int size = temp.size();
    if (size > 0)
    {
        out << "\tPrivacios terapijos eile: ";
        for (int i = 0; i < size; i++)
        {
            if (i != size - 1)
                out << "P" << temp.front().ID << ", ";
            else
                out << "P" << temp.front().ID << ".";
            temp.pop();
        }
        out << endl;
    }
    // out << endl;
}

void printGroupQueue(Clinic clinic)
{
    queue<Patient> temp(clinic.groupWaiting);
    int size = temp.size();
    if (size > 0)
    {
        out << "\tGrupines terapijos eile: ";
        for (int i = 0; i < size; i++)
        {
            if (i != size - 1)
                out << "P" << temp.front().ID << ", ";
            else
                out << "P" << temp.front().ID << ".";
            temp.pop();
        }
        out << endl;
    }
    // out << endl;
}

void printCurrentGroup(State state, Clinic clinic)
{
    for (int i = 0; i < state.n; i++)
    {
        if (clinic.groupTherapies[i].patients.size() > 0)
        {
            out << "\tGrupes " << i + 1 << " pacientai: ";
            for (int j = 0; j < clinic.groupTherapies[i].patients.size(); j++)
            {
                // out << "Size: " << clinic.groupTherapies[i].patients.size()-1 << " j: " << j << endl;
                if (j != clinic.groupTherapies[i].patients.size() - 1)
                    out << "P" << clinic.groupTherapies[i].patients[j].ID << ", ";
                else
                    out << "P" << clinic.groupTherapies[i].patients[j].ID << ".";
            }
            out << endl;
        }
    }
    // out << endl;
}

Clinic checkGroupPatients(State state, Clinic clinic, int time)
{
    int size;
    bool wasAdded = false;
    size = clinic.groupWaiting.size(); // checking the waiting queue and adding patients through it
    if (size > 0)
    {
        for (int i = 0; i < size; i++)
        {
            for (int i = maxGroup(state, clinic, true); i < state.n; i++)
            {
                if (clinic.groupTherapies[i].patients.size() < state.k && clinic.groupWaiting.front().firstTime == true && clinic.groupTherapies[i].isForFirstTime)
                {
                    // out << "Adding from waiting list" << endl;
                    clinic.groupTherapies[i].patients.push_back(clinic.groupWaiting.front());
                    clinic.groupTherapies[i].isForFirstTime = true;
                    out << "\tP" << clinic.groupWaiting.front().ID << " yra priskirtas prie grupes " << i + 1 << ". Jos dydis = " << clinic.groupTherapies[i].patients.size() << " pacientai(-as)." << endl;
                    clinic.groupWaiting.pop();
                    wasAdded = true;
                    // out << "Grupes " << i + 1 << " dydis = " << clinic.groupTherapies[i].patients.size() << '.' << endl;
                    if (clinic.groupTherapies[i].patients.size() == state.k)
                    {
                        clinic.groupTherapies[i].isFull = true;
                    }
                    break;
                }
                else if ((clinic.groupTherapies[i].patients.size() == 0 && clinic.groupWaiting.front().firstTime == false) || (clinic.groupTherapies[i].patients.size() > 0 && clinic.groupTherapies[i].isForFirstTime == false && clinic.groupWaiting.front().firstTime == false))
                {
                    // out << "Adding from waiting list" << endl;
                    clinic.groupTherapies[i].patients.push_back(clinic.groupWaiting.front());
                    clinic.groupTherapies[i].isForFirstTime = false;
                    out << "\tP" << clinic.groupWaiting.front().ID << " yra priskirtas prie grupes " << i + 1 << ". Jos dydis = " << clinic.groupTherapies[i].patients.size() << " pacientai(-as)." << endl;
                    clinic.groupWaiting.pop();
                    wasAdded = true;
                    // out << "Assigned to group therapy " << i + 1 << endl;
                    // out << "\tGroup size: " << clinic.groupTherapies[i].patients.size() << endl;
                    if (clinic.groupTherapies[i].patients.size() == state.k)
                    {
                        clinic.groupTherapies[i].isFull = true;
                    }
                    break;
                }
            }
        }
    }
    // out << "test" << endl;
    wasAdded = false;
    size = clinic.patients.size(); // checking the patients queue and adding patients through it
    if (size > 0)
    {
        if (!clinic.patients.front().isIndividual)
        {
            out << "\tAtvyko P" << clinic.patients.front().ID << ". Pacientas nori grupines terapijos." << endl;
            int id;
            for (int i = maxGroup(state, clinic, true); i < state.n; i++)
            {
                if (clinic.groupTherapies[i].patients.size() < state.k && clinic.patients.front().firstTime == true && clinic.groupTherapies[i].isForFirstTime)
                {
                    // out << "Adding from patients list" << endl;
                    clinic.groupTherapies[i].patients.push_back(clinic.patients.front());
                    clinic.groupTherapies[i].isForFirstTime = true;
                    out << "\tP" << clinic.patients.front().ID << " yra priskirtas prie grupes " << i + 1 << ". Jos dydis = " << clinic.groupTherapies[i].patients.size() << " pacientai(-as)." << endl;
                    clinic.patients.pop();
                    wasAdded = true;
                    // out << "Assigned to group therapy " << i + 1 << endl;
                    // out << "Grupes " << i+1 << " dydis = " << clinic.groupTherapies[i].patients.size() << endl;
                    if (clinic.groupTherapies[i].patients.size() == state.k)
                    {
                        clinic.groupTherapies[i].isFull = true;
                    }
                    break;
                }
                if (!wasAdded)
                    id = clinic.patients.front().ID;
                // out << "\tP" << clinic.patients.front().ID << " turi laukt eileje." << endl;

                // TODO In group therapy add a check for when patients are returning (make return only groups)
            }
            if (!wasAdded)
            {
                out << "\tP" << clinic.patients.front().ID << " turi laukt eileje." << endl;
                // out << "\tPatient was not added" << endl;
                clinic.groupWaiting.push(clinic.patients.front());
                clinic.patients.pop();
            }
        }
    }
    return clinic;
}

Revenue simulate(State state)
{
    writeStateData(state);
    srand(time(NULL));
    Revenue revenue = {
        .InR = BigInt("0"),
        .GrR = BigInt("0"),
    };
    Clinic clinic = {
        .groupTherapies = vector<GroupT>(state.n),
        .individualTherapies = vector<IndividualT>(state.m),
        .patients = queue<Patient>(),
        .indivWaiting = queue<Patient>(),
        .groupWaiting = queue<Patient>(),
        .returningP = queue<Patient>(),
    };
    int time = 0;
    int patients = 0;
    while (time <= state.sim_hours)
    {
        out << "T= " << time << '.' << endl;
        // new patient
        if (rand_prob(state.p1))
        {
            // out << "New patient arrived" << endl;
            patients++;
            Patient patient = {
                .ID = patientID,
                .isIndividual = rand_prob(state.p3),
                .firstTime = true,
                .willHealFaster = rand_prob(state.p2),
                .timeInTherapy = 0,
            };
            patientID++;
            // out << "Is indiv: " << endl;

            if (patient.isIndividual)
                patient.neededTherapyTime = state.InN;
            else
                patient.neededTherapyTime = state.GrN;

            if (patient.willHealFaster && !patient.isIndividual && state.GrN > 1)
            {
                patient.neededTherapyTime = state.GrN * (state.h / 100.0);
                // out << "NEEDED THERAPY TIME (group): " << patient.neededTherapyTime << endl;
            }
            clinic.patients.push(patient);
            // out << "Patient pushed." << endl;
        }

        // deal with individual therapy patients
        clinic = checkIndividualPatients(state, clinic, time);
        for (int i = 0; i < state.m; i++) // individual therapies
        {
            // out << "ID: " << i << " | has started? " << clinic.individualTherapies[i].hasStarted << " | is full? " << clinic.individualTherapies[i].isFull << endl;
            if (clinic.individualTherapies[i].isFull && !clinic.individualTherapies[i].hasStarted)
            {
                out << "\tPrivati terapija su P" << clinic.individualTherapies[i].patient.ID << " prasidejo. Privataus specialisto ID: " << i + 1 << '.' << endl;
                clinic.individualTherapies[i].hasStarted = true;
                clinic.individualTherapies[i].startTime = time;
                clinic.individualTherapies[i].endTime = time + state.InT;
            }
            else if (clinic.individualTherapies[i].hasStarted && time == clinic.individualTherapies[i].endTime)
            {
                out << "\tPrivati terapija su P" << clinic.individualTherapies[i].patient.ID << " pasibaige. Privataus specialisto ID: " << i + 1 << '.' << endl;
                clinic.individualTherapies[i].hasStarted = false;
                clinic.individualTherapies[i].isFull = false;
                clinic.individualTherapies[i].patient.timeInTherapy++;
                if (clinic.individualTherapies[i].patient.timeInTherapy < state.InN)
                {
                    clinic.individualTherapies[i].patient.firstTime = false;
                    clinic.individualTherapies[i].patient.whenToReturn = time + state.d;
                    clinic.returningP.push(clinic.individualTherapies[i].patient);
                }
                else if (clinic.individualTherapies[i].patient.timeInTherapy == state.InN)
                {
                    out << "\tP" << clinic.individualTherapies[i].patient.ID << " pasveiko." << endl;
                    clinic.isWell++;
                    revenue.InR += state.InK;
                }
            }
            // out << "ID: " << i << " | has started? " << clinic.individualTherapies[i].hasStarted << " | is full? " << clinic.individualTherapies[i].isFull << endl;
        }

        clinic = checkGroupPatients(state, clinic, time);
        printCurrentGroup(state, clinic);
        printGroupQueue(clinic);
        printIndividualQueue(clinic);

        int size = clinic.patients.size();
        // Adds patients to their respective therapies
        size = clinic.returningP.size();
        bool wasAdded = false;

        if (size > 0)
        {
            if (clinic.returningP.size() > 0)
            {
                if (!clinic.returningP.front().isIndividual)
                {
                    wasAdded = false;
                    for (int j = maxGroup(state, clinic, false); j < state.n; j++)
                    {
                        if (clinic.returningP.size() == 0)
                            break;
                        for (int i = 0; i < size; i++)
                        {
                            // if (clinic.returningP.front().whenToReturn == time)
                            //     out << "Patient (ID: " << clinic.returningP.front().ID << ") returning to group therapy" << endl;
                            if ((clinic.groupTherapies[j].patients.size() == 0 && clinic.returningP.front().whenToReturn == time) || (clinic.groupTherapies[j].patients.size() < state.nK && clinic.groupTherapies[j].isForFirstTime == false && clinic.returningP.front().whenToReturn == time))
                            {
                                clinic.groupTherapies[j].patients.push_back(clinic.returningP.front());
                                clinic.groupTherapies[j].isForFirstTime = false;
                                out << "\tP" << clinic.returningP.front().ID << " priskiriamas prie grupes " << j + 1 << ". Jos dydis: " << clinic.groupTherapies[j].patients.size() << " pacientai(-as)." << endl;
                                clinic.returningP.pop();
                                wasAdded = true;
                                // out << "\tGroup size: " << clinic.groupTherapies[j].patients.size() << endl;
                                if (clinic.groupTherapies[j].patients.size() == state.nK)
                                {
                                    clinic.groupTherapies[j].isFull = true;
                                }
                            }
                        }
                    }
                    // if (!wasAdded && clinic.returningP.front().whenToReturn == time)
                    // out << "\tReturning patient not added" << endl;
                }
            }
        }

        // TODO add returning patients to group therapies
        // TODO check if patients heal faster in group therapy
        // therapy simulations

        for (int i = 0; i < state.n; i++) // group therapies
        {
            if (clinic.groupTherapies[i].isFull && !clinic.groupTherapies[i].hasStarted)
            {
                out << "\tGrupine terapija prasidejo. Grupes nr: " << i + 1 << '.' << endl;
                clinic.groupTherapies[i].hasStarted = true;
                clinic.groupTherapies[i].startTime = time;
                clinic.groupTherapies[i].endTime = time + state.GrT;
            }
            else if (clinic.groupTherapies[i].hasStarted && time == clinic.groupTherapies[i].endTime)
            {
                out << "\tGrupine terapija pasibaige. Grupes nr: " << i + 1 << '.' << endl;
                clinic.groupTherapies[i].hasStarted = false;
                clinic.groupTherapies[i].isFull = false;
                clinic.groupTherapies[i].isForFirstTime = true;
                size = clinic.groupTherapies[i].patients.size();
                for (int j = 0; j < size; j++)
                {
                    int fastHeal = state.GrN * (state.h / 100.0);
                    clinic.groupTherapies[i].patients[j].timeInTherapy++;
                    if (clinic.groupTherapies[i].patients[j].neededTherapyTime == state.GrN)
                    {
                        if (clinic.groupTherapies[i].patients[j].timeInTherapy < state.GrN)
                        {
                            clinic.groupTherapies[i].patients[j].firstTime = false;
                            clinic.groupTherapies[i].patients[j].whenToReturn = time + state.d;
                            clinic.returningP.push(clinic.groupTherapies[i].patients[j]);
                        }
                        else if (clinic.groupTherapies[i].patients[j].timeInTherapy == state.GrN)
                        {
                            out << "\tP" << clinic.groupTherapies[i].patients[j].ID << " pasveiko." << endl;
                            clinic.isWell++;
                            revenue.GrR += state.GrK;
                        }
                    }
                    else
                    {
                        if (clinic.groupTherapies[i].patients[j].timeInTherapy < fastHeal)
                        {
                            clinic.groupTherapies[i].patients[j].firstTime = false;
                            clinic.groupTherapies[i].patients[j].whenToReturn = time + state.d;
                            clinic.returningP.push(clinic.groupTherapies[i].patients[j]);
                        }
                        else if (clinic.groupTherapies[i].patients[j].timeInTherapy == fastHeal)
                        {
                            out << "\tP" << clinic.groupTherapies[i].patients[j].ID << " pasveiko." << endl;
                            clinic.isWell++;
                            revenue.GrR += state.GrK;
                        }
                    }
                }

                clinic.groupTherapies[i].patients.clear();
            }
        }

        ++time;
        out << endl;
    }
    getWaitAndReturnList(state, clinic);
    out << "Grupiniu terapiju pelnas \t\t\t\t= " << revenue.GrR << "eur." << endl;
    out << "Privaciu terapiju pelnas \t\t\t\t= " << revenue.InR << "eur." << endl << endl;
    printNeededSpecialists(state,clinic);
    // getClinicData(clinic);
    return revenue;
}

int main()
{
    State s = {
        .n = 2,
        .k = 5,
        .nK = 3,
        .m = 1,
        .d = 2,
        .InT = 2,
        .GrT = 5,
        .p1 = 70,
        .p2 = 50,
        .h = 80,
        .p3 = 50,
        .GrN = 2,
        .InN = 2,
        .InK = BigInt("20"),
        .GrK = BigInt("10"),
        .sim_hours = 100,
    };
    Revenue r = simulate(s);
    // out << "Grupiniu terapiju pelnas =\t\t\t\t" << r.GrR << "eur." << endl;
    // out << "Privaciu terapiju pelnas =\t\t\t\t" << r.InR << "eur." << endl;
    return 0;
}