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
        NTuplePtr ntb(ntupleSvc(), "FILE1/kppi");
        NTuplePtr nta(ntupleSvc(), "FILE1/pk");
        NTuplePtr count_E(ntupleSvc(), "FILE1/count");
        if (ntb)
            m_ntb = ntb;
        else
        {
            m_ntb = ntupleSvc()->book("FILE1/kppi", CLID_ColumnWiseTuple, "DST Study");
            m_ntb->addItem("mass", m_nchg_kkpi);
        }
        if (count_E)
            m_count_E = count_E;
        else
        {
            m_count_E = ntupleSvc()->book("FILE1/count", CLID_ColumnWiseTuple, "DST Study");
            m_count_E->addItem("countP", countP);
            m_count_E->addItem("countM", countM);
        }
        if (nta)
            m_nta = nta;
        else
        {
            m_nta = ntupleSvc()->book("FILE1/pk", CLID_ColumnWiseTuple, "DST Study");
            m_nta->addItem("mass", m_mDsgam_kkpi);
        }
        if (nt10)
            m_nt10 = nt10;
        else
        {
            m_nt10 = ntupleSvc()->book("FILE1/kkpi", CLID_ColumnWiseTuple, "Ds ST Study");
        }
        if (m_nt10)
        {
            status = m_nt10->addItem("mLc", m);
            status = m_nt10->addItem("deltaE", mDeltaE);
            status = m_nt10->addItem("ma", ma);
            //status = m_nt10->addItem("ma", mb);
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

    Vint ipionm, ipionp, iprotonp;
    ipionm.clear();
    ipionp.clear();
    iprotonp.clear();
    Vint ikaonm, ikaonp, iprotonm;
    ikaonm.clear();
    ikaonp.clear();
    iprotonm.clear();
    Vp4 p4pionm, p4pionp, p4protonp;
    p4pionm.clear();
    p4pionp.clear();
    p4protonp.clear();
    Vp4 p4kaonm, p4kaonp, p4protonm;
    p4kaonm.clear();
    p4kaonp.clear();
    p4protonm.clear();

    int iep = 0, iem = 0;
    int nCharge = 0;
    for (int i = 0; i < evtRecEvent->totalCharged(); i++)
    {
        EvtRecTrackIterator itTrk = evtRecTrackCol->begin() + i;
        RecMdcTrack *mdcTrk = (*itTrk)->mdcTrack();
        RecMdcKalTrack *mdcKalTrk = (*itTrk)->mdcKalTrack();

        preparePID(*itTrk);
        if (isGoodTrackForKsLambda(*itTrk))
        {
            if (mdcTrk->charge() == 1)
            {
                iTrkp.push_back(i);
                if (m_HavePion && ispion())
                {
                    ipionp.push_back(i);
                    mdcKalTrk->setPidType(RecMdcKalTrack::pion);
                    p4pionp.push_back(mdcKalTrk->p4(xmass[2]));
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
            }
        }
        if (isGoodTrack(*itTrk))
        {
            iGood.push_back(i);

            if (mdcTrk->charge() == 1)
            {
                iTrkp.push_back(i);
                // if (m_HavePion && ispion())
                // {
                //     ipionp.push_back(i);
                //     mdcKalTrk->setPidType(RecMdcKalTrack::pion);
                //     p4pionp.push_back(mdcKalTrk->p4(xmass[2]));
                // }
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
                    p4protonp.push_back(mdcKalTrk->p4(xmass[4]));
                }
            }
            if (mdcTrk->charge() == -1)
            {
                iTrkm.push_back(i);
                // if (m_HavePion && ispion())
                // {
                //     ipionm.push_back(i);
                //     mdcKalTrk->setPidType(RecMdcKalTrack::pion);
                //     p4pionm.push_back(mdcKalTrk->p4(xmass[2]));
                // }
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
                    p4protonm.push_back(mdcKalTrk->p4(xmass[4]));
                }
            }
            nCharge += mdcTrk->charge();
        }
        if (isGoodTrackForE(*itTrk))
        {
            if ((mdcKalTrk->p4(xmass[4])).e() > 0.2)
            {
                if (mdcTrk->charge() == 1)
                {
                    if (iselectron())
                    {
                        iep++;
                    }
                }
                if (mdcTrk->charge() == -1)
                {
                    if (iselectron())
                    {
                        iem++;
                    }
                }
            }
        }
    }
    // Finish Good Charged Track Selection
    int nGood = iGood.size();

    int nTrkp = iTrkp.size();
    int npionp = ipionp.size();
    int nkaonp = ikaonp.size();
    int nprotonp = iprotonp.size();

    int nTrkm = iTrkm.size();
    int npionm = ipionm.size();
    int nkaonm = ikaonm.size();
    int nprotonm = iprotonm.size();

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

    //>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    double ECMS = 4.6000;
    int flagL = 0;
    // *** for kkpi *** //
    if (decay_kkpi)
    {
        int passFlagLambdacm = 0; //是否有有效的lambda-事例
        if ((!(nprotonm == 0 || nkaonp == 0 || npionm == 0)))
        { //要有反质子，K+与pi-存在，才进行基于其的重建
            double delta_E = 9999;
            double mLcm = 0, mm = 0;
            ;
            for (int npm = 0; npm < nprotonm; npm++)
            {
                for (int nkp = 0; nkp < nkaonp; nkp++)
                {
                    for (int npim = 0; npim < npionm; npim++)
                    {
                        HepLorentzVector p4Lcm = p4protonm[npm] + p4kaonp[nkp] + p4pionm[npim];
                        double E = p4Lcm.e();
                        m = p4Lcm.m();
                        if (abs(m) < 0.1)
                            continue;
                        ma = mm;
                        m = p4Lcm.m();
                        delta_E = E - 2.3;
                        mDeltaE = delta_E;
                        m_nt10->write();
                        m_nchg_kkpi = p4Lcm.m();
                        m_ntb->write();
                        if ((E - ECMS / 2) > -0.021 && (E - ECMS / 2) < 0.019)
                        {
                            flagL = 1;
                        }
                    }
                }
            }
        }

        if ((!(nprotonm == 0 || npionm == 0 || npionp == 0)))
        {
            double delta_E = 9999; //lambdac-能量与平均能量只差，越小越好，而可选范围见文章
            double mLcm = 0, mm = 0;
            for (int npm = 0; npm < nprotonm; npm++)
            {
                for (int npip = 0; npip < npionp; npip++)
                {
                    for (int npim = 0; npim < npionm; npim++)
                    {
                        HepLorentzVector p4ks = p4pionp[npip] + p4pionm[npim];
                        if (((m_nchg_kkpi = p4ks.m()) > 0.487) && (m_nchg_kkpi < 0.511))
                        {
                            HepLorentzVector p4Lcm = p4protonm[npm] + p4pionp[npip] + p4pionm[npim];
                            double E = p4Lcm.e();
                            mLcm = p4Lcm.m();
                            if (abs(mLcm) < 0.1)
                                continue;
                            m = p4Lcm.m();
                            mDeltaE = E - 2.3;
                            m_nt10->write();
                            m_mDsgam_kkpi = p4Lcm.m();
                            m_nta->write();
                            if ((E - ECMS / 2) > -0.021 && (E - ECMS / 2) < 0.019)
                            {
                                flagL = 1;
                            }
                        }
                        //m_ntb->write();
                    }
                }
            }
        }
        if (flagL)
        {
            if (iep - iem > 0)
            {
                countP = 1;
            }
            countM = iep - iem;
            m_count_E->write();
        }
    }

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

    ParticleID *PID = ParticleID::instance();
    PID->init();
    PID->setMethod(PID->methodProbability());
    PID->setChiMinCut(4);
    PID->setRecTrack(track);
    PID->usePidSys(PID->useDedx() | PID->useTof1() | PID->useTof2() | PID->useTof() | PID->useEmc()); // use PID sub-system
    PID->identify(PID->onlyPion() | PID->onlyKaon() | PID->onlyProton() | PID->onlyElectron());
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
    if (mm_prob[2] >= 0.001 && mm_prob[2] >= mm_prob[0] && mm_prob[2] >= mm_prob[3] && mm_prob[2] >= mm_prob[4])
        return true;
    return false;
}

bool Deltakkpi::iskaon()
{
    if (mm_prob[3] >= 0.001 && mm_prob[3] >= mm_prob[0] && mm_prob[3] >= mm_prob[2] && mm_prob[3] >= mm_prob[4])
    {
        return true;
    }
    return false;
}

bool Deltakkpi::isproton()
{
    return mm_prob[4] >= 0.001 && mm_prob[4] >= mm_prob[0] && mm_prob[4] >= mm_prob[2] && mm_prob[4] >= mm_prob[3];
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
bool Deltakkpi::isGoodTrackForE(EvtRecTrack *trk)
{
    double m_vz0cut = 10.0;
    double m_vr0cut = 1.0;
    double m_CosThetaCut = 0.8;

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
