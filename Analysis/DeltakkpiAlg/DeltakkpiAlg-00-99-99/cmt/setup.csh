# echo "setup DeltakkpiAlg DeltakkpiAlg-00-99-99 in /data2/kc2019/workfs/7.0.3/Analysis"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /data/lzuhep/sw/Boss/CMT/v1r25
endif
source ${CMTROOT}/mgr/setup.csh
set cmtDeltakkpiAlgtempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtDeltakkpiAlgtempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=DeltakkpiAlg -version=DeltakkpiAlg-00-99-99 -path=/data2/kc2019/workfs/7.0.3/Analysis  -no_cleanup $* >${cmtDeltakkpiAlgtempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt setup -csh -pack=DeltakkpiAlg -version=DeltakkpiAlg-00-99-99 -path=/data2/kc2019/workfs/7.0.3/Analysis  -no_cleanup $* >${cmtDeltakkpiAlgtempfile}"
  set cmtsetupstatus=2
  /bin/rm -f ${cmtDeltakkpiAlgtempfile}
  unset cmtDeltakkpiAlgtempfile
  exit $cmtsetupstatus
endif
set cmtsetupstatus=0
source ${cmtDeltakkpiAlgtempfile}
if ( $status != 0 ) then
  set cmtsetupstatus=2
endif
/bin/rm -f ${cmtDeltakkpiAlgtempfile}
unset cmtDeltakkpiAlgtempfile
exit $cmtsetupstatus

