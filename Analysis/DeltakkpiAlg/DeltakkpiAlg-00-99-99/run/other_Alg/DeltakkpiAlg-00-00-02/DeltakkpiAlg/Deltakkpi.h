#include <string>
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/NTuple.h"
#include "DeltakkpiAlg/ReadBeamInfFromDb.h"
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
    void updata1c(int& ikaonp, int& ikaonm, int& ipion, int& igam, bool& upfit, double& DsST_chis, HepLorentzVector& p4_Lab, HepLorentzVector& p4_DsST_1c, HepLorentzVector& p4_Ds_1c, HepLorentzVector& p4_pion_1c, HepLorentzVector& p4_kaonp_1c, HepLorentzVector& p4_kaonm_1c, HepLorentzVector& p4_gam_1c, HepLorentzVector& p4_miss_1c);
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
    ReadBeamInfFromDb m_readDb;

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
    NTuple::Item<double>    m_chargpion_kkpi;

    NTuple::Item<double>    m_mDs_1c;
    NTuple::Item<double>    m_mDsgam_1c;
    NTuple::Item<double>    m_mrec_Ds_1c;
    NTuple::Item<double>    m_mrec_Dsgam_1c;
    NTuple::Item<double>    m_delta_E_1c;
    NTuple::Item<double>    m_upfit_kkpi;
    NTuple::Item<double>    m_chis1c_kkpi;
    NTuple::Array<double>   m_p4_DsST_1c;
    NTuple::Array<double>   m_p4_Ds_1c;
    NTuple::Array<double>   m_p4_pion_1c;
    NTuple::Array<double>   m_p4_kaonp_1c;
    NTuple::Array<double>   m_p4_kaonm_1c;
    NTuple::Array<double>   m_p4_gam_1c;
    NTuple::Array<double>   m_p4_miss_1c;
   
    NTuple::Array<double>   m_p4_DsST_kkpi;
    NTuple::Array<double>   m_p4_Ds_kkpi;
    NTuple::Array<double>   m_p4_pion_kkpi;
    NTuple::Array<double>   m_p4_kaonm_kkpi;
    NTuple::Array<double>   m_p4_kaonp_kkpi;
    NTuple::Array<double>   m_p4_gam_kkpi;
     
    NTuple::Item<int>       m_pass_flag_kkpi;   
    NTuple::Item<int>       m_rec_flag_p_kkpi;
    NTuple::Item<int>       m_rec_flag_m_kkpi;
    NTuple::Item<int>       m_flag_ST_kkpi;
    NTuple::Item<int>       m_flag_mc_p_kkpi;
    NTuple::Item<int>       m_flag_mc_m_kkpi;

    NTuple::Item<int>       m_num_DsSTp_kkpi;
    NTuple::Array<int>      m_id_DsSTp_kkpi;
    NTuple::Matrix<double>  m_ptruth_DsSTp_kkpi;
    
    NTuple::Item<int>       m_num_Dsp_kkpi;
    NTuple::Array<int>      m_id_Dsp_kkpi;
    NTuple::Matrix<double>  m_ptruth_Dsp_kkpi;   
 
    NTuple::Item<int>       m_num_DsSTm_kkpi;
    NTuple::Array<int>      m_id_DsSTm_kkpi;
    NTuple::Matrix<double>  m_ptruth_DsSTm_kkpi;
    
    NTuple::Item<int>       m_num_Dsm_kkpi;
    NTuple::Array<int>      m_id_Dsm_kkpi;
    NTuple::Matrix<double>  m_ptruth_Dsm_kkpi;   

    
    NTuple::Tuple*  m_nt20;
    NTuple::Item<int>       m_total_og;
    NTuple::Item<int>       m_flag_ST_og;
    NTuple::Item<int>       m_flag_mc_p_og;
    NTuple::Item<int>       m_flag_mc_m_og;

    NTuple::Item<int>       m_num_DsST_og;
    NTuple::Array<int>      m_id_DsST_og;
    NTuple::Matrix<double>  m_ptruth_DsST_og;  
    
    NTuple::Item<int>       m_num_Dsp_og;
    NTuple::Array<int>      m_id_Dsp_og;
    NTuple::Matrix<double>  m_ptruth_Dsp_og;   
    
    NTuple::Item<int>       m_num_Dsm_og;
    NTuple::Array<int>      m_id_Dsm_og;
    NTuple::Matrix<double>  m_ptruth_Dsm_og;   
    
    NTuple::Item<int>       m_num_PI0p_og;
    NTuple::Array<int>      m_id_PI0p_og;
    NTuple::Matrix<double>  m_ptruth_PI0p_og;  
    
    NTuple::Item<int>       m_num_PI0m_og;
    NTuple::Array<int>      m_id_PI0m_og;
    NTuple::Matrix<double>  m_ptruth_PI0m_og; 
   
    NTuple::Item<int>       m_num_KS0_og;
    NTuple::Array<int>      m_id_KS0_og;
    NTuple::Matrix<double>  m_ptruth_KS0_og;
    
    NTuple::Item<int>       m_num_KS0p_og;
    NTuple::Array<int>      m_id_KS0p_og;
    NTuple::Matrix<double>  m_ptruth_KS0p_og;   
    
    NTuple::Item<int>       m_num_KS0m_og;
    NTuple::Array<int>      m_id_KS0m_og;
    NTuple::Matrix<double>  m_ptruth_KS0m_og;   
    
    NTuple::Item<int>       m_num_etap_og;
    NTuple::Array<int>      m_id_etap_og;
    NTuple::Matrix<double>  m_ptruth_etap_og;   
    
    NTuple::Item<int>       m_num_etam_og;
    NTuple::Array<int>      m_id_etam_og;
    NTuple::Matrix<double>  m_ptruth_etam_og;   
    
    NTuple::Item<int>       m_num_etaprip_og;
    NTuple::Array<int>      m_id_etaprip_og;
    NTuple::Matrix<double>  m_ptruth_etaprip_og;   
    
    NTuple::Item<int>       m_num_etaprim_og;
    NTuple::Array<int>      m_id_etaprim_og;
    NTuple::Matrix<double>  m_ptruth_etaprim_og;   
    
    NTuple::Item<int>       m_num_etasonp_og;
    NTuple::Array<int>      m_id_etasonp_og;
    NTuple::Matrix<double>  m_ptruth_etasonp_og;   
    
    NTuple::Item<int>       m_num_etasonm_og;
    NTuple::Array<int>      m_id_etasonm_og;
    NTuple::Matrix<double>  m_ptruth_etasonm_og;   
    
    NTuple::Item<int>       m_num_rho0p_og;
    NTuple::Array<int>      m_id_rho0p_og;
    NTuple::Matrix<double>  m_ptruth_rho0p_og;   
    
    NTuple::Item<int>       m_num_rho0m_og;
    NTuple::Array<int>      m_id_rho0m_og;
    NTuple::Matrix<double>  m_ptruth_rho0m_og;   
    

};
