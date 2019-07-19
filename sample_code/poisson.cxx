#include<TGraphErrors.h>
#include<TCanvas.h>
#include<TRandom2.h>
#include<TF1.h>
#include<TMath.h>
#include<TH1.h>
#include<time.h>
#include<TMultiGraph.h>
TRandom2 Urand(time(nullptr));
const int N = 100;
const double p = 0.2;

void poisson(){    
    auto hist=new TH1I("h1","possion",100,0,N);
    auto mg=new TMultiGraph; 
    auto c1=new TCanvas;
    TF1 *f1 = new TF1("f1","[0]*TMath::Power(([1]/[2]),(x/[2]))*(TMath::Exp(-([1]/[2])))/TMath::Gamma((x/[2])+1.)", 0, N);
    f1->SetParameters(1, 1, 1);
    int count=0;
    for (int i=0; i < 500; i++){
        for (int j=0 ; j < N; j++){
            if (Urand.Uniform()<0.2){
                count++;
            }
        }
        hist->Fill(count);
        hist->Draw();
        c1->Update();
        count=0;
    }
    hist->Fit("f1","R");
    //hist->Draw("AH");

    c1->Draw();
}