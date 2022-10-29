
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
#include<format>
#include <typeinfo>
#include <iomanip>
#include <sstream>
#include <math.h>
#include <random>
#include <sys/time.h>

using namespace std;


string oneline;

int rs_iter = 10000;
int hc_iter = 10000;


int num_city = 0;
vector<vector<float>> city_xy;
vector<float> xy;


//EAÀ„∑®œ‡πÿ±‰¡ø
string EA_method;       //
vector<int> gene;       //
vector<int> best_gene;  //
double fitness;         //
double best_fit;        //
vector<double> lc;      //
vector<int> evo_gene;           //
vector<vector<int>> evo_his;    //

//
double Cal_dis(int idx1, int idx2) {
    float d_x = city_xy[idx1][0] - city_xy[idx2][0];
    float d_y = city_xy[idx1][1] - city_xy[idx2][1];
    double dis = pow(d_x, 2) + pow(d_y, 2);
    return pow(dis, 0.5);
}



// Random search
void RS(int iter_num) {
    EA_method = "RS";      //Random Search
    lc.clear(); //
    gene.clear(); //
    for (int i = 0; i < num_city; i++)
    {
        gene.push_back(i);
    }

    //
    for (int i = 0; i < iter_num; i++)
    {
        //
        mt19937 g;
        shuffle(gene.begin(), gene.end(),g);
        //
        fitness = 0;
        for (int gene_idx = 0; gene_idx < num_city - 1; gene_idx++)
        {
            fitness += Cal_dis(gene[gene_idx], gene[gene_idx + 1]);
        }
        

        
        if (i == 0)   //
        {
            best_fit = fitness;
            best_gene.assign(gene.begin(), gene.end());
            evo_his.push_back(best_gene);   //
        }
        else          //
        {
            if (fitness < best_fit)
            {
                best_fit = fitness;     //
                best_gene.assign(gene.begin(), gene.end()); //
                //
                evo_gene.clear();
                evo_gene.assign(best_gene.begin(), best_gene.end()); //Ω´¥À ±µƒ◊Óº—ª˘“Ú∏≥÷µ
                evo_gene.insert(evo_gene.begin(), i + 1);            //
                //
                evo_his.push_back(evo_gene);
            }
        }
        lc.push_back(best_fit);                   //
        cout << i+1 << " " << best_fit << endl;   //
    }
}


// Hill Climer
//
int idx1, idx2;
void HC(int iter_num) {
    EA_method = "HC";      //Hill Climbing
    lc.clear(); // «Âø’learning curve
    gene.clear(); // …˙≥…1-city”––Ú ˝◊È
    for (int i = 0; i < num_city; i++)
    {
        gene.push_back(i);
    }
    //
    mt19937 g;
    shuffle(gene.begin(), gene.end(),g);

    //
    for (int i = 0; i < iter_num; i++)
    {
        //
        idx1 = rand() % num_city;
        idx2 = rand() % num_city;
        while (idx2 == idx1)
        {
            idx2 = rand() % num_city;    //
        }
        
        //
        int temp = gene[idx1];
        gene[idx1] = gene[idx2];
        gene[idx2] = temp;
        //
        fitness = 0;
        for (int gene_idx = 0; gene_idx < num_city - 1; gene_idx++)
        {
            fitness += Cal_dis(gene[gene_idx], gene[gene_idx + 1]);
        }
        
        //
        if (i == 0)   //
        {
            best_fit = fitness;
            best_gene.assign(gene.begin(), gene.end());
            evo_his.push_back(best_gene);   //
        }
        else          //
        {
            if (fitness < best_fit)
            {
                best_fit = fitness;     //
                best_gene.assign(gene.begin(), gene.end()); //
                //find better way
                evo_gene.clear();
                evo_gene.assign(best_gene.begin(), best_gene.end()); //
                
                evo_gene.insert(evo_gene.begin(), i + 1);
                
                evo_his.push_back(evo_gene);
            }
        }
        lc.push_back(best_fit);         //
        cout << i+1 << " " << best_fit << endl;
    }
}




int main() {
    

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
    HC(hc_iter);    // hill climber


    //œÚcsv–¥»Î ˝æ›
    string filename = "lc_" + EA_method + "1" + ".csv";
    ofstream outFile;
    outFile.open(filename, ios::out);
    
    for (auto iter : lc)
    {
        outFile << iter << endl;
    }
    cout << "File saved!" << endl;
    return 0;
}
