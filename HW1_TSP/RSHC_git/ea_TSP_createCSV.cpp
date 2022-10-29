#include<windows.h>
#include<stdio.h>
#include<string.h>
#include<cstdlib>
#include<time.h>
#include<stdlib.h>
#include<string>
#include<vector>
#include<iostream>
#include<fstream>
#include<istream>
#include<algorithm>
#include <numeric>
#include<format>
#include <typeinfo>
#include <iomanip>
#include <sstream>
#include <math.h>
#include <random>
using namespace std;




string oneline;


mt19937 g;
int ea_iter = 50000;

int num_city = 0;
vector<vector<float>> city_xy;
vector<float> xy;


string EA_method;       //
vector<int> gene;       // individual
vector<int> best_gene;  //
double fitness;         // length of path
double best_fit;        //
vector<double> lc;      // learning curve
int pop_size = 100;      //population size
double pc = 0.9, pm = 0.6;
// probability of crossover and mutation

//mutation
int idx1, idx2;
vector<int> mut;

//crossover
int por1, por2;
vector<int>p1;
vector<int>p2;

//arg
vector<double> fit;

//ea
vector<vector<int>> population;
vector<vector<int>> good_pop;
float pr = 0.5;

vector<vector<int>> his_best_gene;




//calculate two cities distance
double Cal_dis(int idx1, int idx2) {
    float d_x = city_xy[idx1][0] - city_xy[idx2][0];
    float d_y = city_xy[idx1][1] - city_xy[idx2][1];
    double dis = pow(d_x, 2) + pow(d_y, 2);
    return pow(dis, 0.5);
}
double Total_dis(vector<int> p) {
    double t = 0;
    for (int i = 1; i < num_city; i++) {
        t += Cal_dis(p[i - 1], p[i]);
    }
    t += Cal_dis(p[num_city - 1], p[0]);
    return t;
}


//mutation

void Mutate(int num_city) {

    idx1 = rand() % num_city;
    idx2 = rand() % num_city;
    while (idx2 == idx1)
    {
        idx2 = rand() % num_city;    
    }
    int temp = mut[idx1];
    mut[idx1] = mut[idx2];
    mut[idx2] = temp;

}



//crossover

void crossover(int num_city) {
    int temp1, temp2, id1, id2;

    por1 = rand() % (num_city - 1);

    por2 = rand() % (num_city - por1 - 1) + por1 + 1;
    //find the crossover portion

    for (int i = por1; i <= por2; i++) {
        temp1 = p1[i];
        temp2 = p2[i];
        id1 = find(p1.begin(), p1.end(), temp2) - p1.begin();
        id2 = find(p2.begin(), p2.end(), temp1) - p2.begin();
        p1[i] = temp2;
        p2[i] = temp1;
        p1[id1] = temp1;
        p2[id2] = temp2;

    }
}



//argsort_population

vector<int> argsort(vector<double>& fit) {

    vector<int> o_fit(fit.size());
    iota(o_fit.begin(), o_fit.end(), 0);

    stable_sort(o_fit.begin(), o_fit.end(),
        [&fit](int i1, int i2) {return fit[i1] < fit[i2]; });

    return o_fit;
}




vector<int> pro_ran(int num_city) {
    gene.clear();
    for (int i = 0; i < num_city; i++)
    {
        gene.push_back(i);
    }

    return gene;
}



void ea(int ea_iter) {
    his_best_gene.clear();
    // form initial parents
    population.clear();
    gene = pro_ran(num_city);
    for (int i = 0; i < pop_size; i++) {
        shuffle(gene.begin(), gene.end(), g);
        population.push_back(gene);
    }

    //iterate for const times
    for (int i = 0; i < ea_iter; i++) {
        cout << i << " ";
        //crossover
        for (int i = 0; i < pop_size; i += 2) {
            p1.clear();
            p2.clear();

            p1 = population[i];
            p2 = population[i + 1];
            if (rand() % 100 < 100 * pc) {
                crossover(num_city);
                population.push_back(p1);
                population.push_back(p2);
            }
            else {
                shuffle(gene.begin(), gene.end(), g);
                population.push_back(gene);
                shuffle(gene.begin(), gene.end(), g);
                population.push_back(gene);
            }
        }

        //mutation
        for (int i = 0; i < pop_size; i++) {
            mut.clear();
            mut = population[i];
            if (rand() % 100 < 100 * pm) {
                Mutate(num_city);
                if (Total_dis(mut) > Total_dis(population[i])) {
                    population[i] = mut;
                }
            }
        }

        // get all the fitness
        fit.clear();
        for (int i = 0; i < pop_size; i++) {
            double total_length;
            total_length = Total_dis(population[i]);
            fit.push_back(total_length);
        }

        // argsort according to fitness
        vector<int> o_fit;
        o_fit = argsort(fit);
        reverse(o_fit.begin(), o_fit.end());
        double best;
        best = fit[0];
        cout << best << endl;
        lc.push_back(best);

        good_pop.clear();
        vector<int> good;
        for (int i = 0; i < pop_size * pr; i++) {
            good = population[o_fit[i]];
            good_pop.push_back(good);
        }
        his_best_gene.push_back(good_pop[0]);
        
        //trans good gene to population
        population.clear();
        for (int i = 0; i < pop_size * pr; i++) {
            population.push_back(good_pop[i]);
        }

        //fillin
        for (int i = 0; i < (pop_size - pop_size * pr); i++) {
            shuffle(gene.begin(), gene.end(), g);
            population.push_back(gene);
        }
    }
}





//main
int main() {

    srand(int(time(NULL)));
    ifstream fin;
    fin.open("tsp.txt");
    while (!fin.eof())
    {
        getline(fin, oneline);
        //cout << str << endl;
        if (oneline == "")
        {
            break;
        }
        num_city += 1;
        xy.push_back(stof(oneline.substr(0, 5)));
        xy.push_back(stof(oneline.substr(8, 13)));
        //std::cout << xy[0]<< " " << xy[1] << endl;
        city_xy.push_back(xy);
        xy.clear();
    }
    fin.close();

    std::cout << "city number:" << num_city << endl;


    //    RS(rs_iter);    // random search
    //    HC(hc_iter);    // hill climber
    ea(ea_iter);    //genetic Algorithm



    string filename = "lc_" + EA_method + "1" + ".csv";
    ofstream outFile;
    outFile.open(filename, ios::out);

    for (auto iter : lc)
    {
        outFile << iter << endl;
    }
    cout << "LC File saved!" << endl;


    string filename1 = "his_" + EA_method + "1" + ".csv";
    ofstream outFile1;
    outFile1.open(filename1, ios::out);

    for (auto iter : his_best_gene)
    {
        for (int i = 0; i < num_city; i++)
        {
            outFile1 << iter[i]<<",";
        }
        outFile1 << endl;
    }
    cout << "History File saved!" << endl;



    return 0;
}

