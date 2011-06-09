#/**
#*** INTEL CONFIDENTIAL
#*** 
#*** Copyright (October 2008) (October 2008) Intel Corporation All Rights Reserved. 
#*** The source code contained or described herein and all documents related to the
#*** source code ("Material") are owned by Intel Corporation or its suppliers or 
#*** licensors. Title to the Material remains with Intel Corporation or its 
#*** suppliers and licensors. The Material contains trade secrets and proprietary 
#*** and confidential information of Intel or its suppliers and licensors. 
#*** The Material is protected by worldwide copyright and trade secret laws 
#*** and treaty provisions. No part of the Material may be used, copied, 
#*** reproduced, modified, published, uploaded, posted, transmitted, distributed,
#*** or disclosed in any way without Intel's prior express written permission.
#***
#*** No license under any patent, copyright, trade secret or other intellectual
#*** property right is granted to or conferred upon you by disclosure or delivery
#*** of the Materials, either expressly, by implication, inducement, estoppel or
#*** otherwise. Any license under such intellectual property rights must be 
#*** express and approved by Intel in writing.
#**/

# @      @@@@@  @    @ @    @ @   @         @@@@@  @@@@@  @@@@@  @    @   @@    @@@@@ @@@@@@
# @        @    @@   @ @    @  @ @          @    @ @    @   @    @    @  @  @     @   @
# @        @    @ @  @ @    @   @           @    @ @    @   @    @    @ @    @    @   @
# @        @    @ @  @ @    @   @    @@@@@@ @@@@@  @@@@@    @    @    @ @    @    @   @@@@@@
# @        @    @  @ @ @    @   @           @      @  @     @     @   @ @@@@@@    @   @
# @        @    @   @@ @    @  @ @          @      @   @    @      @ @  @    @    @   @
# @@@@@@ @@@@@  @    @  @@@@  @   @         @      @    @ @@@@@     @   @    @    @   @@@@@@

#-----------------------------------------------------------------------------
# IEC SDK configuration.
#-----------------------------------------------------------------------------
#IECSDK_VERSION=debug
IECSDK_VERSION=release
#IECSDK_ADDRESSING=32
IECSDK_ADDRESSING=64

#-----------------------------------------------------------------------------
# IEC SDK tools.
#-----------------------------------------------------------------------------
IECSDK_CLEAR=/usr/bin/clear
IECSDK_CC=/usr/bin/gcc
IECSDK_CPP=/usr/bin/g++
IECSDK_LD=/usr/bin/ld
IECSDK_LL=/bin/ls -al
IECSDK_CD=(cd 
IECSDK_FILE=/usr/bin/file
IECSDK_LDD=/usr/bin/ldd
IECSDK_NM=/usr/bin/nm
IECSDK_LN=/bin/ln
IECSDK_LL=/bin/ls -al
IECSDK_GREP=/bin/grep
IECSDK_OBJDUMP=/usr/bin/objdump
IECSDK_STRIP=/usr/bin/strip
IECSDK_RM_F=/bin/rm -f
IECSDK_RM_RF=/bin/rm -rf
IECSDK_CP=/bin/cp
IECSDK_CP_R=/bin/cp -r
IECSDK_MV=/bin/mv
IECSDK_MKDIR=/bin/mkdir -p
IECSDK_TAR=/bin/tar
IECSDK_GZIP=/bin/gzip
IECSDK_CHMOD=/bin/chmod
IECSDK_MAKE=/usr/bin/make
IECSDK_AR=/usr/bin/ar
IECSDK_ECHO=/bin/echo
IECSDK_JAVAC=$(JAVA_PATH)/javac
IECSDK_JAVAH=$(JAVA_PATH)/javah

#-----------------------------------------------------------------------------
# IEC SDK build configuration.
#-----------------------------------------------------------------------------
IECSDK_SO_MAJOR_VERSION=1
IECSDK_SO_MINOR_VERSION=0
ifeq ($(IECSDK_ADDRESSING), 32)
	IECSDK_COMPILER_ADDRESSING=-m32
	ifeq ($(IECSDK_VERSION), debug)
		IECSDK_COMPILER_DEBUG_INFO=-g -D_DEBUG -D__PL_DEBUG__
		IECSDK_COMPILER_OPTIMIZATION=-O0 -msse
	else
		IECSDK_COMPILER_DEBUG_INFO=
		IECSDK_COMPILER_OPTIMIZATION=-O2 -msse
	endif
else
	IECSDK_COMPILER_ADDRESSING=-m64
	ifeq ($(IECSDK_VERSION), debug)
		IECSDK_COMPILER_DEBUG_INFO=-g -D_DEBUG -D__PL_DEBUG__
		IECSDK_COMPILER_OPTIMIZATION=-O0 -msse
	else
		IECSDK_COMPILER_DEBUG_INFO=
		IECSDK_COMPILER_OPTIMIZATION=-O2 -msse
	endif
endif

#-----------------------------------------------------------------------------
# API build configuration.
#-----------------------------------------------------------------------------
IECSDK_LIB_CFLAGS=\
	-D__PL_LINUX__ \
	$(IECSDK_COMPILER_ADDRESSING) \
	$(IECSDK_COMPILER_DEBUG_INFO) \
	$(IECSDK_COMPILER_OPTIMIZATION) \
	-Wall \
	-D_REENTRANT \
	-D_LIBC_REENTRANT \
	-pthread

#-----------------------------------------------------------------------------
# IECSDK targets.
#-----------------------------------------------------------------------------
IECSDK_PRIVATE_ESRV_LIB_TEST=esrv_lib_test

#-----------------------------------------------------------------------------
# Display available targets.
#-----------------------------------------------------------------------------
$(IECSDK_HELP):
	@$(IECSDK_CLEAR)
	@$(IECSDK_ECHO) "Makefile Targets:"
	@$(IECSDK_ECHO) "-----------------"
	@$(IECSDK_ECHO) ""
	@$(IECSDK_ECHO) "esrv_lib_test:..............Build The Energy Server Shared Library Test Program (DO NOT DISTRIBUTE)."
	@$(IECSDK_ECHO) ""
	@$(IECSDK_ECHO) "Notes:"
	@$(IECSDK_ECHO) "------"
	@$(IECSDK_ECHO) "   Use The IECSDK_VERSION Symbol To Build Debug (debug) Or Release (release) Binaries."
	@$(IECSDK_ECHO) "   Use The IECSDK_ADDRESSING Symbol To Build 32bit (32) Or 64bit (64) Binaries."
	@$(IECSDK_ECHO) "   By Default, IECSDK_VERSION=release and IECSDK_ADDRESSING=32 Are Used."
	@$(IECSDK_ECHO) ""

#-----------------------------------------------------------------------------
# Build The Energy Server Shared Library Test Program (DO NOT DISTRIBUTE).
#-----------------------------------------------------------------------------
$(IECSDK_PRIVATE_ESRV_LIB_TEST): $(IECSDK_PRIVATE_ESRV_LIB)
	@$(IECSDK_ECHO) "#-----------------------------------------------------------------------------"
	@$(IECSDK_ECHO) "# Build The Energy Server Shared Library Test Program (DO NOT DISTRIBUTE)."
	@$(IECSDK_ECHO) "#-----------------------------------------------------------------------------"
	$(IECSDK_CC) \
		$(IECSDK_LIB_CFLAGS) \
		esrv_lib_test.c \
		-o $(IECSDK_PRIVATE_ESRV_LIB_TEST) \
		-L. \
		esrv_lib.so
