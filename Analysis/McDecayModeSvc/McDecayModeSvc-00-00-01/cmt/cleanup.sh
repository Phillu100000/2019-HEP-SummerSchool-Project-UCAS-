# echo "cleanup McDecayModeSvc McDecayModeSvc-00-00-01 in /data2/kc2019/workfs/7.0.3/Analysis"

if test "${CMTROOT}" = ""; then
  CMTROOT=/data/lzuhep/sw/Boss/CMT/v1r25; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtMcDecayModeSvctempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtMcDecayModeSvctempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt cleanup -sh -pack=McDecayModeSvc -version=McDecayModeSvc-00-00-01 -path=/data2/kc2019/workfs/7.0.3/Analysis  $* >${cmtMcDecayModeSvctempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt cleanup -sh -pack=McDecayModeSvc -version=McDecayModeSvc-00-00-01 -path=/data2/kc2019/workfs/7.0.3/Analysis  $* >${cmtMcDecayModeSvctempfile}"
  cmtcleanupstatus=2
  /bin/rm -f ${cmtMcDecayModeSvctempfile}
  unset cmtMcDecayModeSvctempfile
  return $cmtcleanupstatus
fi
cmtcleanupstatus=0
. ${cmtMcDecayModeSvctempfile}
if test $? != 0 ; then
  cmtcleanupstatus=2
fi
/bin/rm -f ${cmtMcDecayModeSvctempfile}
unset cmtMcDecayModeSvctempfile
return $cmtcleanupstatus

