//
//  main.cpp
//  EA_sr
//
//  Created by Qianhuang Li on 10/14/22.
//

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<string>
#include<string.h>
#include<sstream>
#include<cstdlib>

#include<istream>
#include<iomanip>
#include<iostream>

//#include<format>
#include<fstream>

#include<typeinfo>
#include<algorithm>
#include<vector>
#include <math.h>
#include <random>
#include<ctype.h>

using namespace std;
int rs_iter = 10000;
int hc_iter = 10000;
int ea_iter = 50000;

string oneline;
int dot;
vector<float> xy;
vector<vector<float> > dot_xy;
float pro = 0.5; // probability of x

vector<float> best_gene;  //
double fitness;         //
double best_fit;        //
vector<double> lc;      //
vector<float> evo_gene;           //
vector<vector<float> > evo_his;    //
vector<float> y_value;
vector<float> best_y;

int level = 6;   //
int len = pow(2, level)-1; //
int len_func = pow(2, level-1)-1; //
vector<float> gene; // binary tree

int pop_size = 100;      //population size
double pc = 1, pm = 0.2;// probability of crossover and mutation
float pr = 0.6;
int x,y;
vector<vector<float> > population;
vector<vector<float> > good_pop;
vector<float> mut;

vector<float> fit;

vector<vector<float> > his_best_gene;
vector<double> diversity;

//generate func
void random_generate(){
    gene.push_back(0);
    for (int i = 1; i <= level; i++){
        if(i<level){
            for(int j =0; j<pow(2, i-1) ;j++){
                gene.push_back(12 + rand()%(17-12+1));
//        cout<< gene[i]<<endl;
            }
        }
        else{
            for(int j =0; j<pow(2, i-1) ;j++){
                if ((rand()%10)/10.0 > pro){  // add a probabilty of x and int
                    gene.push_back(-5);
                }
                else {
                    gene.push_back((-40 +rand()%(40+40+10))/10.0);
                }
            }
        }
    }
}

// form real function
float get_value(float x, vector<float> gene1){
    float value;
    for (int i= len; i>1; i = i-2){
        if (gene1[i] == -5){
            gene1[i] = x;
        }
        if (gene1[i-1] == -5){
            gene1[i-1] = x;
        }
//        if (i == 2){
//            gene1[(i-1)/2] = sin(gene1[i-1]);
//        }
        if(gene1[(i-1)/2] == 14){
            gene1[(i-1)/2] = gene1[i] + gene1[i-1];
        }else if (gene1[(i-1)/2] == 15){
            gene1[(i-1)/2] = gene1[i] - gene1[i-1];
        }else if (gene1[(i-1)/2] == 16){
            gene1[(i-1)/2] = gene1[i] * gene1[i-1];
        }else if (gene1[(i-1)/2] == 17){
            if(gene1[i-1] == 0){
                return 0;
            }
            else{
                gene1[(i-1)/2] = gene1[i] / gene1[i-1];
            }
        }else if (gene1[(i-1)/2] == 12){
            gene1[(i-1)/2] = cos(gene1[i]);

        }else if (gene1[(i-1)/2] == 13){
            gene1[(i-1)/2] = sin(gene1[i]);
        }
    }
    value = gene1[1];
    return value;
}

//mutation func
void mutation(int rc){
    if(rc<len+1){
        if(rc>=pow(2,level-1)){
            mut[rc]= (-40 +rand()%(40+40+10))/10.0;
        }
        else{
            mut[rc]= 12 + rand()%(17-12+1);
            mutation(2*rc);
            mutation(2*rc+1);
        }
    }
}


//crossover func
vector<float> pp;
vector<float> mm;
void crossover(int rc_c){
    if(rc_c<len+1){
        float temp;
        temp = pp[rc_c];
        pp[rc_c]=mm[rc_c];
        mm[rc_c]=temp;
        crossover(2*rc_c);
        crossover(2*rc_c+1);
    }
}

//cal_value func
double Cal_dis(int idx1, vector<float> temp) {
    double y = get_value(dot_xy[idx1][0], temp);
    best_y.push_back(y);
//    cout<<y<<endl;
    double fit = abs(dot_xy[idx1][1] - y);
    return fit;
}

vector<float> p2;
vector<float> y_vec;
void get_y(vector<float> gene){
    y_vec.clear();
    for(int i= 0;i<dot;i++){
        p2 = gene;
        double y = get_value(dot_xy[i][0], p2);
        y_vec.push_back(y);
    }
    his_best_gene.push_back(y_vec);
}

double Total_dis(vector<float> p){
    double t = 0;
    best_y.clear();
    vector<float> p1;
    for (int i = 0; i < dot; i++) {
        p1 = p;
        t += Cal_dis(i, p1);
    }
    t=t/dot;
    return t;
}


//argsort_population

vector<int> argsort(vector<float> &fit){
    
    vector<int> o_fit(fit.size());
        iota(o_fit.begin(), o_fit.end(), 0);

        stable_sort(o_fit.begin(), o_fit.end(),[ &fit](int i1, int i2) {return fit[i1] < fit[i2];});
    
    return o_fit;
}


float diver;
void EA(int ea_iter){
    // form initial parents
    population.clear();
    his_best_gene.clear();
    diversity.clear();
    
    for (int i = 0; i< pop_size; i++){
        gene.clear();
        random_generate();
        population.push_back(gene);
    }
    
    //iterate for const times
    for (int i =0; i<ea_iter; i++){
        
        //crossover
        for (int i = 0; i < pop_size; i+= 2) {
            pp.clear();
            mm.clear();
            
            pp = population[i];
            mm = population[i+1];
            if (rand()%100< 100*pc){

                x=int(1+ rand()%(len_func+1));
                crossover(x);
                population.push_back(pp);
                population.push_back(mm);
            }
            else{
                gene.clear();
                random_generate();
                population.push_back(gene);
                gene.clear();
                random_generate();
                population.push_back(gene);
            }
        }
        
        //mutation
        for (int i = 0; i < pop_size; i++) {
            mut.clear();
            mut = population[i];
            if (rand()%100 < 100*pm){

                y=int(1+ rand()%(len_func+1));
                mutation(y);
                if (Total_dis(mut)<Total_dis(population[i])){
                    population[i] = mut;
//                    cout<< Total_dis(mut)<<endl;
                }
            }
        }
        
        // get all the fitness
        fit.clear();
        for(int i = 0; i< pop_size; i++){
            double total_length;
            total_length = Total_dis(population[i]);
//            cout<< total_length<<endl;
            fit.push_back(total_length);
//            cout<<"--------"<<endl;
        }
        
        // argsort according to fitness
        vector<int> o_fit;
        o_fit  = argsort(fit);
        float best;
        best = fit[o_fit[0]];
        cout<< i<<" "<<best<<endl;
        diversity.push_back(fit[o_fit[99]]-best);
        
        lc.push_back(best);
        
        // record good gene
        good_pop.clear();
        vector<float> good;
        for (int i = 0; i<pop_size*pr; i++ ){
            good = population[o_fit[i]];
            good_pop.push_back(good);
        }

        get_y(good_pop[0]);
        best_gene = population[o_fit[0]];
        //trans good gene to population
        population.clear();
        for(int i = 0; i<pop_size*pr; i++ ){
            population.push_back(good_pop[i]);
        }
        
        //fillin
        for (int i = 0; i<(pop_size-pop_size*pr); i++ ){
            gene.clear();
            random_generate();
            population.push_back(gene);
        }
    }
    
    
}





int main() {
    srand(int(time(NULL)));
    ifstream fin;
    fin.open("data.txt");
    while (!fin.eof())
    {
        
        getline(fin, oneline);
        
        if (oneline == "")
        {
            break;
        }
        istringstream is(oneline);
        float t;
        dot += 1;
        while(is >> t){
            char ch;
            
            xy.push_back(t);
            is >> ch;
        }
        dot_xy.push_back(xy);
//        cout<< xy[0]<<" "<<xy[1]<<endl;
        xy.clear();
    }
    fin.close();
//    cout<<"dot:"<< dot<<endl;
    
    //run code
    string EA_method = "EA";
    string curve = "dot";
    EA(ea_iter);    // random search
    
    string filename = "lc" + EA_method + ".csv";
    ofstream outFile;
    outFile.open(filename, ios::out);
    
    for (auto iter : lc)
    {
        outFile << iter << endl;
    }
    cout << "File saved!" << endl;
    outFile.close();
    
    string filename2 = "his" + curve + ".csv";
    ofstream outFile2;
    outFile2.open(filename2, ios::out);
    
    for (auto iter : his_best_gene)
    {
        for (int i = 0; i < dot; i++)
        {
            outFile2 << iter[i]<<",";
        }
        outFile2 << endl;
    }
    cout << "History File saved!" << endl;
    
    string filename3 = "div" + curve  + ".csv";
    ofstream outFile3;
    outFile3.open(filename3, ios::out);
    
    for (auto iter : diversity)
    {
       
        outFile3 << iter <<endl;
    }
    cout << "diversity File saved!" << endl;

    string filename4 = "g" + curve + ".csv";
    ofstream outFile4;
    outFile4.open(filename4, ios::out);
    
    for (auto iter : best_gene)
    {
       
        outFile4 << iter <<endl;
    }
    cout << "gene File saved!" << endl;
    return 0;

}




