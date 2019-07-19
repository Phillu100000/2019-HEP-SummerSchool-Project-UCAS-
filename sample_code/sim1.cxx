#include<TGraphErrors.h>
#include<TCanvas.h>
#include<TRandom2.h>
#include<TF1.h>
#include<TMath.h>
#include<iostream>
#define N 10000
TCanvas* sim1(){
    auto func=new TF1("con","1",-1,1);
    long long n=0;
    double *aver, *errA, *X;
    aver=new double[N];
    errA=new double[N];
    X=new double[N];
    for (n=0; n < N; n++){
        X[n]=n+1;
    }
    for(n=0; n < N; n++){
        aver[n]=0;
    }
    n=0;
    for(n=0; n < N; n++){
        errA[n]=4/TMath::Sqrt((double)(n+1));
        }
    for (n=0; n < N; n++){
        if(TMath::Power(func->GetRandom(),2)+TMath::Power(func->GetRandom(),2)<1){
            aver[n]++;
        }
        if(n!=0){
            aver[n]+=aver[n-1];
        }
    }
    n=0;
    for(n=0; n < N; n++){
        aver[n]=4*aver[n]/(n+1);
    }
    delete func;
    auto c1=new TCanvas();
    auto graph=new TGraphErrors(N,X,aver,nullptr,errA);
    std::cout<< aver[N-1]<<"\n"<<std::endl;
    graph->Draw();
    c1->Draw();
		c1->SaveSource("k.c");
		return c1;
}

int main(){
    auto x=sim1();
		TFile *f = new TFile("Yan.root","UPDATE");
		f->WriteTObject(x);
    return 0;
}
