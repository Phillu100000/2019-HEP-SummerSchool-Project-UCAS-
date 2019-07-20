#-- start of make_header -----------------

#====================================
#  Library McDecayModeSvc
#
#   Generated Fri Jul 19 09:05:46 2019  by kc2019
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

#cmt_local_tagfile_McDecayModeSvc = $(McDecayModeSvc_tag)_McDecayModeSvc.make
cmt_local_tagfile_McDecayModeSvc = $(bin)$(McDecayModeSvc_tag)_McDecayModeSvc.make

else

tags      = $(tag),$(CMTEXTRATAGS)

McDecayModeSvc_tag = $(tag)

#cmt_local_tagfile_McDecayModeSvc = $(McDecayModeSvc_tag).make
cmt_local_tagfile_McDecayModeSvc = $(bin)$(McDecayModeSvc_tag).make

endif

include $(cmt_local_tagfile_McDecayModeSvc)
#-include $(cmt_local_tagfile_McDecayModeSvc)

ifdef cmt_McDecayModeSvc_has_target_tag

cmt_final_setup_McDecayModeSvc = $(bin)setup_McDecayModeSvc.make
cmt_dependencies_in_McDecayModeSvc = $(bin)dependencies_McDecayModeSvc.in
#cmt_final_setup_McDecayModeSvc = $(bin)McDecayModeSvc_McDecayModeSvcsetup.make
cmt_local_McDecayModeSvc_makefile = $(bin)McDecayModeSvc.make

else

cmt_final_setup_McDecayModeSvc = $(bin)setup.make
cmt_dependencies_in_McDecayModeSvc = $(bin)dependencies.in
#cmt_final_setup_McDecayModeSvc = $(bin)McDecayModeSvcsetup.make
cmt_local_McDecayModeSvc_makefile = $(bin)McDecayModeSvc.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)McDecayModeSvcsetup.make

#McDecayModeSvc :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'McDecayModeSvc'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = McDecayModeSvc/
#McDecayModeSvc::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
#-- start of libary_header ---------------

McDecayModeSvclibname   = $(bin)$(library_prefix)McDecayModeSvc$(library_suffix)
McDecayModeSvclib       = $(McDecayModeSvclibname).a
McDecayModeSvcstamp     = $(bin)McDecayModeSvc.stamp
McDecayModeSvcshstamp   = $(bin)McDecayModeSvc.shstamp

McDecayModeSvc :: dirs  McDecayModeSvcLIB
	$(echo) "McDecayModeSvc ok"

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




#-- start of cpp_library -----------------

ifneq (-MMD -MP -MF $*.d -MQ $@,)

ifneq ($(MAKECMDGOALS),McDecayModeSvcclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)McDecayModeSvc_entries.d

$(bin)$(binobj)McDecayModeSvc_entries.d :

$(bin)$(binobj)McDecayModeSvc_entries.o : $(cmt_final_setup_McDecayModeSvc)

$(bin)$(binobj)McDecayModeSvc_entries.o : $(src)components/McDecayModeSvc_entries.cxx
	$(cpp_echo) $(src)components/McDecayModeSvc_entries.cxx
	$(cpp_silent) $(cppcomp) -MMD -MP -MF $*.d -MQ $@ -o $@ $(use_pp_cppflags) $(McDecayModeSvc_pp_cppflags) $(lib_McDecayModeSvc_pp_cppflags) $(McDecayModeSvc_entries_pp_cppflags) $(use_cppflags) $(McDecayModeSvc_cppflags) $(lib_McDecayModeSvc_cppflags) $(McDecayModeSvc_entries_cppflags) $(McDecayModeSvc_entries_cxx_cppflags) -I../src/components $(src)components/McDecayModeSvc_entries.cxx
endif
endif

else
$(bin)McDecayModeSvc_dependencies.make : $(McDecayModeSvc_entries_cxx_dependencies)

$(bin)McDecayModeSvc_dependencies.make : $(src)components/McDecayModeSvc_entries.cxx

$(bin)$(binobj)McDecayModeSvc_entries.o : $(McDecayModeSvc_entries_cxx_dependencies)
	$(cpp_echo) $(src)components/McDecayModeSvc_entries.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(McDecayModeSvc_pp_cppflags) $(lib_McDecayModeSvc_pp_cppflags) $(McDecayModeSvc_entries_pp_cppflags) $(use_cppflags) $(McDecayModeSvc_cppflags) $(lib_McDecayModeSvc_cppflags) $(McDecayModeSvc_entries_cppflags) $(McDecayModeSvc_entries_cxx_cppflags) -I../src/components $(src)components/McDecayModeSvc_entries.cxx

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (-MMD -MP -MF $*.d -MQ $@,)

ifneq ($(MAKECMDGOALS),McDecayModeSvcclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)McDecayModeSvc_load.d

$(bin)$(binobj)McDecayModeSvc_load.d :

$(bin)$(binobj)McDecayModeSvc_load.o : $(cmt_final_setup_McDecayModeSvc)

$(bin)$(binobj)McDecayModeSvc_load.o : $(src)components/McDecayModeSvc_load.cxx
	$(cpp_echo) $(src)components/McDecayModeSvc_load.cxx
	$(cpp_silent) $(cppcomp) -MMD -MP -MF $*.d -MQ $@ -o $@ $(use_pp_cppflags) $(McDecayModeSvc_pp_cppflags) $(lib_McDecayModeSvc_pp_cppflags) $(McDecayModeSvc_load_pp_cppflags) $(use_cppflags) $(McDecayModeSvc_cppflags) $(lib_McDecayModeSvc_cppflags) $(McDecayModeSvc_load_cppflags) $(McDecayModeSvc_load_cxx_cppflags) -I../src/components $(src)components/McDecayModeSvc_load.cxx
endif
endif

else
$(bin)McDecayModeSvc_dependencies.make : $(McDecayModeSvc_load_cxx_dependencies)

$(bin)McDecayModeSvc_dependencies.make : $(src)components/McDecayModeSvc_load.cxx

$(bin)$(binobj)McDecayModeSvc_load.o : $(McDecayModeSvc_load_cxx_dependencies)
	$(cpp_echo) $(src)components/McDecayModeSvc_load.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(McDecayModeSvc_pp_cppflags) $(lib_McDecayModeSvc_pp_cppflags) $(McDecayModeSvc_load_pp_cppflags) $(use_cppflags) $(McDecayModeSvc_cppflags) $(lib_McDecayModeSvc_cppflags) $(McDecayModeSvc_load_cppflags) $(McDecayModeSvc_load_cxx_cppflags) -I../src/components $(src)components/McDecayModeSvc_load.cxx

endif

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: McDecayModeSvcclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(McDecayModeSvc.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

McDecayModeSvcclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library McDecayModeSvc
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(library_prefix)McDecayModeSvc$(library_suffix).a $(library_prefix)McDecayModeSvc$(library_suffix).s? McDecayModeSvc.stamp McDecayModeSvc.shstamp
#-- end of cleanup_library ---------------
