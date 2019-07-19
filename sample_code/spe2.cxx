#include<TF1.h>
#include<TMath.h>
#include<TH1.h>
#include<time.h>
#include<TFile.h>

void spe2(){
    auto file=new TFile;
    file->Open("./2spe.root");
    auto hist=new TH1F;
    auto func=new TF1("x","[0]*TMath::Gaus(x,[1],[2])+[3]*TMath::Gaus(x,[4],[5])",-20,100);
    for (int i=0;i<=5;i++){
        func->SetParameter(i,i+1);
    }
    hist->Read("f");
    hist->Fit(func);
    hist->Draw();
}