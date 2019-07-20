if test "${CMTROOT}" = ""; then
  CMTROOT=/afs/ihep.ac.cn/bes3/offline/ExternalLib/contrib/CMT/v1r20p20081118; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then tempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt cleanup -sh -pack=LprFastAlg -version=LprFastAlg-00-00-00 -path=/workfs/bes/lipr/6.6.4.p01/Analysis $* >${tempfile}; . ${tempfile}
/bin/rm -f ${tempfile}

