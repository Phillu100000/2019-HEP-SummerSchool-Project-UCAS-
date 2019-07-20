#-- start of make_header -----------------

#====================================
#  Library McDecayModeSvcLib
#
#   Generated Fri Oct 14 20:12:04 2011  by zhangr
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_McDecayModeSvcLib_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_McDecayModeSvcLib_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_McDecayModeSvcLib

McDecayModeSvc_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_McDecayModeSvcLib = /tmp/CMT_$(McDecayModeSvc_tag)_McDecayModeSvcLib.make$(cmt_lock_pid)
else
cmt_local_tagfile_McDecayModeSvcLib = $(McDecayModeSvc_tag)_McDecayModeSvcLib.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

McDecayModeSvc_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_McDecayModeSvcLib = /tmp/CMT_$(McDecayModeSvc_tag).make$(cmt_lock_pid)
else
cmt_local_tagfile_McDecayModeSvcLib = $(McDecayModeSvc_tag).make
endif

endif

-include $(cmt_local_tagfile_McDecayModeSvcLib)

ifdef cmt_McDecayModeSvcLib_has_target_tag

ifdef READONLY
cmt_final_setup_McDecayModeSvcLib = /tmp/CMT_McDecayModeSvc_McDecayModeSvcLibsetup.make
cmt_local_McDecayModeSvcLib_makefile = /tmp/CMT_McDecayModeSvcLib$(cmt_lock_pid).make
else
cmt_final_setup_McDecayModeSvcLib = $(bin)McDecayModeSvc_McDecayModeSvcLibsetup.make
cmt_local_McDecayModeSvcLib_makefile = $(bin)McDecayModeSvcLib.make
endif

else

ifdef READONLY
cmt_final_setup_McDecayModeSvcLib = /tmp/CMT_McDecayModeSvcsetup.make
cmt_local_McDecayModeSvcLib_makefile = /tmp/CMT_McDecayModeSvcLib$(cmt_lock_pid).make
else
cmt_final_setup_McDecayModeSvcLib = $(bin)McDecayModeSvcsetup.make
cmt_local_McDecayModeSvcLib_makefile = $(bin)McDecayModeSvcLib.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_McDecayModeSvcsetup.make
else
cmt_final_setup = $(bin)McDecayModeSvcsetup.make
endif

McDecayModeSvcLib ::


ifdef READONLY
McDecayModeSvcLib ::
	@echo tags=$(tags)
	@echo cmt_local_tagfile=$(cmt_local_tagfile)
endif


dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	@echo 'McDecayModeSvcLib'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = McDecayModeSvcLib/
McDecayModeSvcLib::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	@echo "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

McDecayModeSvcLiblibname   = $(bin)$(library_prefix)McDecayModeSvcLib$(library_suffix)
McDecayModeSvcLiblib       = $(McDecayModeSvcLiblibname).a
McDecayModeSvcLibstamp     = $(bin)McDecayModeSvcLib.stamp
McDecayModeSvcLibshstamp   = $(bin)McDecayModeSvcLib.shstamp

McDecayModeSvcLib :: dirs  McDecayModeSvcLibLIB
	@/bin/echo "------> McDecayModeSvcLib ok"

#-- end of libary_header ----------------

McDecayModeSvcLibLIB :: $(McDecayModeSvcLiblib) $(McDecayModeSvcLibshstamp)
	@/bin/echo "------> McDecayModeSvcLib : library ok"

$(McDecayModeSvcLiblib) :: $(bin)McDecayModeSvc.o $(bin)DecayParticle.o $(bin)DecayModes.o $(bin)PartId2Name.o
	$(lib_echo) library
	$(lib_silent) cd $(bin); \
	  $(ar) $(McDecayModeSvcLiblib) $?
	$(lib_silent) $(ranlib) $(McDecayModeSvcLiblib)
	$(lib_silent) cat /dev/null >$(McDecayModeSvcLibstamp)

#------------------------------------------------------------------
#  Future improvement? to empty the object files after
#  storing in the library
#
##	  for f in $?; do \
##	    rm $${f}; touch $${f}; \
##	  done
#------------------------------------------------------------------

$(McDecayModeSvcLiblibname).$(shlibsuffix) :: $(McDecayModeSvcLiblib) $(McDecayModeSvcLibstamps)
	$(lib_silent) cd $(bin); QUIET=$(QUIET); $(make_shlib) "$(tags)" McDecayModeSvcLib $(McDecayModeSvcLib_shlibflags)

$(McDecayModeSvcLibshstamp) :: $(McDecayModeSvcLiblibname).$(shlibsuffix)
	@if test -f $(McDecayModeSvcLiblibname).$(shlibsuffix) ; then cat /dev/null >$(McDecayModeSvcLibshstamp) ; fi

McDecayModeSvcLibclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) cd $(bin); /bin/rm -f $(bin)McDecayModeSvc.o $(bin)DecayParticle.o $(bin)DecayModes.o $(bin)PartId2Name.o

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

ifeq ($(INSTALLAREA),)
installarea = $(CMTINSTALLAREA)
else
ifeq ($(findstring `,$(INSTALLAREA)),`)
installarea = $(shell $(subst `,, $(INSTALLAREA)))
else
installarea = $(INSTALLAREA)
endif
endif

install_dir = ${installarea}/${CMTCONFIG}/lib
McDecayModeSvcLibinstallname = $(library_prefix)McDecayModeSvcLib$(library_suffix).$(shlibsuffix)

McDecayModeSvcLib :: McDecayModeSvcLibinstall

install :: McDecayModeSvcLibinstall

McDecayModeSvcLibinstall :: $(install_dir)/$(McDecayModeSvcLibinstallname)
	@if test ! "${installarea}" = ""; then\
	  echo "installation done"; \
	fi

$(install_dir)/$(McDecayModeSvcLibinstallname) :: $(bin)$(McDecayModeSvcLibinstallname)
	@if test ! "${installarea}" = ""; then \
	  cd $(bin); \
	  if test ! "$(install_dir)" = ""; then \
	    if test ! -d "$(install_dir)"; then \
	      mkdir -p $(install_dir); \
	    fi ; \
	    if test -d "$(install_dir)"; then \
	      echo "Installing library $(McDecayModeSvcLibinstallname) into $(install_dir)"; \
	      if test -e $(install_dir)/$(McDecayModeSvcLibinstallname); then \
	        $(cmt_uninstall_area_command) $(install_dir)/$(McDecayModeSvcLibinstallname); \
	        $(cmt_uninstall_area_command) $(install_dir)/$(McDecayModeSvcLibinstallname).cmtref; \
	      fi; \
	      $(cmt_install_area_command) `pwd`/$(McDecayModeSvcLibinstallname) $(install_dir)/$(McDecayModeSvcLibinstallname); \
	      echo `pwd`/$(McDecayModeSvcLibinstallname) >$(install_dir)/$(McDecayModeSvcLibinstallname).cmtref; \
	    fi \
          else \
	    echo "Cannot install library $(McDecayModeSvcLibinstallname), no installation directory specified"; \
	  fi; \
	fi

McDecayModeSvcLibclean :: McDecayModeSvcLibuninstall

uninstall :: McDecayModeSvcLibuninstall

McDecayModeSvcLibuninstall ::
	@if test ! "${installarea}" = ""; then \
	  cd $(bin); \
	  if test ! "$(install_dir)" = ""; then \
	    if test -d "$(install_dir)"; then \
	      echo "Removing installed library $(McDecayModeSvcLibinstallname) from $(install_dir)"; \
	      $(cmt_uninstall_area_command) $(install_dir)/$(McDecayModeSvcLibinstallname); \
	      $(cmt_uninstall_area_command) $(install_dir)/$(McDecayModeSvcLibinstallname).cmtref; \
	    fi \
          else \
	    echo "Cannot uninstall library $(McDecayModeSvcLibinstallname), no installation directory specified"; \
	  fi; \
	fi




#-- start of dependency ------------------

$(bin)McDecayModeSvcLib_dependencies.make :: dirs

ifndef QUICK
$(bin)McDecayModeSvcLib_dependencies.make :: $(src)*.cxx  requirements $(use_requirements) $(cmt_final_setup_McDecayModeSvcLib)
	@echo "------> (McDecayModeSvcLib.make) Rebuilding $@"; \
	  args=`echo $? | sed -e "s#requirements.*##"`; \
	  $(build_dependencies) McDecayModeSvcLib -all_sources $${args}
endif

#$(McDecayModeSvcLib_dependencies)

-include $(bin)McDecayModeSvcLib_dependencies.make

#-- end of dependency -------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)McDecayModeSvc.o : $(McDecayModeSvc_cxx_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)McDecayModeSvc.o $(use_pp_cppflags) $(McDecayModeSvcLib_pp_cppflags) $(lib_McDecayModeSvcLib_pp_cppflags) $(McDecayModeSvc_pp_cppflags) $(use_cppflags) $(McDecayModeSvcLib_cppflags) $(lib_McDecayModeSvcLib_cppflags) $(McDecayModeSvc_cppflags) $(McDecayModeSvc_cxx_cppflags)  $(src)McDecayModeSvc.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)DecayParticle.o : $(DecayParticle_cxx_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)DecayParticle.o $(use_pp_cppflags) $(McDecayModeSvcLib_pp_cppflags) $(lib_McDecayModeSvcLib_pp_cppflags) $(DecayParticle_pp_cppflags) $(use_cppflags) $(McDecayModeSvcLib_cppflags) $(lib_McDecayModeSvcLib_cppflags) $(DecayParticle_cppflags) $(DecayParticle_cxx_cppflags)  $(src)DecayParticle.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)DecayModes.o : $(DecayModes_cxx_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)DecayModes.o $(use_pp_cppflags) $(McDecayModeSvcLib_pp_cppflags) $(lib_McDecayModeSvcLib_pp_cppflags) $(DecayModes_pp_cppflags) $(use_cppflags) $(McDecayModeSvcLib_cppflags) $(lib_McDecayModeSvcLib_cppflags) $(DecayModes_cppflags) $(DecayModes_cxx_cppflags)  $(src)DecayModes.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)PartId2Name.o : $(PartId2Name_cxx_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)PartId2Name.o $(use_pp_cppflags) $(McDecayModeSvcLib_pp_cppflags) $(lib_McDecayModeSvcLib_pp_cppflags) $(PartId2Name_pp_cppflags) $(use_cppflags) $(McDecayModeSvcLib_cppflags) $(lib_McDecayModeSvcLib_cppflags) $(PartId2Name_cppflags) $(PartId2Name_cxx_cppflags)  $(src)PartId2Name.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: McDecayModeSvcLibclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	@echo "WARNING >> You should provide a target for $@"
else
.DEFAULT::
	@echo "PEDANTIC MODE >> You should provide a target for $@"
	@if test `expr index $@ '.'` != 0 ; \
	then  echo "PEDANTIC MODE >> This target seems to be a missing file, please check"; exit -1 ;\
	else echo "PEDANTIC MODE >> Just ignore it ; as it seems to be just a fake target due to some pattern" ; exit 0; fi; 		
endif

McDecayModeSvcLibclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)McDecayModeSvcLib$(library_suffix).a $(binobj)$(library_prefix)McDecayModeSvcLib$(library_suffix).s? $(binobj)McDecayModeSvcLib.stamp $(binobj)McDecayModeSvcLib.shstamp
#-- end of cleanup_library ---------------

