XEN_ROOT = $(CURDIR)/../..
include $(XEN_ROOT)/tools/Rules.mk

MAJOR    = 4.11
MINOR    = 0

ifeq ($(CONFIG_LIBXC_MINIOS),y)
# Save/restore of a domain is currently incompatible with a stubdom environment
override CONFIG_MIGRATE := n
endif

CTRL_SRCS-y       :=
CTRL_SRCS-y       += xc_altp2m.c
CTRL_SRCS-y       += xc_core.c
CTRL_SRCS-$(CONFIG_X86) += xc_core_x86.c
CTRL_SRCS-$(CONFIG_ARM) += xc_core_arm.c
CTRL_SRCS-y       += xc_cpupool.c
CTRL_SRCS-y       += xc_domain.c
CTRL_SRCS-y       += xc_faultinjection.c
CTRL_SRCS-y       += xc_evtchn.c
CTRL_SRCS-y       += xc_gnttab.c
CTRL_SRCS-y       += xc_misc.c
CTRL_SRCS-y       += xc_flask.c
CTRL_SRCS-y       += xc_physdev.c
CTRL_SRCS-y       += xc_private.c
CTRL_SRCS-y       += xc_csched.c
CTRL_SRCS-y       += xc_csched2.c
CTRL_SRCS-y       += xc_arinc653.c
CTRL_SRCS-y       += xc_rt.c
CTRL_SRCS-y       += xc_tbuf.c
CTRL_SRCS-y       += xc_pm.c
CTRL_SRCS-y       += xc_cpu_hotplug.c
CTRL_SRCS-y       += xc_resume.c
CTRL_SRCS-y       += xc_tmem.c
CTRL_SRCS-y       += xc_vm_event.c
CTRL_SRCS-y       += xc_monitor.c
CTRL_SRCS-y       += xc_mem_paging.c
CTRL_SRCS-y       += xc_mem_access.c
CTRL_SRCS-y       += xc_memshr.c
CTRL_SRCS-y       += xc_hcall_buf.c
CTRL_SRCS-y       += xc_foreign_memory.c
CTRL_SRCS-y       += xc_kexec.c
CTRL_SRCS-y       += xc_resource.c
CTRL_SRCS-$(CONFIG_X86) += xc_psr.c
CTRL_SRCS-$(CONFIG_X86) += xc_pagetab.c
CTRL_SRCS-$(CONFIG_Linux) += xc_linux.c
CTRL_SRCS-$(CONFIG_FreeBSD) += xc_freebsd.c
CTRL_SRCS-$(CONFIG_SunOS) += xc_solaris.c
CTRL_SRCS-$(CONFIG_NetBSD) += xc_netbsd.c
CTRL_SRCS-$(CONFIG_NetBSDRump) += xc_netbsd.c
CTRL_SRCS-$(CONFIG_MiniOS) += xc_minios.c
CTRL_SRCS-y       += xc_evtchn_compat.c
CTRL_SRCS-y       += xc_gnttab_compat.c
CTRL_SRCS-y       += xc_devicemodel_compat.c

GUEST_SRCS-y :=
GUEST_SRCS-y += xg_private.c xc_suspend.c
ifeq ($(CONFIG_MIGRATE),y)
GUEST_SRCS-y += xc_sr_common.c
GUEST_SRCS-$(CONFIG_X86) += xc_sr_common_x86.c
GUEST_SRCS-$(CONFIG_X86) += xc_sr_common_x86_pv.c
GUEST_SRCS-$(CONFIG_X86) += xc_sr_restore_x86_pv.c
GUEST_SRCS-$(CONFIG_X86) += xc_sr_restore_x86_hvm.c
GUEST_SRCS-$(CONFIG_X86) += xc_sr_save_x86_pv.c
GUEST_SRCS-$(CONFIG_X86) += xc_sr_save_x86_hvm.c
GUEST_SRCS-y += xc_sr_restore.c
GUEST_SRCS-y += xc_sr_save.c
GUEST_SRCS-y += xc_offline_page.c xc_compression.c
else
GUEST_SRCS-y += xc_nomigrate.c
endif

vpath %.c ../../xen/common/libelf
CFLAGS += -I../../xen/common/libelf

ELF_SRCS-y += libelf-tools.c libelf-loader.c
ELF_SRCS-y += libelf-dominfo.c

GUEST_SRCS-y += $(ELF_SRCS-y)

$(patsubst %.c,%.o,$(ELF_SRCS-y)): CFLAGS += -Wno-pointer-sign
$(patsubst %.c,%.opic,$(ELF_SRCS-y)): CFLAGS += -Wno-pointer-sign

# new domain builder
GUEST_SRCS-y                 += xc_dom_core.c xc_dom_boot.c
GUEST_SRCS-y                 += xc_dom_elfloader.c
GUEST_SRCS-$(CONFIG_X86)     += xc_dom_bzimageloader.c
GUEST_SRCS-$(CONFIG_X86)     += xc_dom_decompress_lz4.c
GUEST_SRCS-$(CONFIG_X86)     += xc_dom_hvmloader.c
GUEST_SRCS-$(CONFIG_ARM)     += xc_dom_armzimageloader.c
GUEST_SRCS-y                 += xc_dom_binloader.c
GUEST_SRCS-y                 += xc_dom_compat_linux.c

GUEST_SRCS-$(CONFIG_X86)     += xc_dom_x86.c
GUEST_SRCS-$(CONFIG_X86)     += xc_cpuid_x86.c
GUEST_SRCS-$(CONFIG_ARM)     += xc_dom_arm.c

ifeq ($(CONFIG_LIBXC_MINIOS),y)
GUEST_SRCS-y                 += xc_dom_decompress_unsafe.c
GUEST_SRCS-y                 += xc_dom_decompress_unsafe_bzip2.c
GUEST_SRCS-y                 += xc_dom_decompress_unsafe_lzma.c
GUEST_SRCS-y                 += xc_dom_decompress_unsafe_lzo1x.c
GUEST_SRCS-y                 += xc_dom_decompress_unsafe_xz.c
endif

-include $(XEN_TARGET_ARCH)/Makefile

CFLAGS   += -Werror -Wmissing-prototypes
CFLAGS   += -I. -I./include $(CFLAGS_xeninclude)
CFLAGS   += -D__XEN_TOOLS__

# Needed for posix_fadvise64() in xc_linux.c
CFLAGS-$(CONFIG_Linux) += -D_GNU_SOURCE

CFLAGS	+= $(PTHREAD_CFLAGS)
CFLAGS	+= $(CFLAGS_libxentoollog)
CFLAGS	+= $(CFLAGS_libxenevtchn)
CFLAGS	+= $(CFLAGS_libxendevicemodel)

CTRL_LIB_OBJS := $(patsubst %.c,%.o,$(CTRL_SRCS-y))
CTRL_PIC_OBJS := $(patsubst %.c,%.opic,$(CTRL_SRCS-y))

GUEST_LIB_OBJS := $(patsubst %.c,%.o,$(GUEST_SRCS-y))
GUEST_PIC_OBJS := $(patsubst %.c,%.opic,$(GUEST_SRCS-y))

$(CTRL_LIB_OBJS) $(GUEST_LIB_OBJS) \
$(CTRL_PIC_OBJS) $(GUEST_PIC_OBJS): CFLAGS += -include $(XEN_ROOT)/tools/config.h

# libxenguest includes xc_private.h, so needs this despite not using
# this functionality directly.
$(CTRL_LIB_OBJS) $(GUEST_LIB_OBJS) \
$(CTRL_PIC_OBJS) $(GUEST_PIC_OBJS): CFLAGS += $(CFLAGS_libxencall) $(CFLAGS_libxenforeignmemory)

$(CTRL_LIB_OBJS) $(CTRL_PIC_OBJS): CFLAGS += $(CFLAGS_libxengnttab)

LIB := libxenctrl.a
ifneq ($(nosharedlibs),y)
LIB += libxenctrl.so libxenctrl.so.$(MAJOR) libxenctrl.so.$(MAJOR).$(MINOR)
endif

LIB += libxenguest.a
ifneq ($(nosharedlibs),y)
LIB += libxenguest.so libxenguest.so.$(MAJOR) libxenguest.so.$(MAJOR).$(MINOR)
endif

genpath-target = $(call buildmakevars2header,_paths.h)
$(eval $(genpath-target))

xc_private.h: _paths.h

ifeq ($(CONFIG_X86),y)

_xc_cpuid_autogen.h: $(XEN_ROOT)/xen/include/public/arch-x86/cpufeatureset.h $(XEN_ROOT)/xen/tools/gen-cpuid.py
	$(PYTHON) $(XEN_ROOT)/xen/tools/gen-cpuid.py -i $^ -o $@.new
	$(call move-if-changed,$@.new,$@)

build: _xc_cpuid_autogen.h
endif

$(CTRL_LIB_OBJS) $(GUEST_LIB_OBJS) \
$(CTRL_PIC_OBJS) $(GUEST_PIC_OBJS): xc_private.h

PKG_CONFIG := xencontrol.pc xenguest.pc
PKG_CONFIG_VERSION := $(MAJOR).$(MINOR)

ifneq ($(CONFIG_LIBXC_MINIOS),y)
PKG_CONFIG_INST := $(PKG_CONFIG)
$(PKG_CONFIG_INST): PKG_CONFIG_PREFIX = $(prefix)
$(PKG_CONFIG_INST): PKG_CONFIG_INCDIR = $(includedir)
$(PKG_CONFIG_INST): PKG_CONFIG_LIBDIR = $(libdir)
endif

PKG_CONFIG_LOCAL := $(foreach pc,$(PKG_CONFIG),$(PKG_CONFIG_DIR)/$(pc))

$(PKG_CONFIG_LOCAL): PKG_CONFIG_PREFIX = $(XEN_ROOT)
$(PKG_CONFIG_LOCAL): PKG_CONFIG_INCDIR = $(XEN_LIBXC)/include
$(PKG_CONFIG_LOCAL): PKG_CONFIG_LIBDIR = $(CURDIR)
$(PKG_CONFIG_LOCAL): PKG_CONFIG_CFLAGS_LOCAL = $(CFLAGS_xeninclude)

.PHONY: all
all: build

.PHONY: build
build:
	$(MAKE) libs

.PHONY: libs
libs: $(LIB) $(PKG_CONFIG_INST) $(PKG_CONFIG_LOCAL)

.PHONY: install
install: build
	$(INSTALL_DIR) $(DESTDIR)$(libdir)
	$(INSTALL_DIR) $(DESTDIR)$(includedir)
	$(INSTALL_SHLIB) libxenctrl.so.$(MAJOR).$(MINOR) $(DESTDIR)$(libdir)
	$(INSTALL_DATA) libxenctrl.a $(DESTDIR)$(libdir)
	$(SYMLINK_SHLIB) libxenctrl.so.$(MAJOR).$(MINOR) $(DESTDIR)$(libdir)/libxenctrl.so.$(MAJOR)
	$(SYMLINK_SHLIB) libxenctrl.so.$(MAJOR) $(DESTDIR)$(libdir)/libxenctrl.so
	$(INSTALL_DATA) include/xenctrl.h include/xenctrl_compat.h $(DESTDIR)$(includedir)
	$(INSTALL_SHLIB) libxenguest.so.$(MAJOR).$(MINOR) $(DESTDIR)$(libdir)
	$(INSTALL_DATA) libxenguest.a $(DESTDIR)$(libdir)
	$(SYMLINK_SHLIB) libxenguest.so.$(MAJOR).$(MINOR) $(DESTDIR)$(libdir)/libxenguest.so.$(MAJOR)
	$(SYMLINK_SHLIB) libxenguest.so.$(MAJOR) $(DESTDIR)$(libdir)/libxenguest.so
	$(INSTALL_DATA) include/xenguest.h $(DESTDIR)$(includedir)
	$(INSTALL_DATA) xencontrol.pc $(DESTDIR)$(PKG_INSTALLDIR)
	$(INSTALL_DATA) xenguest.pc $(DESTDIR)$(PKG_INSTALLDIR)

.PHONY: uninstall
uninstall:
	rm -f $(DESTDIR)$(PKG_INSTALLDIR)/xenguest.pc
	rm -f $(DESTDIR)$(includedir)/xenguest.h
	rm -f $(DESTDIR)$(libdir)/libxenguest.so
	rm -f $(DESTDIR)$(libdir)/libxenguest.so.$(MAJOR)
	rm -f $(DESTDIR)$(libdir)/libxenguest.so.$(MAJOR).$(MINOR)
	rm -f $(DESTDIR)$(libdir)/libxenguest.a
	rm -f $(DESTDIR)$(PKG_INSTALLDIR)/xencontrol.pc
	rm -f $(DESTDIR)$(includedir)/xenctrl.h
	rm -f $(DESTDIR)$(includedir)/xenctrl_compat.h
	rm -f $(DESTDIR)$(libdir)/libxenctrl.so
	rm -f $(DESTDIR)$(libdir)/libxenctrl.so.$(MAJOR)
	rm -f $(DESTDIR)$(libdir)/libxenctrl.so.$(MAJOR).$(MINOR)
	rm -f $(DESTDIR)$(libdir)/libxenctrl.a

.PHONY: TAGS
TAGS:
	etags -t *.c *.h

.PHONY: clean
clean:
	rm -rf *.rpm $(LIB) *~ $(DEPS_RM) \
            _paths.h \
	    xencontrol.pc xenguest.pc \
            $(CTRL_LIB_OBJS) $(CTRL_PIC_OBJS) \
            $(GUEST_LIB_OBJS) $(GUEST_PIC_OBJS)

.PHONY: distclean
distclean: clean

.PHONY: rpm
rpm: build
	rm -rf staging
	mkdir staging
	mkdir staging/i386
	rpmbuild --define "staging$$PWD/staging" --define '_builddir.' \
		--define "_rpmdir$$PWD/staging" -bb rpm.spec
	mv staging/i386/*.rpm .
	rm -rf staging

# libxenctrl

libxenctrl.a: $(CTRL_LIB_OBJS)
	$(AR) rc $@ $^

libxenctrl.so: libxenctrl.so.$(MAJOR)
	$(SYMLINK_SHLIB) $< $@
libxenctrl.so.$(MAJOR): libxenctrl.so.$(MAJOR).$(MINOR)
	$(SYMLINK_SHLIB) $< $@

libxenctrl.so.$(MAJOR).$(MINOR): $(CTRL_PIC_OBJS)
	$(CC) $(LDFLAGS) $(PTHREAD_LDFLAGS) -Wl,$(SONAME_LDFLAG) -Wl,libxenctrl.so.$(MAJOR) $(SHLIB_LDFLAGS) -o $@ $^ $(LDLIBS_libxentoollog) $(LDLIBS_libxenevtchn) $(LDLIBS_libxengnttab) $(LDLIBS_libxencall) $(LDLIBS_libxenforeignmemory) $(LDLIBS_libxendevicemodel) $(PTHREAD_LIBS) $(APPEND_LDFLAGS)

# libxenguest

libxenguest.a: $(GUEST_LIB_OBJS)
	$(AR) rc $@ $^

libxenguest.so: libxenguest.so.$(MAJOR)
	$(SYMLINK_SHLIB) $< $@
libxenguest.so.$(MAJOR): libxenguest.so.$(MAJOR).$(MINOR)
	$(SYMLINK_SHLIB) $< $@

ifeq ($(CONFIG_MiniOS),y)
zlib-options =
else
zlib-options = $(ZLIB)
endif

xc_dom_bzimageloader.o: CFLAGS += $(filter -D%,$(zlib-options))
xc_dom_bzimageloader.opic: CFLAGS += $(filter -D%,$(zlib-options))

libxenguest.so.$(MAJOR).$(MINOR): COMPRESSION_LIBS = $(filter -l%,$(zlib-options))
libxenguest.so.$(MAJOR).$(MINOR): $(GUEST_PIC_OBJS) libxenctrl.so
	$(CC) $(LDFLAGS) -Wl,$(SONAME_LDFLAG) -Wl,libxenguest.so.$(MAJOR) $(SHLIB_LDFLAGS) -o $@ $(GUEST_PIC_OBJS) $(COMPRESSION_LIBS) -lz $(LDLIBS_libxenevtchn) $(LDLIBS_libxenctrl) $(PTHREAD_LIBS) $(APPEND_LDFLAGS)

-include $(DEPS_INCLUDE)

