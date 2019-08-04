EXECUTABLE=mobsink
INSTDIR=/usr/bin
SHAREDIR=/usr/share/mobsink

ifneq (${BUILD}, Debug)
	BUILD=Release
endif

all:
	@echo 'Making a ${BUILD} build...'
	cd ${BUILD} ; make
	@echo 'Done! Run "make install" to install MobSink.'

clean:
	@echo 'Cleaning ${BUILD} build...'
	cd ${BUILD} ; make clean
	@echo 'Done!'

install:
	@echo 'Installing a ${BUILD} build...'
	mkdir -p ${SHAREDIR}/icons
	mkdir -p ${SHAREDIR}/png
	cp -a res/ico/* ${SHAREDIR}/icons/
	cp -a res/png/* ${SHAREDIR}/png/
	cp  ${BUILD}/${EXECUTABLE} ${INSTDIR}/
	@echo 'Done! MobSink installed in ${INSTDIR}/${EXECUTABLE}'

uninstall:
	@echo 'Uninstalling...'
	rm -rf ${SHAREDIR}
	rm -rf ${INSTDIR}/${EXECUTABLE}
	@echo 'Done!'
