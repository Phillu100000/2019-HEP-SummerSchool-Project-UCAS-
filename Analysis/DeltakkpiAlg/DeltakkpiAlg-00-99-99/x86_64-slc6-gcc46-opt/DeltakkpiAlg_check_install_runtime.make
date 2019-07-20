#-- start of make_header -----------------

#====================================
#  Document DeltakkpiAlg_check_install_runtime
#
#   Generated Fri Jul 19 12:31:25 2019  by kc2019
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_DeltakkpiAlg_check_install_runtime_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_DeltakkpiAlg_check_install_runtime_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_DeltakkpiAlg_check_install_runtime

DeltakkpiAlg_tag = $(tag)

#cmt_local_tagfile_DeltakkpiAlg_check_install_runtime = $(DeltakkpiAlg_tag)_DeltakkpiAlg_check_install_runtime.make
cmt_local_tagfile_DeltakkpiAlg_check_install_runtime = $(bin)$(DeltakkpiAlg_tag)_DeltakkpiAlg_check_install_runtime.make

else

tags      = $(tag),$(CMTEXTRATAGS)

DeltakkpiAlg_tag = $(tag)

#cmt_local_tagfile_DeltakkpiAlg_check_install_runtime = $(DeltakkpiAlg_tag).make
cmt_local_tagfile_DeltakkpiAlg_check_install_runtime = $(bin)$(DeltakkpiAlg_tag).make

endif

include $(cmt_local_tagfile_DeltakkpiAlg_check_install_runtime)
#-include $(cmt_local_tagfile_DeltakkpiAlg_check_install_runtime)

ifdef cmt_DeltakkpiAlg_check_install_runtime_has_target_tag

cmt_final_setup_DeltakkpiAlg_check_install_runtime = $(bin)setup_DeltakkpiAlg_check_install_runtime.make
cmt_dependencies_in_DeltakkpiAlg_check_install_runtime = $(bin)dependencies_DeltakkpiAlg_check_install_runtime.in
#cmt_final_setup_DeltakkpiAlg_check_install_runtime = $(bin)DeltakkpiAlg_DeltakkpiAlg_check_install_runtimesetup.make
cmt_local_DeltakkpiAlg_check_install_runtime_makefile = $(bin)DeltakkpiAlg_check_install_runtime.make

else

cmt_final_setup_DeltakkpiAlg_check_install_runtime = $(bin)setup.make
cmt_dependencies_in_DeltakkpiAlg_check_install_runtime = $(bin)dependencies.in
#cmt_final_setup_DeltakkpiAlg_check_install_runtime = $(bin)DeltakkpiAlgsetup.make
cmt_local_DeltakkpiAlg_check_install_runtime_makefile = $(bin)DeltakkpiAlg_check_install_runtime.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)DeltakkpiAlgsetup.make

#DeltakkpiAlg_check_install_runtime :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'DeltakkpiAlg_check_install_runtime'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = DeltakkpiAlg_check_install_runtime/
#DeltakkpiAlg_check_install_runtime::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
#-- start of cmt_action_runner_header ---------------

ifdef ONCE
DeltakkpiAlg_check_install_runtime_once = 1
endif

ifdef DeltakkpiAlg_check_install_runtime_once

DeltakkpiAlg_check_install_runtimeactionstamp = $(bin)DeltakkpiAlg_check_install_runtime.actionstamp
#DeltakkpiAlg_check_install_runtimeactionstamp = DeltakkpiAlg_check_install_runtime.actionstamp

DeltakkpiAlg_check_install_runtime :: $(DeltakkpiAlg_check_install_runtimeactionstamp)
	$(echo) "DeltakkpiAlg_check_install_runtime ok"
#	@echo DeltakkpiAlg_check_install_runtime ok

#$(DeltakkpiAlg_check_install_runtimeactionstamp) :: $(DeltakkpiAlg_check_install_runtime_dependencies)
$(DeltakkpiAlg_check_install_runtimeactionstamp) ::
	$(silent) /data/lzuhep/sw/Boss/dist/7.0.3/BesPolicy/BesPolicy-01-05-05/cmt/bes_check_installations.sh -files= -s=../share *.txt   -installdir=/data2/kc2019/workfs/7.0.3/InstallArea/share
	$(silent) cat /dev/null > $(DeltakkpiAlg_check_install_runtimeactionstamp)
#	@echo ok > $(DeltakkpiAlg_check_install_runtimeactionstamp)

DeltakkpiAlg_check_install_runtimeclean ::
	$(cleanup_silent) /bin/rm -f $(DeltakkpiAlg_check_install_runtimeactionstamp)

else

#DeltakkpiAlg_check_install_runtime :: $(DeltakkpiAlg_check_install_runtime_dependencies)
DeltakkpiAlg_check_install_runtime ::
	$(silent) /data/lzuhep/sw/Boss/dist/7.0.3/BesPolicy/BesPolicy-01-05-05/cmt/bes_check_installations.sh -files= -s=../share *.txt   -installdir=/data2/kc2019/workfs/7.0.3/InstallArea/share

endif

install ::
uninstall ::

#-- end of cmt_action_runner_header -----------------
#-- start of cleanup_header --------------

clean :: DeltakkpiAlg_check_install_runtimeclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(DeltakkpiAlg_check_install_runtime.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

DeltakkpiAlg_check_install_runtimeclean ::
#-- end of cleanup_header ---------------
