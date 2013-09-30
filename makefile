CFLAGS=-Wall -O3 -DREST_LOG -msse4

all: x86

x86:
	$(MAKE) --file MakeFoREST release \
		MCXXFLAGS="-O3 -Wall -Werror -DARCH_X86 -DNDEBUG" \
		CC="gcc" \
		CXX="g++" \
		GLOG_CONFIGURE=""
	# Build lPowerProbe and offline benchmark
	$(MAKE) --file MakeFoREST offline \
		CXX="g++" \
		CC="gcc"
	
	# run offline script from make
	$(MAKE) --file MakeFoREST offline-run

mic:		
	$(MAKE) --file MakeFoREST release \
		CXX="icpc -mmic" \
		CC="icc -mmic" \
		MCXXFLAGS="-O3 -DARCH_MIC -DNDEBUG" \
		GLOG_CONFIGURE="CC=icc CXX=icpc CXXFLAGS=-mmic LDFLAGS=-mmic CFLAGS=-mmic --host --host-alias x86_64"
	
	# Build lPowerProbe and offline benchmark
	$(MAKE) --file MakeFoREST offline \
		CXX="icpc -mmic" \
		CC="icc -mmic"

	$(MAKE) -C offline offline

	# Build MIC library
	$(MAKE) -C offline/lPowerProbe mic-libs

clean:
	$(MAKE) -e --file MakeFoREST clean

distclean:
	$(MAKE) -e --file MakeFoREST distclean


