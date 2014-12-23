include Makefile.common

all:	libraries

check:	libraries
	@echo Building and executing the unit tests...
	make -C checks

libraries:
	@echo Building libraries...
	$(MAKE) -C lib all

dependencies:
	@echo Making external provider components: openssl + donna...
	cd providers && ./build.sh

clean:	
	$(MAKE) -C checks clean
	$(MAKE) -C lib clean
