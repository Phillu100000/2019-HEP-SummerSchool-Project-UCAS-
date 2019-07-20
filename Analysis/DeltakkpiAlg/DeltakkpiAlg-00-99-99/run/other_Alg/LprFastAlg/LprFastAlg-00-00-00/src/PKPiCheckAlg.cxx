#include "LprFastAlg/PKPiCheckAlg.h"
#include "McDecayModeSvc/McDecayModeSvc.h"
#include "BestDTagSvc/BestDTagSvc.h"

#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/PropertyMgr.h"

#include "EventModel/EventModel.h"
#include "EventModel/Event.h"
#include "EventModel/EventHeader.h"

#include "EvtRecEvent/EvtRecEvent.h"
#include "EvtRecEvent/EvtRecTrack.h"
#include "EvtRecEvent/EvtRecDTag.h"
#include "EvtRecEvent/EvtRecVeeVertex.h"
#include "EvtRecEvent/EvtRecPi0.h"
#include "DstEvent/TofHitStatus.h"

#include "TMath.h"
#include "GaudiKernel/INTupleSvc.h"
#include "GaudiKernel/NTuple.h"
#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/IHistogramSvc.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Vector/LorentzVector.h"
#include "CLHEP/Vector/TwoVector.h"
#include "CLHEP/Geometry/Point3D.h"


#include "McTruth/McParticle.h"
#include "EvTimeEvent/RecEsTime.h"
#include "MdcRecEvent/RecMdcKalTrack.h"
using CLHEP::Hep3Vector;
using CLHEP::Hep2Vector;
using CLHEP::HepLorentzVector;

#include "VertexFit/Helix.h" 
#include "VertexFit/IVertexDbSvc.h"
#include "VertexFit/KinematicFit.h"
#include "VertexFit/VertexFit.h"
#include "ParticleID/ParticleID.h"
#include "CLHEP/Geometry/Point3D.h"

#ifndef ENABLE_BACKWARDS_COMPATIBILITY
typedef HepGeom::Point3D<double> HepPoint3D;
#endif

#include <vector>
const double xmass[5] = {0.000511, 0.105658, 0.139570,0.493677, 0.938272};
typedef std::vector<int> Vint;
typedef std::vector<double> VDouble;
typedef std::vector<HepLorentzVector> Vp4;

static long m_cout_all(0), m_cout_ngood(0), m_cout_pkpi(0),m_cout_skim(0);


PKPiCheckAlg::PKPiCheckAlg(const std::string& name, ISvcLocator* pSvcLocator):Algorithm(name,pSvcLocator){
		declareProperty("Vr0cut",   m_vr0cut=5.0);
		declareProperty("Vz0cut",   m_vz0cut=20.0);
		declareProperty("Probcut",   m_prob_cut=0.0);
		declareProperty("BestCandidate",   m_BestCandidate=false);
		declareProperty("UseOverallTof",   m_use_Total_TOF=false);
		declareProperty("SkimFlag", m_skim=false);
		declareProperty("beamE", m_beamE=2.300);
		declareProperty("ecms", m_ecms=4.600);
		declareProperty("debug", m_debug=false);
}
PKPiCheckAlg::~PKPiCheckAlg(){
		//add your code for deconstructor

}
StatusCode PKPiCheckAlg::initialize(){
		MsgStream log(msgSvc(), name());
		log<<MSG::INFO<<"PKPiCheckAlg::initialize()"<<endreq;
		//add your code here
		StatusCode status;
		NTuplePtr nt1(ntupleSvc(), "FILE1/Lp");
		if ( nt1 ) m_tuple1 = nt1;
		else {
				m_tuple1 = ntupleSvc()->book ("FILE1/Lp", CLID_ColumnWiseTuple, "exam N-Tuple example");
				if ( m_tuple1 )    {
						status = m_tuple1->addItem ("run",   m_run);
						status = m_tuple1->addItem ("event", m_event);
						//status = m_tuple1->addItem("indexmc",          m_idxmc, 0, 100);
						//status = m_tuple1->addIndexedItem("pdgid",     m_idxmc, m_pdgid);
						//status = m_tuple1->addIndexedItem("motheridx", m_idxmc, m_motheridx);

						status = m_tuple1->addItem ("flag_p", m_flag_p);
						status = m_tuple1->addItem ("charge", m_charge);
						status = m_tuple1->addItem ("nGood", m_ngood);
						status = m_tuple1->addItem ("Ebeam", m_ebeam);
						status = m_tuple1->addItem ("mBC", m_mbc);
						status = m_tuple1->addItem ("mass",m_mass);
						status = m_tuple1->addItem ("De", m_deltaE);
						status = m_tuple1->addItem ("index", m_index,0,3);
						status = m_tuple1->addIndexedItem ("fourmom",m_index, 4, m_fourmom);
						status = m_tuple1->addIndexedItem ("mom",m_index, m_mom);
						status = m_tuple1->addIndexedItem ("Rxy",m_index, m_xy);
						status = m_tuple1->addIndexedItem ("Rz",m_index, m_z);
						status = m_tuple1->addItem ("oa_ppi",m_oa_ppi);
						status = m_tuple1->addItem ("oa_pk",m_oa_pk);
						status = m_tuple1->addItem ("oa_kpi",m_oa_kpi);
						status = m_tuple1->addItem ("dphi_ppi",m_diphi_ppi);
						status = m_tuple1->addItem ("dphi_pk",m_diphi_pk);
						status = m_tuple1->addItem ("dphi_kpi",m_diphi_kpi);
						status = m_tuple1->addItem ("phi_p",m_phi_p);
						status = m_tuple1->addItem ("phi_k",m_phi_k);
						status = m_tuple1->addItem ("phi_pi",m_phi_pi);
						status = m_tuple1->addItem ("theta_p",m_theta_p);
						status = m_tuple1->addItem ("theta_k",m_theta_k);
						status = m_tuple1->addItem ("theta_pi",m_theta_pi);
				}
				else    {
						log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple1) << endmsg;
						return StatusCode::FAILURE;
				}
		}

		NTuplePtr nt2(ntupleSvc(), "FILE1/Lm");
		if ( nt2 ) m_tuple2 = nt2;
		else {
				m_tuple2 = ntupleSvc()->book ("FILE1/Lm", CLID_ColumnWiseTuple, "exam N-Tuple example");
				if ( m_tuple2 )    {
						status = m_tuple2->addItem ("run",   m_run2);
						status = m_tuple2->addItem ("event", m_event2);
						//status = m_tuple2->addItem("indexmc",          m_idxmc, 0, 100);
						//status = m_tuple2->addIndexedItem("pdgid",     m_idxmc, m_pdgid_2);
						//status = m_tuple2->addIndexedItem("motheridx", m_idxmc, m_motheridx_2);

						status = m_tuple2->addItem ("charge", m_charge2);
						status = m_tuple2->addItem ("flag_m", m_flag_m);
						status = m_tuple2->addItem ("nGood", m_ngood2);
						status = m_tuple2->addItem ("Ebeam", m_ebeam2);
						status = m_tuple2->addItem ("mBC", m_mbc2);
						status = m_tuple2->addItem ("mass",m_mass2);
						status = m_tuple2->addItem ("De", m_deltaE2);
						status = m_tuple2->addItem ("index", m_index2,0, 3);
						status = m_tuple2->addIndexedItem ("fourmom",m_index2 , 4, m_fourmom2);
						status = m_tuple2->addIndexedItem ("mom",m_index2 , m_mom2);
						status = m_tuple2->addIndexedItem ("Rxy",m_index2 , m_xy_2);
						status = m_tuple2->addIndexedItem ("Rz",m_index2 , m_z_2);
						status = m_tuple2->addItem ("oa_ppi",m_oa_ppi2);
						status = m_tuple2->addItem ("oa_pk",m_oa_pk2);
						status = m_tuple2->addItem ("oa_kpi",m_oa_kpi2);
						status = m_tuple2->addItem ("dphi_ppi",m_diphi_ppi2);
						status = m_tuple2->addItem ("dphi_pk",m_diphi_pk2);
						status = m_tuple2->addItem ("dphi_kpi",m_diphi_kpi2);
						status = m_tuple2->addItem ("phi_p",m_phi_p2);
						status = m_tuple2->addItem ("phi_k",m_phi_k2);
						status = m_tuple2->addItem ("phi_pi",m_phi_pi2);
						status = m_tuple2->addItem ("theta_p",m_theta_p2);
						status = m_tuple2->addItem ("theta_k",m_theta_k2);
						status = m_tuple2->addItem ("theta_pi",m_theta_pi2);
				}
				else    {
						log << MSG::ERROR << "    Cannot book N-tuple:" << long(m_tuple2) << endmsg;
						return StatusCode::FAILURE;
				}
		}


		return StatusCode::SUCCESS;

}
StatusCode PKPiCheckAlg::beginRun(){
		MsgStream log(msgSvc(), name());
		log<<MSG::INFO<<"PKPiCheckAlg::beginRun()"<<endreq;
		//add your code here
		return StatusCode::SUCCESS;

}
StatusCode PKPiCheckAlg::execute(){
		MsgStream log(msgSvc(), name());
		log<<MSG::INFO<<"PKPiCheckAlg::execute()"<<endreq;
		SmartDataPtr<Event::EventHeader> eventHeader(eventSvc(),"/Event/EventHeader");
		m_cout_all++;

		//initi
		m_flag_m=-10;
		m_flag_p=-10;
		int runNo = eventHeader->runNumber();
		int eventNo = eventHeader->eventNumber();

		log << MSG::DEBUG <<"run, evtnum = "
				<< runNo << " , "
				<< eventNo <<endreq;

		SmartDataPtr<EvtRecEvent> evtRecEvent(eventSvc(), EventModel::EvtRec::EvtRecEvent);
		log << MSG::DEBUG <<"ncharg, nneu, tottks = "
				<< evtRecEvent->totalCharged() << " , "
				<< evtRecEvent->totalNeutral() << " , "
				<< evtRecEvent->totalTracks() <<endreq;

		SmartDataPtr<EvtRecTrackCol> evtRecTrkCol(eventSvc(),  EventModel::EvtRec::EvtRecTrackCol);

		/*if (eventHeader->runNumber()<0)
			{
			SmartDataPtr<Event::McParticleCol> mcParticleCol(eventSvc(), "/Event/MC/McParticleCol");

			int m_numParticle = 0;
			if (!mcParticleCol)
			{
			std::cout << "Could not retrieve McParticelCol" << std::endl;
		// return StatusCode::FAILURE;
		}
		else
		{
		bool psippDecay = false;
		int rootIndex = -1;
		Event::McParticleCol::iterator iter_mc = mcParticleCol->begin();
		for (; iter_mc != mcParticleCol->end(); iter_mc++)
		{
		if ((*iter_mc)->primaryParticle()) continue;
		//if (!(*iter_mc)->decayFromGenerator()) continue;
		if ((*iter_mc)->particleProperty()==30443)
		{
		psippDecay = true;
		//rootIndex = (*iter_mc)->trackIndex();
		}
		int mcidx = ((*iter_mc)->mother()).trackIndex();
		int pdgid = (*iter_mc)->particleProperty();
		m_pdgid[m_numParticle] = pdgid;
		m_motheridx[m_numParticle] = mcidx;
		m_numParticle += 1;
		}
		} 
		m_idxmc = m_numParticle;
		}
		 */


		Vint iGood;iGood.clear(); Vint iProton; iProton.clear(); Vint iPion; iPion.clear(); Vint iKaon; iKaon.clear();
		VDouble Rxy;Rxy.clear(); VDouble Rz; Rz.clear();
		Vint iProtonm; iProtonm.clear(); Vint iProtonp; iProtonp.clear();
		Vint iPionm; iPionm.clear();  Vint iPionp; iPionp.clear();
		Vint iKaonm; iKaonm.clear();  Vint iKaonp; iKaonp.clear();

		VDouble Rxy_pm,Rz_pm; Rxy_pm.clear(); Rz_pm.clear();
		VDouble Rxy_km,Rz_km; Rxy_km.clear(); Rz_km.clear();
		VDouble Rxy_pim,Rz_pim; Rxy_pim.clear(); Rz_pim.clear();
		VDouble Rxy_pp,Rz_pp; Rxy_pp.clear(); Rz_pp.clear();
		VDouble Rxy_kp,Rz_kp; Rxy_kp.clear(); Rz_kp.clear();
		VDouble Rxy_pip,Rz_pip; Rxy_pip.clear(); Rz_pip.clear();

		if(m_debug) cout<<"ngood is "<<iGood.size()<<endl;

		for(int i=0; i<evtRecEvent->totalCharged(); i++){
				EvtRecTrackIterator itTrk=evtRecTrkCol->begin() + i;
				double vz=-10, vxy=-10;
				if(!(Is_good_trk(itTrk, vz, vxy)))  continue;
				iGood.push_back(i);
				if(m_debug) cout<<"vz, vxy "<<vz << " , " << vxy <<endl;
				Rxy.push_back(vxy);
				Rz.push_back(vz);
				//m_vz[cnt_good] = vz;
				//vz_mean+=vz;
				//m_vxy[cnt_good] = vxy;
				//cnt_good++;
		}

		if(m_debug) cout<<"ngood is "<<iGood.size()<<endl;

		if(iGood.size()<3) return StatusCode::SUCCESS;
		m_ngood=iGood.size();
		m_ngood2=iGood.size();
		m_cout_ngood++;

		int const Ng=iGood.size();

		//double rxy[Ng][3]={-10}, rz[Ng][3]={-10};
		//double rxy_m[Ng][3]={-10}, rz_m[Ng][3]={-10};

		for(int i=0; i<iGood.size(); i++)
		{
				EvtRecTrackIterator itTrk=evtRecTrkCol->begin() + iGood[i];
				RecMdcTrack* mdcTrk = (*itTrk)->mdcTrack();
				int charge =  mdcTrk->charge();
				if(IsPion(itTrk)){iPion.push_back(iGood[i]); 
						if(charge>0) {
								iPionp.push_back(iGood[i]);
								Rxy_pip.push_back(Rxy[i]);Rz_pip.push_back(Rz[i]);
						} else { 
								iPionm.push_back(iGood[i]);
								Rxy_pim.push_back(Rxy[i]);Rz_pim.push_back(Rz[i]);
						} }
						if(IsKaon(itTrk)){iKaon.push_back(iGood[i]); 
								if(charge>0) { iKaonp.push_back(iGood[i]); Rxy_kp.push_back(Rxy[i]);Rz_kp.push_back(Rz[i]);} 
								else { iKaonm.push_back(iGood[i]);
										Rxy_km.push_back(Rxy[i]);Rz_km.push_back(Rz[i]); 
								} }
								if(IsPronton(itTrk)){iProton.push_back(iGood[i]); 
										if(charge>0) {iProtonp.push_back(iGood[i]); 
												Rxy_pp.push_back(Rxy[i]);Rz_pp.push_back(Rz[i]);
										}

										else { iProtonm.push_back(iGood[i]); Rxy_pm.push_back(Rxy[i]);Rz_pm.push_back(Rz[i]); } }
		}

		if(iPion.size()<1) return StatusCode::SUCCESS;
		if(iKaon.size()<1) return StatusCode::SUCCESS;
		if(iProton.size()<1) return StatusCode::SUCCESS;
		m_cout_pkpi++;

		double m_beamE=2.3;

		HepLorentzVector proton_p4, kaon_p4, pion_p4;

		int cnt_Lp=0, cnt_Lm=0; 
		double tmp_De_lp=999, tmp_De_lm=999;

		//lambda + --> p+ K- pi+
		for(int i=0;i<iProtonp.size();i++)
		{
				EvtRecTrackIterator itTrk=evtRecTrkCol->begin() + iProtonp[i];
				RecMdcKalTrack::setPidType  (RecMdcKalTrack::proton);
				//RecMdcKalTrack *kaltrk = (*itTrk)->mdcKalTrack();
				proton_p4 = ((*itTrk)->mdcKalTrack())->p4(xmass[4]);
				for(int j=0;j<iKaonm.size();j++)
				{
						itTrk=evtRecTrkCol->begin() + iKaonm[j];
						RecMdcKalTrack::setPidType  (RecMdcKalTrack::kaon);
						kaon_p4 = ((*itTrk)->mdcKalTrack())->p4(xmass[3]);
						for(int t=0;t<iPionp.size();t++)
						{
								itTrk=evtRecTrkCol->begin() + iPionp[t];
								RecMdcKalTrack::setPidType  (RecMdcKalTrack::pion);
								pion_p4 = ((*itTrk)->mdcKalTrack())->p4(xmass[2]);

								//boost 
								HepLorentzVector cms(0,0,0,m_ecms);
								HepLorentzVector tot_p4= proton_p4+kaon_p4+pion_p4;
								HepLorentzVector tot_p4_boost;
								tot_p4_boost = tot_p4.boost(-0.011,0.,0.);
								double mbc2 = m_beamE*m_beamE- tot_p4_boost.v().mag2();
								double mbc = mbc2 > 0 ? sqrt( mbc2 ) : -10;
								double deltaE = tot_p4_boost.t() - m_beamE;
								if(deltaE<-0.5||deltaE>0.5) continue;
								if(mbc<1.5||mbc>3.5) continue;
								if(m_BestCandidate&&mbc>2.25&&mbc<2.3)
								{
										if(fabs(deltaE)<tmp_De_lp)
										{
												cnt_Lp++;
												tmp_De_lp=deltaE;
										}
										else continue;
								}
								//book
								m_mass = tot_p4.m();
								for(int pp=0;pp<4;pp++)
								{
										m_fourmom[0][pp] = proton_p4[pp];
										m_fourmom[1][pp] = kaon_p4[pp];
										m_fourmom[2][pp] = pion_p4[pp];
								}
								m_xy[0]=Rxy_pp[i];m_z[0]=Rz_pp[i];
								m_xy[1]=Rxy_km[j];m_z[1]=Rz_km[j];
								m_xy[2]=Rxy_pip[t];m_z[2]=Rz_pip[t];
								m_mom[0] = proton_p4.vect().mag();
								m_mom[1] = kaon_p4.vect().mag();
								m_mom[2] = pion_p4.vect().mag();
								m_flag_p=1;
								m_charge=1;
								m_run=runNo;
								m_event=eventNo;
								m_ebeam = m_beamE;
								m_mbc=mbc;
								m_deltaE=deltaE;
								m_ngood=iGood.size();
								m_phi_p=proton_p4.vect().phi();
								m_phi_k=kaon_p4.vect().phi();
								m_phi_pi=pion_p4.vect().phi();
								m_theta_p=proton_p4.vect().theta();
								m_theta_k=kaon_p4.vect().theta();
								m_theta_pi=pion_p4.vect().theta();
								m_oa_ppi= proton_p4.vect().angle(pion_p4.vect());
								m_oa_pk = proton_p4.vect().angle(kaon_p4.vect());
								m_oa_kpi= kaon_p4.vect().angle(pion_p4.vect());
								m_diphi_ppi= proton_p4.vect().phi()-pion_p4.vect().phi();
								m_diphi_pk = proton_p4.vect().phi()-kaon_p4.vect().phi();
								m_diphi_kpi= kaon_p4.vect().phi()-pion_p4.vect().phi();
								m_index=3;

								m_tuple1->write();
						}

				}

		}

		HepLorentzVector proton_p4_2, kaon_p4_2, pion_p4_2;
		//lamdac- -- > p- K+ pi-
		for(int i=0;i<iProtonm.size();i++)
		{
				EvtRecTrackIterator itTrk=evtRecTrkCol->begin() + iProtonm[i];
				RecMdcKalTrack::setPidType  (RecMdcKalTrack::proton);
				proton_p4_2 =  ((*itTrk)->mdcKalTrack())->p4(xmass[4]);
				for(int j=0;j<iKaonp.size();j++)
				{
						itTrk=evtRecTrkCol->begin() + iKaonp[j];
						RecMdcKalTrack::setPidType  (RecMdcKalTrack::kaon);
						kaon_p4_2 = ((*itTrk)->mdcKalTrack())->p4(xmass[3]);
						for(int t=0;t<iPionm.size();t++)
						{
								itTrk=evtRecTrkCol->begin() + iPionm[t];
								RecMdcKalTrack::setPidType  (RecMdcKalTrack::pion);
								pion_p4_2 = ((*itTrk)->mdcKalTrack())->p4(xmass[2]);

								//boost 
								HepLorentzVector cms(0,0,0,m_ecms);
								HepLorentzVector tot_p4= proton_p4_2+kaon_p4_2+pion_p4_2;
								HepLorentzVector tot_p4_boost;
								tot_p4_boost = tot_p4.boost(-0.011,0.,0.);
								double mbc2 = m_beamE*m_beamE- tot_p4_boost.v().mag2();
								double mbc = mbc2 > 0 ? sqrt( mbc2 ) : -10;
								double deltaE = tot_p4_boost.t() - m_beamE;

								if(deltaE<-0.5||deltaE>0.5) continue;
								if(mbc<1.5||mbc>3.5) continue;
								if(m_BestCandidate&&mbc>2.25&&mbc<2.3)
								{
										if(fabs(deltaE)<tmp_De_lm)
										{
												cnt_Lp++;
												tmp_De_lm=deltaE;
										}
										else continue;
								}

								//book 
								m_mass2 = tot_p4.m();
								for(int pp=0;pp<4;pp++)
								{
										m_fourmom2[0][pp] = proton_p4_2[pp];
										m_fourmom2[1][pp] = kaon_p4_2[pp];
										m_fourmom2[2][pp] = pion_p4_2[pp];
								}
								m_mom2[0] = proton_p4_2.vect().mag();
								m_mom2[1] = kaon_p4_2.vect().mag();
								m_mom2[2] = pion_p4_2.vect().mag();
								m_xy_2[0]=Rxy_pm[i];m_z_2[0]=Rz_pm[i];
								m_xy_2[1]=Rxy_kp[j];m_z_2[1]=Rz_kp[j];
								m_xy_2[2]=Rxy_pim[t];m_z_2[2]=Rz_pim[t];
								m_flag_m=1;
								m_charge2=-1;
								m_run2=runNo;
								m_event2=eventNo;
								m_ebeam2 = m_beamE;
								m_mbc2=mbc;
								m_deltaE2=deltaE;
								m_phi_p2=proton_p4_2.vect().phi();
								m_phi_k2=kaon_p4_2.vect().phi();
								m_phi_pi2=pion_p4_2.vect().phi();
								m_theta_p2=proton_p4_2.vect().theta();
								m_theta_k2=kaon_p4_2.vect().theta();
								m_theta_pi2=pion_p4_2.vect().theta();
								m_oa_ppi2= proton_p4_2.vect().angle(pion_p4_2.vect());
								m_oa_pk2= proton_p4_2.vect().angle(kaon_p4_2.vect());
								m_oa_kpi2= kaon_p4_2.vect().angle(pion_p4_2.vect());
								m_diphi_ppi2= proton_p4_2.vect().phi()-pion_p4_2.vect().phi();
								m_diphi_pk2= proton_p4_2.vect().phi()-kaon_p4_2.vect().phi();
								m_diphi_kpi2= kaon_p4_2.vect().phi()-pion_p4_2.vect().phi();
								m_ngood2=iGood.size();
								m_index2=3;
								m_tuple2->write();
						} 

				}   

		}     



		return StatusCode::SUCCESS;

}
StatusCode PKPiCheckAlg::endRun(){
		MsgStream log(msgSvc(), name());
		log<<MSG::INFO<<"PKPiCheckAlg::endRun()"<<endreq;
		//add your code here
		return StatusCode::SUCCESS;

}
StatusCode PKPiCheckAlg::finalize(){
		MsgStream log(msgSvc(), name());
		log<<MSG::INFO<<"PKPiCheckAlg::finalize()"<<endreq;
		cout<<"total events "<< m_cout_all <<endl;
		cout<<"pass ngood  "<< m_cout_ngood <<endl;
		cout<<"pass p k pi  "<< m_cout_pkpi <<endl;

		//add your code here
		return StatusCode::SUCCESS;
}

bool PKPiCheckAlg::Is_good_trk(EvtRecTrackIterator itTrk, double &vz , double &vxy) {
		double CosThetaCut= 0.93;
		if ( !(*itTrk)->isMdcTrackValid() ) return false;
		if ( !(*itTrk)->isMdcKalTrackValid() ) return false;
		RecMdcTrack* mdcTrk = (*itTrk)->mdcTrack();

		Hep3Vector xorigin(0,0,0);
		IVertexDbSvc*  vtxsvc;
		Gaudi::svcLocator()->service("VertexDbSvc", vtxsvc);
		if(vtxsvc->isVertexValid()){
				double* dbv = vtxsvc->PrimaryVertex();
				double* vv = vtxsvc->SigmaPrimaryVertex();
				//pretect
				if(vtxsvc->PrimaryVertex()[0]>100||vtxsvc->PrimaryVertex()[1]>100 || vtxsvc->PrimaryVertex()[2]>100)
				{
						cout<<"Vertex is abnormal! check your jobOption "<<endl;
						dbv[0]=0;
						dbv[1]=0;
						dbv[2]=0;
				}       
				if(m_debug)cout<<dbv[0]<< " , "<< dbv[1] <<" , "<< dbv[2] <<endl;
				xorigin.setX(dbv[0]);
				xorigin.setY(dbv[1]);
				xorigin.setZ(dbv[2]);
		}
		else { cout<<"warning !!! IVertexDbSvc is inValid!"<<endl;}
		if(m_debug) cout<<" xorigin : "<< xorigin[0] << " , "<< xorigin[1]<< " , "<< xorigin[2]<<endl;

		HepVector a = mdcTrk->helix();
		HepSymMatrix Ea = mdcTrk->err();
		HepPoint3D point0(0.,0.,0.);
		HepPoint3D IP(xorigin[0],xorigin[1],xorigin[2]);
		VFHelix helixip3(point0,a,Ea);
		helixip3.pivot(IP);
		HepVector  vecipa = helixip3.a();
		double dr=(vecipa[0]);
		double dz=(vecipa[3]);
		double costheta=cos(mdcTrk->theta());
		if (  fabs(dr)>= m_vr0cut) return false;
		if (  fabs(dz)>= m_vz0cut ) return false;
		if ( fabs(costheta) >= CosThetaCut ) return false;
		vz  = dz ;
		vxy = dr;

		return true;
}


//add your code here,for other member-functions
bool PKPiCheckAlg::IsPronton(EvtRecTrackIterator itTrk)
{

		//double m_prob_cut =0.001;
		if(!(*itTrk)->isMdcKalTrackValid()) return false;
		ParticleID *pid = ParticleID::instance();
		pid->init();
		pid->setMethod(pid->methodProbability());
		pid->setChiMinCut(4);
		pid->setRecTrack(*itTrk);
		if(m_use_Total_TOF)
		{pid->usePidSys(pid->useDedx() | pid->useTof1() | pid->useTof2()| pid->useTof());}
		else { pid->usePidSys(pid->useDedx() | pid->useTof1() | pid->useTof2());}
		pid->identify(pid->onlyProton()|pid->onlyPion()|pid->onlyKaon());
		pid->calculate();
		if(!(pid->IsPidInfoValid())) return false;

		if(pid->prob(4)<m_prob_cut) return  false;

		if( pid->prob(4)< pid->prob(3) || pid->prob(4)< pid->prob(2)) return false;
		return true;

}

bool PKPiCheckAlg::IsPion( EvtRecTrackIterator itTrk )
{
		//double m_prob_cut =0.001;
		if(!(*itTrk)->isMdcKalTrackValid()) return false;
		ParticleID *pid = ParticleID::instance();
		pid->init();
		pid->setMethod(pid->methodProbability());
		pid->setChiMinCut(4);
		pid->setRecTrack(*itTrk);
		if(m_use_Total_TOF)
		{pid->usePidSys(pid->useDedx() | pid->useTof1() | pid->useTof2()| pid->useTof());}
		else { pid->usePidSys(pid->useDedx() | pid->useTof1() | pid->useTof2());}
		pid->identify(pid->onlyPion()|pid->onlyKaon());
		pid->calculate();
		if(!(pid->IsPidInfoValid())) return false;
		//prob_k = pid->prob(3);
		//prob_pi=pid->prob(2);

		if(pid->prob(2)<m_prob_cut) return  false;

		//if( pid->prob(2)< pid->prob(3) || pid->prob(2)< pid->prob(4)) return false;
		if( pid->prob(2)< pid->prob(3)) return false;
		return true;

}
bool PKPiCheckAlg::IsKaon( EvtRecTrackIterator itTrk )
{
		//double m_prob_cut =0.001;
		if(!(*itTrk)->isMdcKalTrackValid()) return false;
		ParticleID *pid = ParticleID::instance();
		pid->init();
		pid->setMethod(pid->methodProbability());
		pid->setChiMinCut(4);
		pid->setRecTrack(*itTrk);
		if(m_use_Total_TOF)
		{pid->usePidSys(pid->useDedx() | pid->useTof1() | pid->useTof2()| pid->useTof());}
		else { pid->usePidSys(pid->useDedx() | pid->useTof1() | pid->useTof2());}
		pid->identify(pid->onlyPion()|pid->onlyKaon());
		pid->calculate();
		if(!(pid->IsPidInfoValid())) return false;

		if(pid->prob(3)<m_prob_cut) return  false;

		//if( pid->prob(3)< pid->prob(2) || pid->prob(3)< pid->prob(4)) return false;
		if( pid->prob(3)< pid->prob(2)) return false;
		return true;

}
