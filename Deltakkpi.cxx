//******* test of Reconstruction mctruth ****************
#include "DeltakkpiAlg/Deltakkpi.h"

#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/IJobOptionsSvc.h"
#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/AlgFactory.h"
#include "GaudiKernel/PropertyMgr.h"

#include "EvtRecEvent/EvtRecEvent.h"
#include "EvtRecEvent/EvtRecTrack.h"
#include "EvtRecEvent/EvtRecVeeVertex.h"
#include "EvtRecEvent/EvtRecPi0.h"
#include "McTruth/McParticle.h"

#include "VertexFit/IVertexDbSvc.h"
#include "VertexFit/KalmanKinematicFit.h"
#include "VertexFit/SecondVertexFit.h"
#include "VertexFit/VertexFit.h"

#include "DstEvent/TofHitStatus.h"

#include "EventModel/EventModel.h"
#include "EventModel/Event.h"
#include "EventModel/EventHeader.h"

#include "EvtRecEvent/EvtRecDTag.h"
#include "EvtRecEvent/EvtRecEtaToGG.h"
#include "MdcRecEvent/RecMdcKalTrack.h"

#include "TRandom.h"
#include "TMath.h"
#include "GaudiKernel/INTupleSvc.h"
#include "GaudiKernel/NTuple.h"
#include "GaudiKernel/IHistogramSvc.h"
#include "CLHEP/Vector/ThreeVector.h"
#include "CLHEP/Vector/LorentzVector.h"
#include "CLHEP/Vector/TwoVector.h"
#include "CLHEP/Geometry/Point3D.h"

#include "McDecayModeSvc/McDecayModeSvc.h"

#include "VertexFit/Helix.h"
#include "VertexFit/KinematicFit.h"
#include "ParticleID/ParticleID.h"
#include <vector>

using CLHEP::Hep2Vector;
using CLHEP::Hep3Vector;
using CLHEP::HepLorentzVector;
#ifndef ENABLE_BACKWARDS_COMPATIBILITY
typedef HepGeom::Point3D<double> HepPoint3D;
#endif

typedef std::vector<int> Vint;
typedef std::vector<double> Vdou;
typedef std::vector<HepLorentzVector> Vp4;

using namespace Event;

const double xmass[5] = {0.000511, 0.105658, 0.139570, 0.493677, 0.938272}; //0=electron 1=muon 2=pion 3=kaon 4=proton
const double mpi0 = 0.134976;
const double mpion = 0.13957;
const double mkaon = 0.493677;

//*********************** Algorithm ***************************************
Deltakkpi::Deltakkpi(const std::string &name, ISvcLocator *pSvcLocator) : Algorithm(name, pSvcLocator)
{
    m_ntot = 0;
    m_debug = 0;
    declareProperty("Decay_kkpi", decay_kkpi = 1);
    declareProperty("CheckTotal", m_checktotal = 1);
    declareProperty("HaveKaon", m_HaveKaon = 1);
    declareProperty("HavePion", m_HavePion = 1);
}

//********************** initialize ***************************************
StatusCode Deltakkpi::initialize()
{
    MsgStream log(msgSvc(), name());
    StatusCode scmgn = service("MagneticFieldSvc", m_pIMF);
    if (scmgn != StatusCode::SUCCESS)
    {
        log << MSG::ERROR << "Unable to open Magnetic field service" << endreq;
    }
    log << MSG::INFO << "in initialize()" << endreq;
    StatusCode status;

    if (decay_kkpi)
    {
        NTuplePtr nt10(ntupleSvc(), "FILE1/kkpi");
        if (nt10)
            m_nt10 = nt10;
        else
        {
            m_nt10 = ntupleSvc()->book("FILE1/kkpi", CLID_ColumnWiseTuple, "Ds ST Study");
        }
        if (m_nt10)
        {
            status = m_nt10->addItem("mDs_kkpi", m);

        }
        else
        {
            log << MSG::ERROR << "  Cannot book N-tuple10: " << long(m_nt10) << endreq;
            return StatusCode::FAILURE;
        }
    }

    //<<<<< MC Truth >>>>>
    /*
    if(m_checktotal){
        NTuplePtr nt20(ntupleSvc(), "FILE1/truth_info_og");
        if(nt20) m_nt20 = nt20;
        else{
            m_nt20 = ntupleSvc()->book("FILE1/truth_info_og", CLID_ColumnWiseTuple, "Ds ST Study");
        }
        if(m_nt20){
            status = m_nt20->addItem("total_og",   m_total_og);
            status = m_nt20->addItem("flag_ST",   m_flag_ST_og);
        }
        else{
            log << MSG::ERROR << "  Cannot book N-tuple20: " << long(m_nt20) << endreq;
            return StatusCode::FAILURE;
        }   
    }
    */

    log << MSG::INFO << "successfully return from initialize()" << endmsg;
    return StatusCode::SUCCESS;
}
//End of initialize

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
StatusCode Deltakkpi::execute()
{
    m_ntot = m_ntot + 1;
    MsgStream log(msgSvc(), name());
    log << MSG::INFO << "in execute()" << endreq;

    SmartDataPtr<Event::EventHeader> eventHeader(eventSvc(), "/Event/EventHeader");
    if (!eventHeader)
    {
        log << MSG::FATAL << "Could not find Event Header" << endreq;
        return (StatusCode::FAILURE);
    }

    int runNo = eventHeader->runNumber();
    int event = eventHeader->eventNumber();

    // ***** topology
    IMcDecayModeSvc *i_svc;
    StatusCode sc_DecayModeSvc = service("McDecayModeSvc", i_svc);
    if (sc_DecayModeSvc.isFailure())
    {
        log << MSG::FATAL << "Could not load McDecayModeSvc!" << endreq;
        return sc_DecayModeSvc;
    }
    m_svc = dynamic_cast<McDecayModeSvc *>(i_svc);

    //<<<<< MC Truth >>>>>
    /*
    // *** initialize flag ***
    int total = 0;
    int flag_ST = -999999;
    if(eventHeader->runNumber()<0){
        SmartDataPtr<Event::McParticleCol> mcParticleCol(eventSvc(), "/Event/MC/McParticleCol");
        if(!mcParticleCol){
            std::cout << "Could not retrieve McParticelCol" << std::endl;// return StatusCode::FAILURE;
        }
        else{
            Event::McParticleCol::iterator iter_mc = mcParticleCol->begin();

            // *** select |Ds*+-| *** distinguish Ds*+ or Ds*- ***
            for(; iter_mc != mcParticleCol->end(); iter_mc++){
                //if((*iter_mc)->primaryParticle()) continue;
                //if(!(*iter_mc)->decayFromGenerator()) continue;
                int pdg = (*iter_mc)->particleProperty();
                int motherpdg = ((*iter_mc)->mother()).particleProperty();
                int mmotherpdg = (((*iter_mc)->mother()).mother()).particleProperty();
                if(fabs(pdg) != 433) continue;
                if(pdg == 433){
                    flag_ST = 1;
                }
                if(pdg == -433){
                    flag_ST = -1;
                }
            }
    }//end of truth info
    if(m_checktotal){
        total = 1;
        m_total_og = total;
        // *** store process flag ***
        m_flag_ST_og   = flag_ST;
    }
    */

    // >>>>>>>>>>>>>>>>>>>>  Selection and Reconstruction begin from here <<<<<<<<<<<<<<<<<<<<<<
    SmartDataPtr<EvtRecEvent> evtRecEvent(eventSvc(), EventModel::EvtRec::EvtRecEvent);
    SmartDataPtr<EvtRecTrackCol> evtRecTrackCol(eventSvc(), EventModel::EvtRec::EvtRecTrackCol);

    Hep3Vector xorigin(0, 0, 0);
    IVertexDbSvc *vtxsvc;
    Gaudi::svcLocator()->service("VertexDbSvc", vtxsvc);
    if (vtxsvc->isVertexValid())
    {
        double *dbv = vtxsvc->PrimaryVertex();
        double *vv = vtxsvc->SigmaPrimaryVertex();
        xorigin.setX(dbv[0]);
        xorigin.setY(dbv[1]);
        xorigin.setZ(dbv[2]);
    }

    // *** select good Charged pi+- K+- *** //
    Vint iGood;
    iGood.clear();
    Vint iTrkm, iTrkp;
    iTrkm.clear();
    iTrkp.clear();

    Vint ipionm, ipionp, iprtonp;
    ipionm.clear();
    ipionp.clear();
    iprtonp.clear();
    Vint ikaonm, ikaonp, iprtonm;
    ikaonm.clear();
    ikaonp.clear();
    iprtonm.clear();
    Vp4 p4pionm, p4pionp, p4prtonp;
    p4pionm.clear();
    p4pionp.clear();
    p4prtonp.clear();
    Vp4 p4kaonm, p4kaonp, p4prtonm;
    p4kaonm.clear();
    p4kaonp.clear();
    p4prtonm.clear();

    int nCharge = 0;
    for (int i = 0; i < evtRecEvent->totalCharged(); i++)
    {
        EvtRecTrackIterator itTrk = evtRecTrackCol->begin() + i;
        RecMdcTrack *mdcTrk = (*itTrk)->mdcTrack();
        RecMdcKalTrack *mdcKalTrk = (*itTrk)->mdcKalTrack();

        //judgement of the charged-Track
        if (!isGoodTrack(*itTrk))
            continue;
        iGood.push_back(i);

        //Particle IDentify for each charged-Track
        preparePID(*itTrk);
        if (mdcTrk->charge() == 1)
        {
            iTrkp.push_back(i);
            if (m_HavePion && ispion())
            {
                ipionp.push_back(i);
                mdcKalTrk->setPidType(RecMdcKalTrack::pion);
                p4pionp.push_back(mdcKalTrk->p4(xmass[2]));
            }
            if (m_HaveKaon && iskaon())
            {
                ikaonp.push_back(i);
                mdcKalTrk->setPidType(RecMdcKalTrack::kaon);
                p4kaonp.push_back(mdcKalTrk->p4(xmass[3]));
            }
            if (isproton())
            {
                iprotonp.push_back(i);
                mdcKalTrk->setPidType(RecMdcKalTrack::proton);
                p4prtonp.push_back(mdcKalTrk->p4(xmass[4]));
            }
        }
        if (mdcTrk->charge() == -1)
        {
            iTrkm.push_back(i);
            if (m_HavePion && ispion())
            {
                ipionm.push_back(i);
                mdcKalTrk->setPidType(RecMdcKalTrack::pion);
                p4pionm.push_back(mdcKalTrk->p4(xmass[2]));
            }
            if (m_HaveKaon && iskaon())
            {
                ikaonm.push_back(i);
                mdcKalTrk->setPidType(RecMdcKalTrack::kaon);
                p4kaonm.push_back(mdcKalTrk->p4(xmass[3]));
            }
            if (isproton())
            {
                iprotonm.push_back(i);
                mdcKalTrk->setPidType(RecMdcKalTrack::proton);
                p4prtonm.push_back(mdcKalTrk->p4(xmass[4]));
            }
        }
        nCharge += mdcTrk->charge();
    }
    // Finish Good Charged Track Selection
    int nGood = iGood.size();

    int nTrkp = iTrkp.size();
    int npionp = ipionp.size();
    int nkaonp = ikaonp.size();
    int nprotonp = iprtonp.size();

    int nTrkm = iTrkm.size();
    int npionm = ipionm.size();
    int nkaonm = ikaonm.size();
    int nprotonm = iprtonm.size();

    // *** select good shower *** //
    Vint iGam;
    iGam.clear();
    Vp4 p4Gam;
    p4Gam.clear();
    for (int i = evtRecEvent->totalCharged(); i < evtRecEvent->totalTracks(); i++)
    {
        EvtRecTrackIterator itTrk = evtRecTrackCol->begin() + i;
        if (!isGoodShower(*itTrk))
            continue;
        iGam.push_back(i);
        RecEmcShower *emcTrk = (*itTrk)->emcShower();
        p4Gam.push_back(getP4(emcTrk, xorigin));
    }
    int nGam = iGam.size();
    if (nGam < 1)
        return StatusCode::SUCCESS;

    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    double ECMS = 4.6000;
    //boost pars
    m_beta.setX(0.0110004);
    m_beta.setY(0);
    m_beta.setZ(0);

    // *** for kkpi *** //
    if (decay_kkpi)
    {   
        int passFlagLambdacm=0;     //是否有有效的lambda-事例
        if(!(nprotonm==0||nkaonp==0||npionm==0)){ //要有反质子，K+与pi-存在，才进行基于其的重建
            double delta_E;//lambdac-能量与平均能量只差，越小越好，而可选范围见文章
            double mLcm=0;
            for (int npm=0;npm<iprtonm;npm++){
                for (int nkp=0;nkp<ikaonp;nkp++){
                    for (int npim=0;npim<ipionm;npim++){
                        HepLorentzVector p4Lcm=p4prtonm[npm]+p4kaonp[nkp]+p4pionm[npim];
                        mLcm=p4Lcm.m();
                    }
                }
            }

        }
        m_nt10->write();
        // int pass_flag_kkpi = 0;
        // if (nkaonm == 0 || nkaonp == 0 || npionp == 0)
        //     return StatusCode::SUCCESS;
        // // *** make Ds ***
        // int index_k1_kkpi(0);
        // int index_k2_kkpi(0);
        // int index_j_kkpi(0);
        // int index_i_kkpi(0);
        // double delta_E_temp = -999999.;
        // double mDs_kkpi = -999999.;
        // double mDsgam_kkpi = -999999.;
        // double mrec_Ds_kkpi = -999999.;
        // double eDs_kkpi = -999999.;
        // HepLorentzVector P_Ds_Lab, P_DsST_Lab, P_Gam_CMS;
        // HepLorentzVector p4Ds_kkpi, p4DsST_kkpi;
        // for (int k1 = 0; k1 < nkaonm; k1++)
        // {
        //     for (int k2 = 0; k2 < nkaonp; k2++)
        //     {
        //         for (int j = 0; j < npionp; j++)
        //         {
        //             for (int i = 0; i < nGam; i++)
        //             {
        //                 if (ipionp[j] == ikaonm[k1=] || ipionp[j] = ikaonp[k2])
        //                     continue;
        //                 if (ikaonm[k1] == ikaonp[k2])
        //                     continue;
        //                 HepLorentzVector p4Ds = p4kaonm[k1] + p4kaonp[k2] + p4pionp[j];
        //                 HepLorentzVector p4DsST = p4Ds + p4Gam[i];
        //                 P_Ds_Lab = p4Ds;
        //                 P_DsST_Lab = p4DsST;
        //                 P_Gam_CMS = p4Gam[i];
        //                 P_Gam_CMS.boost(-m_beta); //change beam
        //                 p4Ds.boost(-m_beta);
        //                 p4DsST.boost(-m_beta);
        //                 double rec_eDsgam = sqrt(p4DsST.rho() * p4DsST.rho() + 1.9683 * 1.9683);
        //                 double eDs = p4Ds.e();
        //                 double eGam = P_Gam_CMS.e();
        //                 double delta_E = ECMS - (eDs + eGam + rec_eDsgam);
        //                 double emiss = ECMS - sqrt(p4Ds.rho() * p4Ds.rho() + 1.9683 * 1.9683);
        //                 double mrec_Ds = sqrt(emiss * emiss - p4Ds.rho() * p4Ds.rho());
        //                 if (fabs(delta_E) < fabs(delta_E_temp))
        //                 {
        //                     pass_flag_kkpi++;
        //                     delta_E_temp = delta_E;
        //                     mDs_kkpi = p4Ds.m();
        //                     mDsgam_kkpi = p4DsST.m();
        //                     mrec_Ds_kkpi = mrec_Ds;
        //                     eDs_kkpi = eDs;
        //                     p4Ds_kkpi = P_Ds_Lab;
        //                     p4DsST_kkpi = P_DsST_Lab;
        //                     index_i_kkpi = i;   //gam
        //                     index_j_kkpi = j;   //pion
        //                     index_k1_kkpi = k1; //kaonm
        //                     index_k2_kkpi = k2; //kaonp
        //                 }
        //             }
        //         }
        //     }
        // }
        // if (pass_flag_kkpi > 0)
        // {

        //     //store the variables
        //     m_runNo_kkpi = runNo;
        //     m_event_kkpi = event;
        //     m_ngam_kkpi = nGam;
        //     m_nchg_kkpi = nGood;
        //     m_charge_kkpi = nCharge;

        //     m_mDs_kkpi = mDs_kkpi;
        //     m_mDsgam_kkpi = mDsgam_kkpi;
        //     m_mrec_Ds_kkpi = mrec_Ds_kkpi;
        //     m_eDs_kkpi = eDs_kkpi;
        //     m_delta_E_kkpi = delta_E_temp;

        //     for (int ll = 0; ll < 4; ll++)
        //     {
        //         m_p4_DsST_kkpi[ll] = p4DsST_kkpi[ll];
        //         m_p4_Ds_kkpi[ll] = p4Ds_kkpi[ll];
        //         m_p4_pion_kkpi[ll] = p4pionp[index_j_kkpi][ll];
        //         m_p4_kaonm_kkpi[ll] = p4kaonm[index_k1_kkpi][ll];
        //         m_p4_kaonp_kkpi[ll] = p4kaonp[index_k2_kkpi][ll];
        //         m_p4_gam_kkpi[ll] = p4Gam[index_i_kkpi][ll];
        //     }

        //     m_pass_flag_kkpi = pass_flag_kkpi;
        //     m_nt10->write(); //write into root
        // }
    } //end decay_kkpi

    return StatusCode::SUCCESS;
}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

StatusCode Deltakkpi::finalize()
{
    MsgStream log(msgSvc(), name());
    log << MSG::INFO << "in finalize()" << endreq;
    return StatusCode::SUCCESS;
}

// * * * * * * * * * * * * * * * * * * * Sub Program * * * * * * * * * * * * * * * * * * * //
HepLorentzVector Deltakkpi::getP4(RecEmcShower *gTrk, Hep3Vector origin)
{
    Hep3Vector Gm_Vec(gTrk->x(), gTrk->y(), gTrk->z());
    Hep3Vector Gm_Mom = Gm_Vec - origin;
    Gm_Mom.setMag(gTrk->energy());
    HepLorentzVector pGm(Gm_Mom, gTrk->energy());
    return pGm;
}

bool Deltakkpi::isGoodTrackForKsLambda(EvtRecTrack *trk)
{
    double m_CosThetaCut = 0.93;
    if (!trk->isMdcKalTrackValid())
    {
        return false;
    }

    Hep3Vector xorigin(0, 0, 0);
    IVertexDbSvc *vtxsvc;
    Gaudi::svcLocator()->service("VertexDbSvc", vtxsvc);
    if (vtxsvc->isVertexValid())
    {
        double *dbv = vtxsvc->PrimaryVertex();
        double *vv = vtxsvc->SigmaPrimaryVertex();
        xorigin.setX(dbv[0]);
        xorigin.setY(dbv[1]);
        xorigin.setZ(dbv[2]);
    }

    RecMdcTrack *mdcTrk = trk->mdcTrack();

    HepVector a = mdcTrk->helix();
    HepSymMatrix Ea = mdcTrk->err();

    HepPoint3D point0(0., 0., 0.); // the initial point for MDC recosntruction
    HepPoint3D IP(xorigin[0], xorigin[1], xorigin[2]);
    VFHelix helixip(point0, a, Ea);
    helixip.pivot(IP);
    HepVector vecipa = helixip.a();
    double Rvxy0 = fabs(vecipa[0]); //the nearest distance to IP in xy plane
    double Rvz0 = vecipa[3];        //the nearest distance to IP in z direction
    double costheta = cos(mdcTrk->theta());

    if (fabs(Rvz0) < 20 && fabs(costheta) < m_CosThetaCut)
        return true;

    return false;
}

bool Deltakkpi::isGoodTrack(EvtRecTrack *trk)
{
    double m_vz0cut = 10.0;
    double m_vr0cut = 1.0;
    double m_CosThetaCut = 0.93;

    if (!trk->isMdcKalTrackValid())
    {
        return false;
    }
    Hep3Vector xorigin(0, 0, 0);
    IVertexDbSvc *vtxsvc;
    Gaudi::svcLocator()->service("VertexDbSvc", vtxsvc);
    if (vtxsvc->isVertexValid())
    {
        double *dbv = vtxsvc->PrimaryVertex();
        double *vv = vtxsvc->SigmaPrimaryVertex();
        xorigin.setX(dbv[0]);
        xorigin.setY(dbv[1]);
        xorigin.setZ(dbv[2]);
    }
    RecMdcTrack *mdcTrk = trk->mdcTrack();
    HepVector a = mdcTrk->helix();
    HepSymMatrix Ea = mdcTrk->err();
    HepPoint3D point0(0., 0., 0.); // the initial point for MDC recosntruction
    HepPoint3D IP(xorigin[0], xorigin[1], xorigin[2]);
    VFHelix helixip(point0, a, Ea);
    helixip.pivot(IP);
    HepVector vecipa = helixip.a();
    double Rvxy0 = fabs(vecipa[0]); //the nearest distance to IP in xy plane
    double Rvz0 = vecipa[3];        //the nearest distance to IP in z direction
    double costheta = cos(mdcTrk->theta());
    if (fabs(Rvz0) < m_vz0cut && fabs(Rvxy0) < m_vr0cut && fabs(costheta) < m_CosThetaCut)
        return true;
    return false;
}

void Deltakkpi::preparePID(EvtRecTrack *track)
{
    ////PID for proton
    //ParticleID *pid = ParticleID::instance();
    //pid->init();
    //pid->setMethod(pid->methodProbability());
    //pid->setChiMinCut(8);
    //pid->setRecTrack(track);
    //pid->usePidSys(pid->useDedx() | pid->useTof1() | pid->useTof2() | pid->useTof()); // use PID sub-system
    ////pid->identify(pid->onlyPion() | pid->onlyKaon());
    //pid->identify(pid->onlyPion() | pid->onlyKaon() | pid->onlyProton());//change
    //pid->calculate();
    //m_prob[0]=pid->probElectron();
    //m_prob[1]=pid->probMuon();
    //m_prob[2]=pid->probPion();
    //m_prob[3]=pid->probKaon();
    //m_prob[4]=pid->probProton();
    //if(!(pid->IsPidInfoValid())){
    //    for(int i=0; i<5; i++) m_prob[i]=-99;
    //}

    //PID for kaon and pion
    ParticleID *PID = ParticleID::instance();
    PID->init();
    PID->setMethod(PID->methodProbability());
    PID->setChiMinCut(4);
    PID->setRecTrack(track);
    PID->usePidSys(PID->useDedx() | PID->useTof1() | PID->useTof2() | PID->useTof() | pid->useEmc()); // use PID sub-system
    PID->identify(PID->onlyPion() | PID->onlyKaon() | PID->onlyProton() | pid->onlyElectron());
    //PID->identify(PID->onlyPion() | PID->onlyKaon() | PID->onlyProton());
    PID->calculate();
    mm_prob[0] = PID->probElectron();
    mm_prob[1] = PID->probMuon();
    mm_prob[2] = PID->probPion();
    mm_prob[3] = PID->probKaon();
    mm_prob[4] = PID->probProton();
    if (!(PID->IsPidInfoValid()))
    {
        for (int i = 0; i < 5; i++)
            mm_prob[i] = -99;
    }

    ////PID for electron
    //ParticleID *Epid = ParticleID::instance();
    //Epid->init();
    //Epid->setMethod(Epid->methodProbability());
    //Epid->setChiMinCut(4);
    //Epid->setRecTrack(track);
    //Epid->usePidSys(Epid->useDedx() | Epid->useTof1() | Epid->useTof2() | Epid->useTof() | Epid->useEmc()); // use PID sub-system
    //Epid->identify(Epid->onlyElectron() | Epid->onlyPion() | Epid->onlyKaon());
    //Epid->calculate();
    //mmm_prob[0]=Epid->probElectron();
    //mmm_prob[1]=Epid->probMuon();
    //mmm_prob[2]=Epid->probPion();
    //mmm_prob[3]=Epid->probKaon();
    //mmm_prob[4]=Epid->probProton();
    //if(!(Epid->IsPidInfoValid())){
    //    for(int i=0; i<5; i++) mmm_prob[i]=-99;
    //}

    //RecMdcTrack *mdcTrk = track->mdcTrack();
    //m_eop=-1;
    //if(track->isEmcShowerValid()){
    //    RecEmcShower *emcTrk = track->emcShower();
    //    m_eop = (emcTrk->energy())/(mdcTrk->p());
    //}
} //end for preparePID

bool Deltakkpi::iselectron()
{
    if (mm_prob[0] > 0.001 && (mm_prob[0] / (mm_prob[0] + mm_prob[1] + mm_prob[2] + mm_prob[3])) > 0.8)
    {
        return true;
    }
    return false;
}

bool Deltakkpi::iselectron_eop()
{
    if (m_eop > 0.8)
        return true;
    return false;
}

bool Deltakkpi::ispion()
{
    if (mm_prob[2] >= 0.00 && mm_prob[2] >= mm_prob[0] && mm_prob[2] >= mm_prob[3] && mm_prob[2] >= mm_prob[4])
        return true;
    return false;
}

bool Deltakkpi::iskaon()
{
    if (mm_prob[3] >= 0.00 && mm_prob[3] >= mm_prob[0] && mm_prob[3] >= mm_prob[2] && mm_prob[3] >= mm_prob[4])
    {
        return true;
    }
    return false;
}

bool Deltakkpi::isproton()
{
    return mm_prob[4] >= 0.00 && mm_prob[4] >= mm_prob[0] && mm_prob[4] >= mm_prob[2] && mm_prob[4] >= mm_prob[3];
}

bool Deltakkpi::isGoodShower(EvtRecTrack *trk)
{
    double m_maxCosThetaBarrel = 0.80;
    double m_minCosThetaEndcap = 0.86;
    double m_maxCosThetaEndcap = 0.92;
    double m_minBarrelEnergy = 0.025;
    double m_minEndcapEnergy = 0.050;
    double m_gammthCut = 10.0;

    if (!trk->isEmcShowerValid())
    {
        return false;
    }
    Hep3Vector xorigin(0, 0, 0);
    IVertexDbSvc *vtxsvc;
    Gaudi::svcLocator()->service("VertexDbSvc", vtxsvc);
    if (vtxsvc->isVertexValid())
    {
        double *dbv = vtxsvc->PrimaryVertex();
        double *vv = vtxsvc->SigmaPrimaryVertex();
        xorigin.setX(dbv[0]);
        xorigin.setY(dbv[1]);
        xorigin.setZ(dbv[2]);
    }
    SmartDataPtr<EvtRecEvent> evtRecEvent(eventSvc(), EventModel::EvtRec::EvtRecEvent);
    SmartDataPtr<EvtRecTrackCol> evtRecTrackCol(eventSvc(), EventModel::EvtRec::EvtRecTrackCol);
    RecEmcShower *emcTrk = trk->emcShower();
    Hep3Vector emcpos(emcTrk->x(), emcTrk->y(), emcTrk->z());
    HepLorentzVector shP4 = getP4(emcTrk, xorigin);
    double cosThetaSh = shP4.vect().cosTheta();
    double eraw = emcTrk->energy();
    double getTime = emcTrk->time();
    if (getTime > 14 || getTime < 0)
        return false;
    if (!((fabs(cosThetaSh) < m_maxCosThetaBarrel && eraw > m_minBarrelEnergy) || ((fabs(cosThetaSh) > m_minCosThetaEndcap) && (fabs(cosThetaSh) < m_maxCosThetaEndcap) && (eraw > m_minEndcapEnergy))))
        return false;
    double dang = 200.;
    for (int j = 0; j < evtRecEvent->totalCharged(); j++)
    {
        EvtRecTrackIterator jtTrk = evtRecTrackCol->begin() + j;
        if (!(*jtTrk)->isExtTrackValid())
            continue;
        RecExtTrack *extTrk = (*jtTrk)->extTrack();
        if (extTrk->emcVolumeNumber() == -1)
            continue;
        Hep3Vector extpos = extTrk->emcPosition();
        double angd = extpos.angle(emcpos);
        if (angd < dang)
            dang = angd;
    }
    if (dang >= 200)
        return false;
    dang = dang * 180 / (CLHEP::pi);
    if (fabs(dang) < m_gammthCut)
        return false;
    return true;
}

//******************************************************************
