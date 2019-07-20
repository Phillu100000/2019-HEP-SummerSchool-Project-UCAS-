# echo "Setting LprFastAlg LprFastAlg-00-00-00 in /workfs/bes/lipr/6.6.4.p01/Analysis"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /afs/ihep.ac.cn/bes3/offline/ExternalLib/contrib/CMT/v1r20p20081118
endif
source ${CMTROOT}/mgr/setup.csh

set tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set tempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=LprFastAlg -version=LprFastAlg-00-00-00 -path=/workfs/bes/lipr/6.6.4.p01/Analysis  -no_cleanup $* >${tempfile}; source ${tempfile}
/bin/rm -f ${tempfile}

