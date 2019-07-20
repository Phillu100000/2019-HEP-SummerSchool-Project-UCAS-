#ifndef PKPiCheckAlg_Header
#define PKPiCheckAlg_Header

#include "GaudiKernel/Algorithm.h"
//you can add oher necessary header files
#include "GaudiKernel/NTuple.h"
#include "McDecayModeSvc/McDecayModeSvc.h"
#include "BestDTagSvc/BestDTagSvc.h"

//
//namespace
//
class PKPiCheckAlg:public Algorithm {
  public:
    PKPiCheckAlg(const std::string& name, ISvcLocator* pSvcLocator);
    ~PKPiCheckAlg();
    StatusCode initialize();
    StatusCode beginRun();   
    StatusCode execute();
    StatusCode endRun();
    StatusCode finalize();

    bool Is_good_trk(EvtRecTrackIterator itTrk, double &vz, double &vxy);
    bool IsPronton(EvtRecTrackIterator itTrk);
    bool IsPion (EvtRecTrackIterator itTrk);
    bool IsKaon (EvtRecTrackIterator itTrk);

  private:

     int m_irun;
     bool m_debug;
     bool m_use_Total_TOF;
     bool m_BestCandidate;

    double m_vr0cut;
    double m_vz0cut;
    double m_skim;
    double m_beamE;
    double m_ecms;
    double m_prob_cut;

    NTuple::Tuple* m_tuple1;   
    NTuple::Item<int> m_run;
    NTuple::Item<int> m_event;
    NTuple::Item<int> m_idxmc;
    NTuple::Array<int>m_pdgid;
    NTuple::Array<int>m_motheridx;

    NTuple::Item<int>m_flag_p;
    NTuple::Item<int>m_charge;
    NTuple::Item<int>m_ngood;
    NTuple::Item<double>m_mass;
    NTuple::Item<double>m_ebeam;
    NTuple::Item<double>m_mbc;
    NTuple::Item<double>m_deltaE;
    NTuple::Item<int>m_index;
    NTuple::Matrix<double>m_fourmom;
    NTuple::Array<double>m_xy;
    NTuple::Array<double>m_mom;
    NTuple::Array<double>m_z;
    NTuple::Item<double>m_oa_ppi;
    NTuple::Item<double>m_oa_pk;
    NTuple::Item<double>m_oa_kpi;
    NTuple::Item<double>m_diphi_ppi;
    NTuple::Item<double>m_diphi_pk;
    NTuple::Item<double>m_diphi_kpi;
    NTuple::Item<double>m_phi_p;
    NTuple::Item<double>m_phi_k;
    NTuple::Item<double>m_phi_pi;
    NTuple::Item<double>m_theta_p;
    NTuple::Item<double>m_theta_k;
    NTuple::Item<double>m_theta_pi;


    NTuple::Tuple* m_tuple2;
    NTuple::Item<int> m_run2;
    NTuple::Item<int> m_event2;
    //NTuple::Item<int> m_idxmc;
    //NTuple::Array<int>m_pdgid;
    //NTuple::Array<int>m_motheridx;

    NTuple::Item<int>m_flag_m;
    NTuple::Item<int>m_charge2;
    NTuple::Item<int>m_ngood2;
    NTuple::Item<double>m_mass2;
    NTuple::Item<double>m_ebeam2;
    NTuple::Item<double>m_mbc2;
    NTuple::Item<double>m_deltaE2;
    NTuple::Item<int>m_index2;
    NTuple::Matrix<double>m_fourmom2;
    NTuple::Array<double>m_mom2;
    NTuple::Array<double>m_xy_2;
    NTuple::Array<double>m_z_2;
    NTuple::Item<double>m_oa_ppi2;
    NTuple::Item<double>m_oa_pk2;
    NTuple::Item<double>m_oa_kpi2;
    NTuple::Item<double>m_diphi_ppi2;
    NTuple::Item<double>m_diphi_pk2;
    NTuple::Item<double>m_diphi_kpi2;
    NTuple::Item<double>m_phi_p2;
    NTuple::Item<double>m_phi_k2;
    NTuple::Item<double>m_phi_pi2;
    NTuple::Item<double>m_theta_p2;
    NTuple::Item<double>m_theta_k2;
    NTuple::Item<double>m_theta_pi2;



  protected:

};
//add your inline methods

//

#endif//PKPiCheckAlg_Header
