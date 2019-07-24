#-- start of make_header -----------------

#====================================
#  Library McDecayModeSvc
#
#   Generated Fri Oct 14 20:12:05 2011  by zhangr
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_McDecayModeSvc_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_McDecayModeSvc_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_McDecayModeSvc

McDecayModeSvc_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_McDecayModeSvc = /tmp/CMT_$(McDecayModeSvc_tag)_McDecayModeSvc.make$(cmt_lock_pid)
else
cmt_local_tagfile_McDecayModeSvc = $(McDecayModeSvc_tag)_McDecayModeSvc.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

McDecayModeSvc_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_McDecayModeSvc = /tmp/CMT_$(McDecayModeSvc_tag).make$(cmt_lock_pid)
else
cmt_local_tagfile_McDecayModeSvc = $(McDecayModeSvc_tag).make
endif

endif

-include $(cmt_local_tagfile_McDecayModeSvc)

ifdef cmt_McDecayModeSvc_has_target_tag

ifdef READONLY
cmt_final_setup_McDecayModeSvc = /tmp/CMT_McDecayModeSvc_McDecayModeSvcsetup.make
cmt_local_McDecayModeSvc_makefile = /tmp/CMT_McDecayModeSvc$(cmt_lock_pid).make
else
cmt_final_setup_McDecayModeSvc = $(bin)McDecayModeSvc_McDecayModeSvcsetup.make
cmt_local_McDecayModeSvc_makefile = $(bin)McDecayModeSvc.make
endif

else

ifdef READONLY
cmt_final_setup_McDecayModeSvc = /tmp/CMT_McDecayModeSvcsetup.make
cmt_local_McDecayModeSvc_makefile = /tmp/CMT_McDecayModeSvc$(cmt_lock_pid).make
else
cmt_final_setup_McDecayModeSvc = $(bin)McDecayModeSvcsetup.make
cmt_local_McDecayModeSvc_makefile = $(bin)McDecayModeSvc.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_McDecayModeSvcsetup.make
else
cmt_final_setup = $(bin)McDecayModeSvcsetup.make
endif

McDecayModeSvc ::


ifdef READONLY
McDecayModeSvc ::
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
	@echo 'McDecayModeSvc'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = McDecayModeSvc/
McDecayModeSvc::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	@echo "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

McDecayModeSvclibname   = $(bin)$(library_prefix)McDecayModeSvc$(library_suffix)
McDecayModeSvclib       = $(McDecayModeSvclibname).a
McDecayModeSvcstamp     = $(bin)McDecayModeSvc.stamp
McDecayModeSvcshstamp   = $(bin)McDecayModeSvc.shstamp

McDecayModeSvc :: dirs  McDecayModeSvcLIB
	@/bin/echo "------> McDecayModeSvc ok"

#-- end of libary_header ----------------

McDecayModeSvcLIB :: $(McDecayModeSvclib) $(McDecayModeSvcshstamp)
	@/bin/echo "------> McDecayModeSvc : library ok"

$(McDecayModeSvclib) :: $(bin)McDecayModeSvc_entries.o $(bin)McDecayModeSvc_load.o
	$(lib_echo) library
	$(lib_silent) cd $(bin); \
	  $(ar) $(McDecayModeSvclib) $?
	$(lib_silent) $(ranlib) $(McDecayModeSvclib)
	$(lib_silent) cat /dev/null >$(McDecayModeSvcstamp)

#------------------------------------------------------------------
#  Future improvement? to empty the object files after
#  storing in the library
#
##	  for f in $?; do \
##	    rm $${f}; touch $${f}; \
##	  done
#------------------------------------------------------------------

$(McDecayModeSvclibname).$(shlibsuffix) :: $(McDecayModeSvclib) $(McDecayModeSvcstamps)
	$(lib_silent) cd $(bin); QUIET=$(QUIET); $(make_shlib) "$(tags)" McDecayModeSvc $(McDecayModeSvc_shlibflags)

$(McDecayModeSvcshstamp) :: $(McDecayModeSvclibname).$(shlibsuffix)
	@if test -f $(McDecayModeSvclibname).$(shlibsuffix) ; then cat /dev/null >$(McDecayModeSvcshstamp) ; fi

McDecayModeSvcclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) cd $(bin); /bin/rm -f $(bin)McDecayModeSvc_entries.o $(bin)McDecayModeSvc_load.o

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
McDecayModeSvcinstallname = $(library_prefix)McDecayModeSvc$(library_suffix).$(shlibsuffix)

McDecayModeSvc :: McDecayModeSvcinstall

install :: McDecayModeSvcinstall

McDecayModeSvcinstall :: $(install_dir)/$(McDecayModeSvcinstallname)
	@if test ! "${installarea}" = ""; then\
	  echo "installation done"; \
	fi

$(install_dir)/$(McDecayModeSvcinstallname) :: $(bin)$(McDecayModeSvcinstallname)
	@if test ! "${installarea}" = ""; then \
	  cd $(bin); \
	  if test ! "$(install_dir)" = ""; then \
	    if test ! -d "$(install_dir)"; then \
	      mkdir -p $(install_dir); \
	    fi ; \
	    if test -d "$(install_dir)"; then \
	      echo "Installing library $(McDecayModeSvcinstallname) into $(install_dir)"; \
	      if test -e $(install_dir)/$(McDecayModeSvcinstallname); then \
	        $(cmt_uninstall_area_command) $(install_dir)/$(McDecayModeSvcinstallname); \
	        $(cmt_uninstall_area_command) $(install_dir)/$(McDecayModeSvcinstallname).cmtref; \
	      fi; \
	      $(cmt_install_area_command) `pwd`/$(McDecayModeSvcinstallname) $(install_dir)/$(McDecayModeSvcinstallname); \
	      echo `pwd`/$(McDecayModeSvcinstallname) >$(install_dir)/$(McDecayModeSvcinstallname).cmtref; \
	    fi \
          else \
	    echo "Cannot install library $(McDecayModeSvcinstallname), no installation directory specified"; \
	  fi; \
	fi

McDecayModeSvcclean :: McDecayModeSvcuninstall

uninstall :: McDecayModeSvcuninstall

McDecayModeSvcuninstall ::
	@if test ! "${installarea}" = ""; then \
	  cd $(bin); \
	  if test ! "$(install_dir)" = ""; then \
	    if test -d "$(install_dir)"; then \
	      echo "Removing installed library $(McDecayModeSvcinstallname) from $(install_dir)"; \
	      $(cmt_uninstall_area_command) $(install_dir)/$(McDecayModeSvcinstallname); \
	      $(cmt_uninstall_area_command) $(install_dir)/$(McDecayModeSvcinstallname).cmtref; \
	    fi \
          else \
	    echo "Cannot uninstall library $(McDecayModeSvcinstallname), no installation directory specified"; \
	  fi; \
	fi




#-- start of dependency ------------------

$(bin)McDecayModeSvc_dependencies.make :: dirs

ifndef QUICK
$(bin)McDecayModeSvc_dependencies.make :: $(src)components/*.cxx  requirements $(use_requirements) $(cmt_final_setup_McDecayModeSvc)
	@echo "------> (McDecayModeSvc.make) Rebuilding $@"; \
	  args=`echo $? | sed -e "s#requirements.*##"`; \
	  $(build_dependencies) McDecayModeSvc -all_sources $${args}
endif

#$(McDecayModeSvc_dependencies)

-include $(bin)McDecayModeSvc_dependencies.make

#-- end of dependency -------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)McDecayModeSvc_entries.o : $(McDecayModeSvc_entries_cxx_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)McDecayModeSvc_entries.o $(use_pp_cppflags) $(McDecayModeSvc_pp_cppflags) $(lib_McDecayModeSvc_pp_cppflags) $(McDecayModeSvc_entries_pp_cppflags) $(use_cppflags) $(McDecayModeSvc_cppflags) $(lib_McDecayModeSvc_cppflags) $(McDecayModeSvc_entries_cppflags) $(McDecayModeSvc_entries_cxx_cppflags) -I../src/components $(src)components/McDecayModeSvc_entries.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------
#
$(bin)$(binobj)McDecayModeSvc_load.o : $(McDecayModeSvc_load_cxx_dependencies)
	$(cpp_echo) $@
	$(cpp_silent) cd $(bin); $(cppcomp) -o $(binobj)McDecayModeSvc_load.o $(use_pp_cppflags) $(McDecayModeSvc_pp_cppflags) $(lib_McDecayModeSvc_pp_cppflags) $(McDecayModeSvc_load_pp_cppflags) $(use_cppflags) $(McDecayModeSvc_cppflags) $(lib_McDecayModeSvc_cppflags) $(McDecayModeSvc_load_cppflags) $(McDecayModeSvc_load_cxx_cppflags) -I../src/components $(src)components/McDecayModeSvc_load.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: McDecayModeSvcclean
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

McDecayModeSvcclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)McDecayModeSvc$(library_suffix).a $(binobj)$(library_prefix)McDecayModeSvc$(library_suffix).s? $(binobj)McDecayModeSvc.stamp $(binobj)McDecayModeSvc.shstamp
#-- end of cleanup_library ---------------

