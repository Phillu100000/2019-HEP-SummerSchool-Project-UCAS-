#-- start of make_header -----------------

#====================================
#  Library LprFastAlg
#
#   Generated Wed Aug 17 10:55:42 2016  by lipr
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_LprFastAlg_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_LprFastAlg_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_LprFastAlg

LprFastAlg_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_LprFastAlg = /tmp/CMT_$(LprFastAlg_tag)_LprFastAlg.make$(cmt_lock_pid)
else
#cmt_local_tagfile_LprFastAlg = $(LprFastAlg_tag)_LprFastAlg.make
cmt_local_tagfile_LprFastAlg = $(bin)$(LprFastAlg_tag)_LprFastAlg.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

LprFastAlg_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_LprFastAlg = /tmp/CMT_$(LprFastAlg_tag).make$(cmt_lock_pid)
else
#cmt_local_tagfile_LprFastAlg = $(LprFastAlg_tag).make
cmt_local_tagfile_LprFastAlg = $(bin)$(LprFastAlg_tag).make
endif

endif

-include $(cmt_local_tagfile_LprFastAlg)

ifdef cmt_LprFastAlg_has_target_tag

ifdef READONLY
cmt_final_setup_LprFastAlg = /tmp/CMT_LprFastAlg_LprFastAlgsetup.make
cmt_local_LprFastAlg_makefile = /tmp/CMT_LprFastAlg$(cmt_lock_pid).make
else
cmt_final_setup_LprFastAlg = $(bin)LprFastAlg_LprFastAlgsetup.make
cmt_local_LprFastAlg_makefile = $(bin)LprFastAlg.make
endif

else

ifdef READONLY
cmt_final_setup_LprFastAlg = /tmp/CMT_LprFastAlgsetup.make
cmt_local_LprFastAlg_makefile = /tmp/CMT_LprFastAlg$(cmt_lock_pid).make
else
cmt_final_setup_LprFastAlg = $(bin)LprFastAlgsetup.make
cmt_local_LprFastAlg_makefile = $(bin)LprFastAlg.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_LprFastAlgsetup.make
else
cmt_final_setup = $(bin)LprFastAlgsetup.make
endif

LprFastAlg ::


ifdef READONLY
LprFastAlg ::
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
	$(echo) 'LprFastAlg'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = LprFastAlg/
LprFastAlg::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------

#-- start of libary_header ---------------

LprFastAlglibname   = $(bin)$(library_prefix)LprFastAlg$(library_suffix)
LprFastAlglib       = $(LprFastAlglibname).a
LprFastAlgstamp     = $(bin)LprFastAlg.stamp
LprFastAlgshstamp   = $(bin)LprFastAlg.shstamp

LprFastAlg :: dirs  LprFastAlgLIB
	$(echo) "LprFastAlg ok"

#-- end of libary_header ----------------

LprFastAlgLIB :: $(LprFastAlglib) $(LprFastAlgshstamp)
	@/bin/echo "------> LprFastAlg : library ok"

$(LprFastAlglib) :: $(bin)basic.o $(bin)PKPiCheckAlg.o $(bin)Entries.o $(bin)Load.o
	$(lib_echo) library
	$(lib_silent) cd $(bin); \
	  $(ar) $(LprFastAlglib) $?
	$(lib_silent) $(ranlib) $(LprFastAlglib)
	$(lib_silent) cat /dev/null >$(LprFastAlgstamp)

#------------------------------------------------------------------
#  Future improvement? to empty the object files after
#  storing in the library
#
##	  for f in $?; do \
##	    rm $${f}; touch $${f}; \
##	  done
#------------------------------------------------------------------

$(LprFastAlglibname).$(shlibsuffix) :: $(LprFastAlglib) $(LprFastAlgstamps)
	$(lib_silent) cd $(bin); QUIET=$(QUIET); $(make_shlib) "$(tags)" LprFastAlg $(LprFastAlg_shlibflags)

$(LprFastAlgshstamp) :: $(LprFastAlglibname).$(shlibsuffix)
	@if test -f $(LprFastAlglibname).$(shlibsuffix) ; then cat /dev/null >$(LprFastAlgshstamp) ; fi

LprFastAlgclean ::
	$(cleanup_echo) objects
	$(cleanup_silent) cd $(bin); /bin/rm -f $(bin)basic.o $(bin)PKPiCheckAlg.o $(bin)Entries.o $(bin)Load.o

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
LprFastAlginstallname = $(library_prefix)LprFastAlg$(library_suffix).$(shlibsuffix)

LprFastAlg :: LprFastAlginstall

install :: LprFastAlginstall

LprFastAlginstall :: $(install_dir)/$(LprFastAlginstallname)
	@if test ! "${installarea}" = ""; then\
	  echo "installation done"; \
	fi

$(install_dir)/$(LprFastAlginstallname) :: $(bin)$(LprFastAlginstallname)
	@if test ! "${installarea}" = ""; then \
	  cd $(bin); \
	  if test ! "$(install_dir)" = ""; then \
	    if test ! -d "$(install_dir)"; then \
	      mkdir -p $(install_dir); \
	    fi ; \
	    if test -d "$(install_dir)"; then \
	      echo "Installing library $(LprFastAlginstallname) into $(install_dir)"; \
	      if test -e $(install_dir)/$(LprFastAlginstallname); then \
	        $(cmt_uninstall_area_command) $(install_dir)/$(LprFastAlginstallname); \
	        $(cmt_uninstall_area_command) $(install_dir)/$(LprFastAlginstallname).cmtref; \
	      fi; \
	      $(cmt_install_area_command) `pwd`/$(LprFastAlginstallname) $(install_dir)/$(LprFastAlginstallname); \
	      echo `pwd`/$(LprFastAlginstallname) >$(install_dir)/$(LprFastAlginstallname).cmtref; \
	    fi \
          else \
	    echo "Cannot install library $(LprFastAlginstallname), no installation directory specified"; \
	  fi; \
	fi

LprFastAlgclean :: LprFastAlguninstall

uninstall :: LprFastAlguninstall

LprFastAlguninstall ::
	@if test ! "${installarea}" = ""; then \
	  cd $(bin); \
	  if test ! "$(install_dir)" = ""; then \
	    if test -d "$(install_dir)"; then \
	      echo "Removing installed library $(LprFastAlginstallname) from $(install_dir)"; \
	      $(cmt_uninstall_area_command) $(install_dir)/$(LprFastAlginstallname); \
	      $(cmt_uninstall_area_command) $(install_dir)/$(LprFastAlginstallname).cmtref; \
	    fi \
          else \
	    echo "Cannot uninstall library $(LprFastAlginstallname), no installation directory specified"; \
	  fi; \
	fi




#-- start of dependency ------------------
ifneq ($(MAKECMDGOALS),LprFastAlgclean)

#$(bin)LprFastAlg_dependencies.make :: dirs

ifndef QUICK
$(bin)LprFastAlg_dependencies.make : ../src/basic.cxx ../src/PKPiCheckAlg.cxx ../src/components/Entries.cxx ../src/components/Load.cxx $(use_requirements) $(cmt_final_setup_LprFastAlg)
	$(echo) "(LprFastAlg.make) Rebuilding $@"; \
	  $(build_dependencies) LprFastAlg -all_sources -out=$@ ../src/basic.cxx ../src/PKPiCheckAlg.cxx ../src/components/Entries.cxx ../src/components/Load.cxx
endif

#$(LprFastAlg_dependencies)

-include $(bin)LprFastAlg_dependencies.make

endif
#-- end of dependency -------------------
#-- start of cpp_library -----------------

$(bin)LprFastAlg_dependencies.make : $(basic_cxx_dependencies)

$(bin)$(binobj)basic.o : $(basic_cxx_dependencies)
	$(cpp_echo) $(src)basic.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(LprFastAlg_pp_cppflags) $(lib_LprFastAlg_pp_cppflags) $(basic_pp_cppflags) $(use_cppflags) $(LprFastAlg_cppflags) $(lib_LprFastAlg_cppflags) $(basic_cppflags) $(basic_cxx_cppflags)  $(src)basic.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)LprFastAlg_dependencies.make : $(PKPiCheckAlg_cxx_dependencies)

$(bin)$(binobj)PKPiCheckAlg.o : $(PKPiCheckAlg_cxx_dependencies)
	$(cpp_echo) $(src)PKPiCheckAlg.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(LprFastAlg_pp_cppflags) $(lib_LprFastAlg_pp_cppflags) $(PKPiCheckAlg_pp_cppflags) $(use_cppflags) $(LprFastAlg_cppflags) $(lib_LprFastAlg_cppflags) $(PKPiCheckAlg_cppflags) $(PKPiCheckAlg_cxx_cppflags)  $(src)PKPiCheckAlg.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)LprFastAlg_dependencies.make : $(Entries_cxx_dependencies)

$(bin)$(binobj)Entries.o : $(Entries_cxx_dependencies)
	$(cpp_echo) $(src)components/Entries.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(LprFastAlg_pp_cppflags) $(lib_LprFastAlg_pp_cppflags) $(Entries_pp_cppflags) $(use_cppflags) $(LprFastAlg_cppflags) $(lib_LprFastAlg_cppflags) $(Entries_cppflags) $(Entries_cxx_cppflags) -I../src/components $(src)components/Entries.cxx

#-- end of cpp_library ------------------
#-- start of cpp_library -----------------

$(bin)LprFastAlg_dependencies.make : $(Load_cxx_dependencies)

$(bin)$(binobj)Load.o : $(Load_cxx_dependencies)
	$(cpp_echo) $(src)components/Load.cxx
	$(cpp_silent) $(cppcomp) -o $(@) $(use_pp_cppflags) $(LprFastAlg_pp_cppflags) $(lib_LprFastAlg_pp_cppflags) $(Load_pp_cppflags) $(use_cppflags) $(LprFastAlg_cppflags) $(lib_LprFastAlg_cppflags) $(Load_cppflags) $(Load_cxx_cppflags) -I../src/components $(src)components/Load.cxx

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: LprFastAlgclean
	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(LprFastAlg.make) $@: No rule for such target" >&2
#	@echo "#CMT> Warning: $@: No rule for such target" >&2; exit
else
.DEFAULT::
	$(echo) "(LprFastAlg.make) PEDANTIC: $@: No rule for such target" >&2
	if test $@ = "$(cmt_final_setup)" -o\
	 $@ = "$(cmt_final_setup_LprFastAlg)" ; then\
	 found=n; for s in 1 2 3 4 5; do\
	 sleep $$s; test ! -f $@ || { found=y; break; }\
	 done; if test $$found = n; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(LprFastAlg.make) PEDANTIC: $@: Seems to be missing. Ignore it for now" >&2; exit 0 ; fi;\
	 elif test `expr index $@ '/'` -ne 0 ; then\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(LprFastAlg.make) PEDANTIC: $@: Seems to be a missing file. Please check" >&2; exit 2 ; \
	 else\
	 test -z "$(cmtmsg)" ||\
	 echo "$(CMTMSGPREFIX)" "(LprFastAlg.make) PEDANTIC: $@: Seems to be a fake target due to some pattern. Just ignore it" >&2 ; exit 0; fi
endif

LprFastAlgclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library
	-$(cleanup_silent) cd $(bin); /bin/rm -f $(binobj)$(library_prefix)LprFastAlg$(library_suffix).a $(binobj)$(library_prefix)LprFastAlg$(library_suffix).s? $(binobj)LprFastAlg.stamp $(binobj)LprFastAlg.shstamp
#-- end of cleanup_library ---------------

