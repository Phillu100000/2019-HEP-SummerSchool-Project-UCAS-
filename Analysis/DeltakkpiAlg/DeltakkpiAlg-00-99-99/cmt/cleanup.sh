# echo "cleanup DeltakkpiAlg DeltakkpiAlg-00-99-99 in /data2/kc2019/workfs/7.0.3/Analysis"

if test "${CMTROOT}" = ""; then
  CMTROOT=/data/lzuhep/sw/Boss/CMT/v1r25; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtDeltakkpiAlgtempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtDeltakkpiAlgtempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt cleanup -sh -pack=DeltakkpiAlg -version=DeltakkpiAlg-00-99-99 -path=/data2/kc2019/workfs/7.0.3/Analysis  $* >${cmtDeltakkpiAlgtempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt cleanup -sh -pack=DeltakkpiAlg -version=DeltakkpiAlg-00-99-99 -path=/data2/kc2019/workfs/7.0.3/Analysis  $* >${cmtDeltakkpiAlgtempfile}"
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

