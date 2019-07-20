#-- start of make_header -----------------

#====================================
#  Library McDecayModeSvcLib
#
#   Generated Fri Jul 19 09:05:44 2019  by kc2019
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

#cmt_local_tagfile_McDecayModeSvcLib = $(McDecayModeSvc_tag)_McDecayModeSvcLib.make
cmt_local_tagfile_McDecayModeSvcLib = $(bin)$(McDecayModeSvc_tag)_McDecayModeSvcLib.make

else

tags      = $(tag),$(CMTEXTRATAGS)

McDecayModeSvc_tag = $(tag)

#cmt_local_tagfile_McDecayModeSvcLib = $(McDecayModeSvc_tag).make
cmt_local_tagfile_McDecayModeSvcLib = $(bin)$(McDecayModeSvc_tag).make

endif

include $(cmt_local_tagfile_McDecayModeSvcLib)
#-include $(cmt_local_tagfile_McDecayModeSvcLib)

ifdef cmt_McDecayModeSvcLib_has_target_tag

cmt_final_setup_McDecayModeSvcLib = $(bin)setup_McDecayModeSvcLib.make
cmt_dependencies_in_McDecayModeSvcLib = $(bin)dependencies_McDecayModeSvcLib.in
#cmt_final_setup_McDecayModeSvcLib = $(bin)McDecayModeSvc_McDecayModeSvcLibsetup.make
cmt_local_McDecayModeSvcLib_makefile = $(bin)McDecayModeSvcLib.make

else

cmt_final_setup_McDecayModeSvcLib = $(bin)setup.make
cmt_dependencies_in_McDecayModeSvcLib = $(bin)dependencies.in
#cmt_final_setup_McDecayModeSvcLib = $(bin)McDecayModeSvcsetup.make
cmt_local_McDecayModeSvcLib_makefile = $(bin)McDecayModeSvcLib.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)McDecayModeSvcsetup.make

#McDecayModeSvcLib :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'McDecayModeSvcLib'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = McDecayModeSvcLib/
#McDecayModeSvcLib::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
#-- start of libary_header ---------------

McDecayModeSvcLiblibname   = $(bin)$(library_prefix)McDecayModeSvcLib$(library_suffix)
McDecayModeSvcLiblib       = $(McDecayModeSvcLiblibname).a
McDecayModeSvcLibstamp     = $(bin)McDecayModeSvcLib.stamp
McDecayModeSvcLibshstamp   = $(bin)McDecayModeSvcLib.shstamp

McDecayModeSvcLib :: dirs  McDecayModeSvcLibLIB
	$(echo) "McDecayModeSvcLib ok"

#-- end of libary_header ----------------

McDecayModeSvcLibLIB :: $(McDecayModeSvcLiblib) $(McDecayModeSvcLibshstamp)
	@/bin/echo "------> McDecayModeSvcLib : library ok"

$(McDecayModeSvcLiblib) :: $(bin)DecayParticle.o $(bin)PartId2Name.o $(bin)McDecayModeSvc.o $(bin)DecayModes.o
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
	$(cleanup_silent) cd $(bin); /bin/rm -f $(bin)DecayParticle.o $(bin)PartId2Name.o $(bin)McDecayModeSvc.o $(bin)DecayModes.o

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




#-- start of cpp_library -----------------

ifneq (-MMD -MP -MF $*.d -MQ $@,)

ifneq ($(MAKECMDGOALS),McDecayModeSvcLibclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)DecayParticle.d

$(bin)$(binobj)DecayParticle.d :

$(bin)$(binobj)DecayParticle.o : $(cmt_final_setup_McDecayModeSvcLib)

$(bin)$(binobj)DecayParticle.o : $(src)DecayParticle.cxx
	$(cpp_echo) $(src)DecayParticle.cxx
	$(cpp_silent) $(cppcomp) -MMD -MP -MF $*.d -MQ $@ -o $@ $(use_pp_cppflags) $(McDecayModeSvcLib_pp_cppflags) $(lib_McDecayModeSvcLib_pp_cppflags) $(DecayParticle_pp_cppflags) $(use_cppflags) $(McDecayModeSvcLib_cppflags) $(lib_McDecayModeSvcLib_cppflags) $(DecayParticle_cppflags) $(DecayParticle_cxx_cppflags)  $(src)DecayParticle.cxx
endif
endif

else
$(bin)McDecayModeSvcLib_dependencies.make : $(DecayParticle_cxx_dependencies)

$(bin)McDecayModeSvcLib_dependencies.make : $(src)DecayParticle.cxx

$(bin)$(binobj)DecayParticle.o : $(DecayParticle_cxx_dependencies)
	$(cpp_echo) $(src)DecayParticle.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(McDecayModeSvcLib_pp_cppflags) $(lib_McDecayModeSvcLib_pp_cppflags) $(DecayParticle_pp_cppflags) $(use_cppflags) $(McDecayModeSvcLib_cppflags) $(lib_McDecayModeSvcLib_cppflags) $(DecayParticle_cppflags) $(DecayParticle_cxx_cppflags)  $(src)DecayParticle.cxx

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (-MMD -MP -MF $*.d -MQ $@,)

ifneq ($(MAKECMDGOALS),McDecayModeSvcLibclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)PartId2Name.d

$(bin)$(binobj)PartId2Name.d :

$(bin)$(binobj)PartId2Name.o : $(cmt_final_setup_McDecayModeSvcLib)

$(bin)$(binobj)PartId2Name.o : $(src)PartId2Name.cxx
	$(cpp_echo) $(src)PartId2Name.cxx
	$(cpp_silent) $(cppcomp) -MMD -MP -MF $*.d -MQ $@ -o $@ $(use_pp_cppflags) $(McDecayModeSvcLib_pp_cppflags) $(lib_McDecayModeSvcLib_pp_cppflags) $(PartId2Name_pp_cppflags) $(use_cppflags) $(McDecayModeSvcLib_cppflags) $(lib_McDecayModeSvcLib_cppflags) $(PartId2Name_cppflags) $(PartId2Name_cxx_cppflags)  $(src)PartId2Name.cxx
endif
endif

else
$(bin)McDecayModeSvcLib_dependencies.make : $(PartId2Name_cxx_dependencies)

$(bin)McDecayModeSvcLib_dependencies.make : $(src)PartId2Name.cxx

$(bin)$(binobj)PartId2Name.o : $(PartId2Name_cxx_dependencies)
	$(cpp_echo) $(src)PartId2Name.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(McDecayModeSvcLib_pp_cppflags) $(lib_McDecayModeSvcLib_pp_cppflags) $(PartId2Name_pp_cppflags) $(use_cppflags) $(McDecayModeSvcLib_cppflags) $(lib_McDecayModeSvcLib_cppflags) $(PartId2Name_cppflags) $(PartId2Name_cxx_cppflags)  $(src)PartId2Name.cxx

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (-MMD -MP -MF $*.d -MQ $@,)

ifneq ($(MAKECMDGOALS),McDecayModeSvcLibclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)McDecayModeSvc.d

$(bin)$(binobj)McDecayModeSvc.d :

$(bin)$(binobj)McDecayModeSvc.o : $(cmt_final_setup_McDecayModeSvcLib)

$(bin)$(binobj)McDecayModeSvc.o : $(src)McDecayModeSvc.cxx
	$(cpp_echo) $(src)McDecayModeSvc.cxx
	$(cpp_silent) $(cppcomp) -MMD -MP -MF $*.d -MQ $@ -o $@ $(use_pp_cppflags) $(McDecayModeSvcLib_pp_cppflags) $(lib_McDecayModeSvcLib_pp_cppflags) $(McDecayModeSvc_pp_cppflags) $(use_cppflags) $(McDecayModeSvcLib_cppflags) $(lib_McDecayModeSvcLib_cppflags) $(McDecayModeSvc_cppflags) $(McDecayModeSvc_cxx_cppflags)  $(src)McDecayModeSvc.cxx
endif
endif

else
$(bin)McDecayModeSvcLib_dependencies.make : $(McDecayModeSvc_cxx_dependencies)

$(bin)McDecayModeSvcLib_dependencies.make : $(src)McDecayModeSvc.cxx

$(bin)$(binobj)McDecayModeSvc.o : $(McDecayModeSvc_cxx_dependencies)
	$(cpp_echo) $(src)McDecayModeSvc.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(McDecayModeSvcLib_pp_cppflags) $(lib_McDecayModeSvcLib_pp_cppflags) $(McDecayModeSvc_pp_cppflags) $(use_cppflags) $(McDecayModeSvcLib_cppflags) $(lib_McDecayModeSvcLib_cppflags) $(McDecayModeSvc_cppflags) $(McDecayModeSvc_cxx_cppflags)  $(src)McDecayModeSvc.cxx

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (-MMD -MP -MF $*.d -MQ $@,)

ifneq ($(MAKECMDGOALS),McDecayModeSvcLibclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)DecayModes.d

$(bin)$(binobj)DecayModes.d :

$(bin)$(binobj)DecayModes.o : $(cmt_final_setup_McDecayModeSvcLib)

$(bin)$(binobj)DecayModes.o : $(src)DecayModes.cxx
	$(cpp_echo) $(src)DecayModes.cxx
	$(cpp_silent) $(cppcomp) -MMD -MP -MF $*.d -MQ $@ -o $@ $(use_pp_cppflags) $(McDecayModeSvcLib_pp_cppflags) $(lib_McDecayModeSvcLib_pp_cppflags) $(DecayModes_pp_cppflags) $(use_cppflags) $(McDecayModeSvcLib_cppflags) $(lib_McDecayModeSvcLib_cppflags) $(DecayModes_cppflags) $(DecayModes_cxx_cppflags)  $(src)DecayModes.cxx
endif
endif

else
$(bin)McDecayModeSvcLib_dependencies.make : $(DecayModes_cxx_dependencies)

$(bin)McDecayModeSvcLib_dependencies.make : $(src)DecayModes.cxx

$(bin)$(binobj)DecayModes.o : $(DecayModes_cxx_dependencies)
	$(cpp_echo) $(src)DecayModes.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(McDecayModeSvcLib_pp_cppflags) $(lib_McDecayModeSvcLib_pp_cppflags) $(DecayModes_pp_cppflags) $(use_cppflags) $(McDecayModeSvcLib_cppflags) $(lib_McDecayModeSvcLib_cppflags) $(DecayModes_cppflags) $(DecayModes_cxx_cppflags)  $(src)DecayModes.cxx

endif

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: McDecayModeSvcLibclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(McDecayModeSvcLib.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

McDecayModeSvcLibclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library McDecayModeSvcLib
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(library_prefix)McDecayModeSvcLib$(library_suffix).a $(library_prefix)McDecayModeSvcLib$(library_suffix).s? McDecayModeSvcLib.stamp McDecayModeSvcLib.shstamp
#-- end of cleanup_library ---------------
