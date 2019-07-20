# echo "cleanup McDecayModeSvc McDecayModeSvc-00-00-01 in /data2/kc2019/workfs/7.0.3/Analysis"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /data/lzuhep/sw/Boss/CMT/v1r25
endif
source ${CMTROOT}/mgr/setup.csh
set cmtMcDecayModeSvctempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtMcDecayModeSvctempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt cleanup -csh -pack=McDecayModeSvc -version=McDecayModeSvc-00-00-01 -path=/data2/kc2019/workfs/7.0.3/Analysis  $* >${cmtMcDecayModeSvctempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt cleanup -csh -pack=McDecayModeSvc -version=McDecayModeSvc-00-00-01 -path=/data2/kc2019/workfs/7.0.3/Analysis  $* >${cmtMcDecayModeSvctempfile}"
  set cmtcleanupstatus=2
  /bin/rm -f ${cmtMcDecayModeSvctempfile}
  unset cmtMcDecayModeSvctempfile
  exit $cmtcleanupstatus
endif
set cmtcleanupstatus=0
source ${cmtMcDecayModeSvctempfile}
if ( $status != 0 ) then
  set cmtcleanupstatus=2
endif
/bin/rm -f ${cmtMcDecayModeSvctempfile}
unset cmtMcDecayModeSvctempfile
exit $cmtcleanupstatus

