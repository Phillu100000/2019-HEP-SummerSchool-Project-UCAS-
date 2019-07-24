#-- start of make_header -----------------

#====================================
#  Document install_includes
#
#   Generated Fri Oct 14 20:12:03 2011  by zhangr
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_install_includes_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_install_includes_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_install_includes

McDecayModeSvc_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_install_includes = /tmp/CMT_$(McDecayModeSvc_tag)_install_includes.make$(cmt_lock_pid)
else
cmt_local_tagfile_install_includes = $(McDecayModeSvc_tag)_install_includes.make
endif

else

tags      = $(tag),$(CMTEXTRATAGS)

McDecayModeSvc_tag = $(tag)

ifdef READONLY
cmt_local_tagfile_install_includes = /tmp/CMT_$(McDecayModeSvc_tag).make$(cmt_lock_pid)
else
cmt_local_tagfile_install_includes = $(McDecayModeSvc_tag).make
endif

endif

-include $(cmt_local_tagfile_install_includes)

ifdef cmt_install_includes_has_target_tag

ifdef READONLY
cmt_final_setup_install_includes = /tmp/CMT_McDecayModeSvc_install_includessetup.make
cmt_local_install_includes_makefile = /tmp/CMT_install_includes$(cmt_lock_pid).make
else
cmt_final_setup_install_includes = $(bin)McDecayModeSvc_install_includessetup.make
cmt_local_install_includes_makefile = $(bin)install_includes.make
endif

else

ifdef READONLY
cmt_final_setup_install_includes = /tmp/CMT_McDecayModeSvcsetup.make
cmt_local_install_includes_makefile = /tmp/CMT_install_includes$(cmt_lock_pid).make
else
cmt_final_setup_install_includes = $(bin)McDecayModeSvcsetup.make
cmt_local_install_includes_makefile = $(bin)install_includes.make
endif

endif

ifdef READONLY
cmt_final_setup = /tmp/CMT_McDecayModeSvcsetup.make
else
cmt_final_setup = $(bin)McDecayModeSvcsetup.make
endif

install_includes ::


ifdef READONLY
install_includes ::
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
	@echo 'install_includes'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = install_includes/
install_includes::
	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
	@echo "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

#-- end of make_header ------------------


#
#  We want to install all header files that follow the standard convention
#
#    ../<package>
#
#  into
#
#    ${CMTINSTALLAREA}/include/<package>/<package>
#
#  (with two levels of <package> directory)
#

ifeq ($(INSTALLAREA),)
installarea = $(CMTINSTALLAREA)
else
ifeq ($(findstring `,$(INSTALLAREA)),`)
installarea = $(shell $(subst `,, $(INSTALLAREA)))
else
installarea = $(INSTALLAREA)
endif
endif

install_include_dir = $(installarea)/include/$(package)

install_includes :: install_includesinstall

install :: install_includesinstall

install_includesinstall :: $(install_include_dir)

$(install_include_dir) ::
	@if test "$(install_include_dir)" = ""; then \
	  echo "Cannot install header files, no installation directory specified"; \
	else \
	  if test -d ../${package}; then \
	    here=`(cd ../${package}; pwd)`; \
	    need_new=yes; \
	    if test -L $(install_include_dir) ; then rm -f $(install_include_dir); fi; \
	    if test ! -d $(install_include_dir) ; then mkdir -p $(install_include_dir); fi; \
	    if test -L $(install_include_dir)/$(package); then \
	      eval d=$(install_include_dir)/$(package); \
	      dd=`ls -l $${d} | sed -e 's#.*[-][>]##'`; \
	      if test ! $${dd} = $${here}; then \
	        eval rm -Rf $(install_include_dir)/$(package) $(install_include_dir)/$(package).cmtref ; \
	      else \
	        need_new=no ; \
	      fi; \
	    fi; \
	    if test "$${need_new}" = "yes"; then \
	      echo "Installing files from standard ../${package} to $(install_include_dir)"; \
              eval ln -s $${here} $(install_include_dir)/$(package); \
              echo $${here} >|$(install_include_dir)/$(package).cmtref; \
	    else \
	      echo "Files from standard ../${package} already installed"; \
	    fi; \
	  else \
	    echo "No standard include file area"; \
	  fi; \
	fi

##	    (cd ../${package}; eval ln -s $(install_include_filter) $(install_include_dir));


##install_includesclean :: install_includesuninstall

uninstall :: install_includesuninstall

install_includesuninstall ::
	@if test "$(install_include_dir)" = ""; then \
	  echo "Cannot uninstall header files, no installation directory specified"; \
	else \
	  if test -d $(install_include_dir) ; then \
	    echo "Uninstalling files from $(install_include_dir)"; \
	    eval rm -rf $(install_include_dir) ; \
	  fi \
	fi


#-- start of cleanup_header --------------

clean :: install_includesclean
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

install_includesclean ::
#-- end of cleanup_header ---------------
