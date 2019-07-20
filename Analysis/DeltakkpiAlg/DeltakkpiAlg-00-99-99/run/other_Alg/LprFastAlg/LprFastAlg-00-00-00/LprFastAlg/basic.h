#ifndef Physics_Analysis_BASIC_H
#define Physics_Analysis_BASIC_H 

#include "VertexFit/WTrackParameter.h"
#include "McDecayModeSvc/McDecayModeSvc.h"
#include "GaudiKernel/NTuple.h"
#include "GaudiKernel/Algorithm.h"
#include "EvtRecEvent/EvtRecTrack.h"
#include "EvtRecEvent/EvtRecDTag.h"
#include "LTagTool/LTagTool.h" 
#include "LTagAlg/ReadBeamInfFromDb.h"
#include "DTagTruthMatchSvc/DTagTruthMatchSvc.h"
#include "VertexFit/Helix.h"
#include "VertexFit/SecondVertexFit.h"
#include "VertexFit/IVertexDbSvc.h"
#include "VertexFit/KalmanKinematicFit.h"
#include "VertexFit/KinematicFit.h"
#include "VertexFit/VertexFit.h"
using CLHEP::HepLorentzVector;
#include <vector>
typedef std::vector<int> Vint;
typedef std::vector<double> Vdou;
typedef std::vector<HepLorentzVector> Vp4;
typedef std::vector<WTrackParameter> VWTrkPara;


class basic : public Algorithm {

		public:
				basic(const std::string& name, ISvcLocator* pSvcLocator);
				StatusCode initialize();
				StatusCode execute();
				StatusCode finalize();

				HepLorentzVector getP4(RecEmcShower* gTrk,Hep3Vector origin);

				HepLorentzVector cal_cms(const HepLorentzVector boost_p4, const HepLorentzVector cal_p4);
				void cal_helicity(const HepLorentzVector A_p4, const HepLorentzVector B_p4,const HepLorentzVector C_p4, double& Cda_B); // A:father B:me C:son

				bool    isGoodTrack(EvtRecTrack* trk);
				bool    isGoodShower(EvtRecTrack* trk);
				void    preparePID(EvtRecTrack* track);
				bool    iselectron();
				bool    iselectron_eop();
				bool    isproton();
				bool    ispion();
				bool    iskaon();
				bool    isGoodpi0(RecEmcShower *shr1,RecEmcShower *shr2,double& pi0_mass,HepLorentzVector& p4_pi0,double& pi0_chis,HepLorentzVector& p4_pi0_1c);
				bool    isGoodeta(RecEmcShower *shr1,RecEmcShower *shr2,double& eta_mass,HepLorentzVector& p4_eta,double& eta_chis,HepLorentzVector& p4_eta_1c);
				bool    isGoodTrackForKsLambda(EvtRecTrack* trk);
				bool    isGoodks(RecMdcKalTrack* pipTrk, RecMdcKalTrack* pimTrk, double& ks_1chis, double& ks_2chis, double& ks_lchue, HepLorentzVector& p4_ks_1s, double& ks_mass);
				bool    isGoodlmd(RecMdcKalTrack* ppTrk, RecMdcKalTrack* pimTrk, double& lmd_1chis, double& lmd_2chis, double& lmd_lchue, HepLorentzVector& p4_lmd_1s, double& lmd_mass);

				bool    pass_vf(VWTrkPara &vwtrkpara, VertexParameter &birth, double &vf_chi2);
				bool    pass_kf(const VWTrkPara &vwtrkpara_charge,const VWTrkPara &vwtrkpara_photon, const VertexParameter birth, double &kf_chi2, Vp4 &p4fit_charge, Vp4 &p4fit_photon);

		private:
				bool m_pid;
				bool m_match;
				bool m_HaveEle;
				bool m_HaveKaon;
				bool m_HavePion;
				bool m_HaveProton;
				bool m_HaveKs;
				bool m_HaveLambda;
				bool m_HavePi0;
				bool m_HaveEta;


				bool m_debug;
				bool m_checktotal;

				double m_vr0cut;
				double m_vz0cut;
				double m_CosThetaCut;
				/// Individual photon cuts

				double m_minEnergy;
				double m_gammaAngleCut;
				double m_gammatlCut;  
				double m_gammathCut;  
				double m_maxCosThetaBarrel;
				double m_minCosThetaEndcap;
				double m_maxCosThetaEndcap;
				double m_minEndcapEnergy;

				double m_ecms;

				double m_prob[5];
				double mm_prob[5];
				double mmm_prob[5];
				double m_eop;



				McDecayModeSvc* m_svc;

				NTuple::Tuple*  m_tuple1;  
				NTuple::Item<int>    m_evtNo_;
				NTuple::Item<int>    m_runNo_;
				NTuple::Item<int>    m_mode1_;
				NTuple::Item<int>    m_mode2_;
				NTuple::Item<int>    m_mode3_;
				NTuple::Item<int>    m_ndaughterAp_;
				NTuple::Array<int>   m_Ap_id_;
				NTuple::Matrix<double>   m_Ap_ptruth_;
				NTuple::Item<int>    m_ndaughterAm_;
				NTuple::Array<int>   m_Am_id_;
				NTuple::Matrix<double>   m_Am_ptruth_;

				NTuple::Tuple*  m_tuple_tree;  
				NTuple::Item<int>    m_evtNo;
				NTuple::Item<int>    m_runNo;
				NTuple::Item<int>    m_mode1;
				NTuple::Item<int>    m_mode2;
				NTuple::Item<int>    m_mode3;
				NTuple::Item<int>   m_mcparticle;
				NTuple::Array<int>  m_pdgid;
				NTuple::Array<int>  m_motheridx;
				NTuple::Item<int>   m_mcparticle_p;
				NTuple::Array<int>  m_pdgid_p;
				NTuple::Array<int>  m_motheridx_p;
				NTuple::Item<int>   m_mcparticle_m;
				NTuple::Array<int>  m_pdgid_m;
				NTuple::Array<int>  m_motheridx_m;
				NTuple::Item<int>    m_ndaughterAp;
				NTuple::Array<int>   m_Ap_id;
				NTuple::Matrix<double>   m_Ap_ptruth;
				NTuple::Item<int>    m_ndaughterAm;
				NTuple::Array<int>   m_Am_id;
				NTuple::Matrix<double>   m_Am_ptruth;

				NTuple::Item<int> m_rightflag;

				NTuple::Array<double>   m_proton_p4;
				NTuple::Array<double>   m_Ks_p4;
				NTuple::Array<double>   m_Eta_p4;

				NTuple::Item<double>   m_Ks_mass;
				NTuple::Item<double>   m_Ks_chis1;
				NTuple::Item<double>   m_Ks_lchue;
				NTuple::Item<double>   m_Eta_mass;
				NTuple::Item<double>   m_Eta_chis;

				NTuple::Item<double>   m_deltaE;
				NTuple::Item<double>   m_mbc;
				//*******************************************************************************

};
#endif 
