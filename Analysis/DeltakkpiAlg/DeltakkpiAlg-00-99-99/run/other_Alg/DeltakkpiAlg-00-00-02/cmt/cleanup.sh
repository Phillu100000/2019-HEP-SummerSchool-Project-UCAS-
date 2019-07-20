# echo "cleanup DeltakkpiAlg DeltakkpiAlg-00-00-02 in /workfs/bes/wangziyi/7.0.3/Analysis/DsPP/DsST"

if test "${CMTROOT}" = ""; then
  CMTROOT=/afs/ihep.ac.cn/bes3/offline/ExternalLib/SLC6/contrib/CMT/v1r25; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtDeltakkpiAlgtempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtDeltakkpiAlgtempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt cleanup -sh -pack=DeltakkpiAlg -version=DeltakkpiAlg-00-00-02 -path=/workfs/bes/wangziyi/7.0.3/Analysis/DsPP/DsST  $* >${cmtDeltakkpiAlgtempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt cleanup -sh -pack=DeltakkpiAlg -version=DeltakkpiAlg-00-00-02 -path=/workfs/bes/wangziyi/7.0.3/Analysis/DsPP/DsST  $* >${cmtDeltakkpiAlgtempfile}"
  cmtcleanupstatus=2
  /bin/rm -f ${cmtDeltakkpiAlgtempfile}
  unset cmtDeltakkpiAlgtempfile
  return $cmtcleanupstatus
fi
cmtcleanupstatus=0
. ${cmtDeltakkpiAlgtempfile}
if test $? != 0 ; then
  cmtcleanupstatus=2
fi
/bin/rm -f ${cmtDeltakkpiAlgtempfile}
unset cmtDeltakkpiAlgtempfile
return $cmtcleanupstatus

