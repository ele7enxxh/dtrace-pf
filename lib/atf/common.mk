# $FreeBSD: release/10.1.0/lib/atf/common.mk 262849 2014-03-06 13:20:38Z jmmv $
#
# Common Makefile code for all components of ATF.
#

.if !defined(ATF)
.error "ATF must be defined and point to the contrib/atf directory"
.endif

# Depend on the atf-version target to generate a file that contains the
# version number of the currently imported ATF release and that only
# changes on new imports.
atf-version: atf-version-real
	@cmp -s atf-version atf-version-real \
	    || cp atf-version-real atf-version
atf-version-real: .PHONY
	@grep 'define VERSION' ${ATF}/bconfig.h \
	    | cut -d '"' -f 2 >atf-version-real
CLEANFILES+= atf-version atf-version-real
