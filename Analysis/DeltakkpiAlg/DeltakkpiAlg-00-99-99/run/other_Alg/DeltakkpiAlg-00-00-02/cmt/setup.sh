# echo "setup DeltakkpiAlg DeltakkpiAlg-00-00-02 in /workfs/bes/wangziyi/7.0.3/Analysis/DsPP/DsST"

if test "${CMTROOT}" = ""; then
  CMTROOT=/afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtDeltakkpiAlgtempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtDeltakkpiAlgtempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=DeltakkpiAlg -version=DeltakkpiAlg-00-00-02 -path=/workfs/bes/wangziyi/7.0.3/Analysis/DsPP/DsST  -no_cleanup $* >${cmtDeltakkpiAlgtempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt setup -sh -pack=DeltakkpiAlg -version=DeltakkpiAlg-00-00-02 -path=/workfs/bes/wangziyi/7.0.3/Analysis/DsPP/DsST  -no_cleanup $* >${cmtDeltakkpiAlgtempfile}"
  cmtsetupstatus=2
  /bin/rm -f ${cmtDeltakkpiAlgtempfile}
  unset cmtDeltakkpiAlgtempfile
  return $cmtsetupstatus
fi
cmtsetupstatus=0
. ${cmtDeltakkpiAlgtempfile}
if test $? != 0 ; then
  cmtsetupstatus=2
fi
/bin/rm -f ${cmtDeltakkpiAlgtempfile}
unset cmtDeltakkpiAlgtempfile
return $cmtsetupstatus

