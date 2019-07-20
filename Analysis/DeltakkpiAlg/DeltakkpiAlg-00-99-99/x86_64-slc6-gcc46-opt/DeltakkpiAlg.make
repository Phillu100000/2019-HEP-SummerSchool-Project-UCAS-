#-- start of make_header -----------------

#====================================
#  Library DeltakkpiAlg
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

cmt_DeltakkpiAlg_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_DeltakkpiAlg_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_DeltakkpiAlg

DeltakkpiAlg_tag = $(tag)

#cmt_local_tagfile_DeltakkpiAlg = $(DeltakkpiAlg_tag)_DeltakkpiAlg.make
cmt_local_tagfile_DeltakkpiAlg = $(bin)$(DeltakkpiAlg_tag)_DeltakkpiAlg.make

else

tags      = $(tag),$(CMTEXTRATAGS)

DeltakkpiAlg_tag = $(tag)

#cmt_local_tagfile_DeltakkpiAlg = $(DeltakkpiAlg_tag).make
cmt_local_tagfile_DeltakkpiAlg = $(bin)$(DeltakkpiAlg_tag).make

endif

include $(cmt_local_tagfile_DeltakkpiAlg)
#-include $(cmt_local_tagfile_DeltakkpiAlg)

ifdef cmt_DeltakkpiAlg_has_target_tag

cmt_final_setup_DeltakkpiAlg = $(bin)setup_DeltakkpiAlg.make
cmt_dependencies_in_DeltakkpiAlg = $(bin)dependencies_DeltakkpiAlg.in
#cmt_final_setup_DeltakkpiAlg = $(bin)DeltakkpiAlg_DeltakkpiAlgsetup.make
cmt_local_DeltakkpiAlg_makefile = $(bin)DeltakkpiAlg.make

else

cmt_final_setup_DeltakkpiAlg = $(bin)setup.make
cmt_dependencies_in_DeltakkpiAlg = $(bin)dependencies.in
#cmt_final_setup_DeltakkpiAlg = $(bin)DeltakkpiAlgsetup.make
cmt_local_DeltakkpiAlg_makefile = $(bin)DeltakkpiAlg.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)DeltakkpiAlgsetup.make

#DeltakkpiAlg :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'DeltakkpiAlg'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = DeltakkpiAlg/
#DeltakkpiAlg::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
#-- start of libary_header ---------------

DeltakkpiAlglibname   = $(bin)$(library_prefix)DeltakkpiAlg$(library_suffix)
DeltakkpiAlglib       = $(DeltakkpiAlglibname).a
DeltakkpiAlgstamp     = $(bin)DeltakkpiAlg.stamp
DeltakkpiAlgshstamp   = $(bin)DeltakkpiAlg.shstamp

DeltakkpiAlg :: dirs  DeltakkpiAlgLIB
	$(echo) "DeltakkpiAlg ok"

#-- end of libary_header ----------------

DeltakkpiAlgLIB :: $(DeltakkpiAlglib) $(DeltakkpiAlgshstamp)
	@/bin/echo "------> DeltakkpiAlg : library ok"

$(DeltakkpiAlglib) :: $(bin)Deltakkpi.o $(bin)Ds_load.o $(bin)Ds_entries.o
	$(lib_echo) library
	$(lib_silent) cd $(bin); \
	  $(ar) $(DeltakkpiAlglib) $?
	$(lib_silent) $(ranlib) $(DeltakkpiAlglib)
	$(lib_silent) cat /dev/null >$(DeltakkpiAlgstamp)

#------------------------------------------------------------------
#  Future improvement? to empty the object files after
#  storing in the library
#
##	  for f in $?; do \
##	    rm $${f}; touch $${f}; \
##	  done
#------------------------------------------------------------------

$(DeltakkpiAlglibname).$(shlibsuffix) :: $(DeltakkpiAlglib) $(DeltakkpiAlgstamps)
	$(lib_silent) cd $(bin); QUIET=$(QUIET); $(make_shlib) "$(tags)" DeltakkpiAlg $(DeltakkpiAlg_shlibflags)

$(DeltakkpiAlgshstamp) :: $(DeltakkpiAlglibname).$(shlibsuffix)
	@if test -f $(DeltakkpiAlglibname).$(shlibsuffix) ; then cat /dev/null >$(DeltakkpiAlgshstamp) ; fi

DeltakkpiAlgclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) cd $(bin); /bin/rm -f $(bin)Deltakkpi.o $(bin)Ds_load.o $(bin)Ds_entries.o

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
DeltakkpiAlginstallname = $(library_prefix)DeltakkpiAlg$(library_suffix).$(shlibsuffix)

DeltakkpiAlg :: DeltakkpiAlginstall

install :: DeltakkpiAlginstall

DeltakkpiAlginstall :: $(install_dir)/$(DeltakkpiAlginstallname)
	@if test ! "${installarea}" = ""; then\
	  echo "installation done"; \
	fi

$(install_dir)/$(DeltakkpiAlginstallname) :: $(bin)$(DeltakkpiAlginstallname)
	@if test ! "${installarea}" = ""; then \
	  cd $(bin); \
	  if test ! "$(install_dir)" = ""; then \
	    if test ! -d "$(install_dir)"; then \
	      mkdir -p $(install_dir); \
	    fi ; \
	    if test -d "$(install_dir)"; then \
	      echo "Installing library $(DeltakkpiAlginstallname) into $(install_dir)"; \
	      if test -e $(install_dir)/$(DeltakkpiAlginstallname); then \
	        $(cmt_uninstall_area_command) $(install_dir)/$(DeltakkpiAlginstallname); \
	        $(cmt_uninstall_area_command) $(install_dir)/$(DeltakkpiAlginstallname).cmtref; \
	      fi; \
	      $(cmt_install_area_command) `pwd`/$(DeltakkpiAlginstallname) $(install_dir)/$(DeltakkpiAlginstallname); \
	      echo `pwd`/$(DeltakkpiAlginstallname) >$(install_dir)/$(DeltakkpiAlginstallname).cmtref; \
	    fi \
          else \
	    echo "Cannot install library $(DeltakkpiAlginstallname), no installation directory specified"; \
	  fi; \
	fi

DeltakkpiAlgclean :: DeltakkpiAlguninstall

uninstall :: DeltakkpiAlguninstall

DeltakkpiAlguninstall ::
	@if test ! "${installarea}" = ""; then \
	  cd $(bin); \
	  if test ! "$(install_dir)" = ""; then \
	    if test -d "$(install_dir)"; then \
	      echo "Removing installed library $(DeltakkpiAlginstallname) from $(install_dir)"; \
	      $(cmt_uninstall_area_command) $(install_dir)/$(DeltakkpiAlginstallname); \
	      $(cmt_uninstall_area_command) $(install_dir)/$(DeltakkpiAlginstallname).cmtref; \
	    fi \
          else \
	    echo "Cannot uninstall library $(DeltakkpiAlginstallname), no installation directory specified"; \
	  fi; \
	fi




#-- start of cpp_library -----------------

ifneq (-MMD -MP -MF $*.d -MQ $@,)

ifneq ($(MAKECMDGOALS),DeltakkpiAlgclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)Deltakkpi.d

$(bin)$(binobj)Deltakkpi.d :

$(bin)$(binobj)Deltakkpi.o : $(cmt_final_setup_DeltakkpiAlg)

$(bin)$(binobj)Deltakkpi.o : $(src)Deltakkpi.cxx
	$(cpp_echo) $(src)Deltakkpi.cxx
	$(cpp_silent) $(cppcomp) -MMD -MP -MF $*.d -MQ $@ -o $@ $(use_pp_cppflags) $(DeltakkpiAlg_pp_cppflags) $(lib_DeltakkpiAlg_pp_cppflags) $(Deltakkpi_pp_cppflags) $(use_cppflags) $(DeltakkpiAlg_cppflags) $(lib_DeltakkpiAlg_cppflags) $(Deltakkpi_cppflags) $(Deltakkpi_cxx_cppflags)  $(src)Deltakkpi.cxx
endif
endif

else
$(bin)DeltakkpiAlg_dependencies.make : $(Deltakkpi_cxx_dependencies)

$(bin)DeltakkpiAlg_dependencies.make : $(src)Deltakkpi.cxx

$(bin)$(binobj)Deltakkpi.o : $(Deltakkpi_cxx_dependencies)
	$(cpp_echo) $(src)Deltakkpi.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(DeltakkpiAlg_pp_cppflags) $(lib_DeltakkpiAlg_pp_cppflags) $(Deltakkpi_pp_cppflags) $(use_cppflags) $(DeltakkpiAlg_cppflags) $(lib_DeltakkpiAlg_cppflags) $(Deltakkpi_cppflags) $(Deltakkpi_cxx_cppflags)  $(src)Deltakkpi.cxx

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (-MMD -MP -MF $*.d -MQ $@,)

ifneq ($(MAKECMDGOALS),DeltakkpiAlgclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)Ds_load.d

$(bin)$(binobj)Ds_load.d :

$(bin)$(binobj)Ds_load.o : $(cmt_final_setup_DeltakkpiAlg)

$(bin)$(binobj)Ds_load.o : $(src)components/Ds_load.cxx
	$(cpp_echo) $(src)components/Ds_load.cxx
	$(cpp_silent) $(cppcomp) -MMD -MP -MF $*.d -MQ $@ -o $@ $(use_pp_cppflags) $(DeltakkpiAlg_pp_cppflags) $(lib_DeltakkpiAlg_pp_cppflags) $(Ds_load_pp_cppflags) $(use_cppflags) $(DeltakkpiAlg_cppflags) $(lib_DeltakkpiAlg_cppflags) $(Ds_load_cppflags) $(Ds_load_cxx_cppflags) -I../src/components $(src)components/Ds_load.cxx
endif
endif

else
$(bin)DeltakkpiAlg_dependencies.make : $(Ds_load_cxx_dependencies)

$(bin)DeltakkpiAlg_dependencies.make : $(src)components/Ds_load.cxx

$(bin)$(binobj)Ds_load.o : $(Ds_load_cxx_dependencies)
	$(cpp_echo) $(src)components/Ds_load.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(DeltakkpiAlg_pp_cppflags) $(lib_DeltakkpiAlg_pp_cppflags) $(Ds_load_pp_cppflags) $(use_cppflags) $(DeltakkpiAlg_cppflags) $(lib_DeltakkpiAlg_cppflags) $(Ds_load_cppflags) $(Ds_load_cxx_cppflags) -I../src/components $(src)components/Ds_load.cxx

endif

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

ifneq (-MMD -MP -MF $*.d -MQ $@,)

ifneq ($(MAKECMDGOALS),DeltakkpiAlgclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)Ds_entries.d

$(bin)$(binobj)Ds_entries.d :

$(bin)$(binobj)Ds_entries.o : $(cmt_final_setup_DeltakkpiAlg)

$(bin)$(binobj)Ds_entries.o : $(src)components/Ds_entries.cxx
	$(cpp_echo) $(src)components/Ds_entries.cxx
	$(cpp_silent) $(cppcomp) -MMD -MP -MF $*.d -MQ $@ -o $@ $(use_pp_cppflags) $(DeltakkpiAlg_pp_cppflags) $(lib_DeltakkpiAlg_pp_cppflags) $(Ds_entries_pp_cppflags) $(use_cppflags) $(DeltakkpiAlg_cppflags) $(lib_DeltakkpiAlg_cppflags) $(Ds_entries_cppflags) $(Ds_entries_cxx_cppflags) -I../src/components $(src)components/Ds_entries.cxx
endif
endif

else
$(bin)DeltakkpiAlg_dependencies.make : $(Ds_entries_cxx_dependencies)

$(bin)DeltakkpiAlg_dependencies.make : $(src)components/Ds_entries.cxx

$(bin)$(binobj)Ds_entries.o : $(Ds_entries_cxx_dependencies)
	$(cpp_echo) $(src)components/Ds_entries.cxx
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(DeltakkpiAlg_pp_cppflags) $(lib_DeltakkpiAlg_pp_cppflags) $(Ds_entries_pp_cppflags) $(use_cppflags) $(DeltakkpiAlg_cppflags) $(lib_DeltakkpiAlg_cppflags) $(Ds_entries_cppflags) $(Ds_entries_cxx_cppflags) -I../src/components $(src)components/Ds_entries.cxx

endif

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: DeltakkpiAlgclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(DeltakkpiAlg.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

DeltakkpiAlgclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library DeltakkpiAlg
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(library_prefix)DeltakkpiAlg$(library_suffix).a $(library_prefix)DeltakkpiAlg$(library_suffix).s? DeltakkpiAlg.stamp DeltakkpiAlg.shstamp
#-- end of cleanup_library ---------------
