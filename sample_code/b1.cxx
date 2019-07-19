#include <TH1.h>
#include <TF1.h>
#include <fstream>

void b1(){
    auto hist=new TH1I("h1","Gaus",100,-20,20);
    std::ifstream file("../1bg.txt");
    double x=0;
    int i=0;
    while (file>>x){
        
        hist->Fill(x);
        i++;
        if(i%100==0){
            std::cout << i <<"\n"<<std::endl<<x;
        }
    }
    hist->Fit("gaus");
    hist->Draw();
}

