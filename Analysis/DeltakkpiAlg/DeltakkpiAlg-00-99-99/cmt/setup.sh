# echo "setup DeltakkpiAlg DeltakkpiAlg-00-99-99 in /data2/kc2019/workfs/7.0.3/Analysis"

if test "${CMTROOT}" = ""; then
  CMTROOT=/data/lzuhep/sw/Boss/CMT/v1r25; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtDeltakkpiAlgtempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtDeltakkpiAlgtempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=DeltakkpiAlg -version=DeltakkpiAlg-00-99-99 -path=/data2/kc2019/workfs/7.0.3/Analysis  -no_cleanup $* >${cmtDeltakkpiAlgtempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt setup -sh -pack=DeltakkpiAlg -version=DeltakkpiAlg-00-99-99 -path=/data2/kc2019/workfs/7.0.3/Analysis  -no_cleanup $* >${cmtDeltakkpiAlgtempfile}"
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

