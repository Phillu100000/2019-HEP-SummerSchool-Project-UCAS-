#include <string>
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/NTuple.h"
#include "VertexFit/WTrackParameter.h"
#include "MagneticField/IMagneticFieldSvc.h"
#include "MagneticField/MagneticFieldSvc.h"
#include "McDecayModeSvc/McDecayModeSvc.h"
#include "EvtRecEvent/EvtRecTrack.h"
#include "EvtRecEvent/EvtRecEvent.h"

typedef std::vector<int> Vint;
typedef std::vector<HepLorentzVector> Vp4;

class Deltakkpi : public Algorithm {

    public:
    Deltakkpi(const std::string& name, ISvcLocator* pSvcLocator);
    StatusCode initialize();
    StatusCode execute();
    StatusCode finalize();

    int m_ntot,  m_debug;

    void preparePID(EvtRecTrack* track);
    HepLorentzVector getP4(RecEmcShower* gTrk, Hep3Vector origin); 
  
    bool isGoodTrackForKsLambda(EvtRecTrack* trk);
    bool isGoodTrack(EvtRecTrack* trk); 
    bool isGoodShower(EvtRecTrack* itTrk);
    bool iselectron();
    bool iselectron_eop();
    bool ispion();
    bool iskaon();
    bool isGoodks(RecMdcKalTrack* pipTrk, RecMdcKalTrack* pimTrk, double& ks_1chis, double& ks_2chis, double& ks_lchue, HepLorentzVector& p4_ks_1s, double& ks_mass, HepLorentzVector& p4pipks, HepLorentzVector& p4pimks);
    private:
    McDecayModeSvc* m_svc;
    PropertyMgr m_propMgr;
    IMagneticFieldSvc* m_pIMF;
    // ****
    bool decay_kkpi;
 	bool m_HaveKaon;
 	bool m_HavePion;

 	double m_beamE;
    Hep3Vector m_beta;

 	double m_prob[5];
 	double mm_prob[5];
 	double mmm_prob[5];
    double m_eop;
    //***
    //******** new 
    bool m_checktotal;
    //bool m_pid;
    bool m_loosewindow;
    int m_vetocut;
    //***************
    NTuple::Tuple*  m_nt10;
    NTuple::Item<double>    m_mDs_kkpi;
    NTuple::Item<double>    m_mDsgam_kkpi;
    NTuple::Item<double>    m_mrec_Ds_kkpi;
    NTuple::Item<double>    m_eDs_kkpi;
    NTuple::Item<double>    m_delta_E_kkpi;

    NTuple::Item<double>    m_runNo_kkpi;
    NTuple::Item<double>    m_event_kkpi;
    NTuple::Item<double>    m_ngam_kkpi;
    NTuple::Item<double>    m_nchg_kkpi;
    NTuple::Item<double>    m_charge_kkpi;

    NTuple::Array<double>   m_p4_DsST_kkpi;
    NTuple::Array<double>   m_p4_Ds_kkpi;
    NTuple::Array<double>   m_p4_pion_kkpi;
    NTuple::Array<double>   m_p4_kaonm_kkpi;
    NTuple::Array<double>   m_p4_kaonp_kkpi;
    NTuple::Array<double>   m_p4_gam_kkpi;
     
    NTuple::Item<int>       m_pass_flag_kkpi;   
    
    NTuple::Tuple*  m_nt20;
    NTuple::Item<int>       m_total_og;
    NTuple::Item<int>       m_flag_ST_og;
};
