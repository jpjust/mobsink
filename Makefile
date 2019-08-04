EXECUTABLE=mobsink
INSTDIR=/usr/bin
SHAREDIR=/usr/share
RESDIR=${SHAREDIR}/mobsink
DESKTOPDIR=${SHAREDIR}/applications

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
	mkdir -p ${RESDIR}/icons
	mkdir -p ${RESDIR}/png
	cp -a res/ico/* ${RESDIR}/icons/
	cp -a res/png/* ${RESDIR}/png/
	cp  ${BUILD}/${EXECUTABLE} ${INSTDIR}/
	cp mobsink.desktop ${DESKTOPDIR}/
	@echo 'Done! MobSink installed in ${INSTDIR}/${EXECUTABLE}'

uninstall:
	@echo 'Uninstalling...'
	rm -rf ${RESDIR}
	rm -rf ${DESKTOPDIR}/mobsink.desktop
	rm -rf ${INSTDIR}/${EXECUTABLE}
	@echo 'Done!'
