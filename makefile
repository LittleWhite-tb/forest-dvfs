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

	# Compile PythonForMIC
	cd offline/PythonForMIC; ./install.sh
	@echo "============================="
	@echo "Please now copy the offline/ folder to mic0 and use the following command: ./offline/usefulFreqs.py 0"

clean:
	$(MAKE) -e --file MakeFoREST clean

distclean:
	$(MAKE) -e --file MakeFoREST distclean

