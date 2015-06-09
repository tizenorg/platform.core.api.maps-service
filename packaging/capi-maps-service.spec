Name:       capi-maps-service
Summary:    Tizen Maps Library
Version:    0.3.2
Release:    1
Group:      Location/Maps Service
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz

BuildRequires:  cmake
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(gmodule-2.0)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(dlog)
BuildRequires:  pkgconfig(pkgmgr-info)
BuildRequires:  pkgconfig(privacy-manager-client)
Requires(post):  /sbin/ldconfig
Requires(postun):  /sbin/ldconfig
Provides: capi-maps-service-plugin-devel

%ifarch %{arm}
%define ARCH arm
%else
%define ARCH i586
%endif

%description
Tizen Maps Library

%prep
%setup -q

%build
export CFLAGS="$CFLAGS -DTIZEN_ENGINEER_MODE"
export CXXFLAGS="$CXXFLAGS -DTIZEN_ENGINEER_MODE"
export FFLAGS="$FFLAGS -DTIZEN_ENGINEER_MODE"

MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix} -DMAJORVER=${MAJORVER} -DFULLVER=%{version}
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

mkdir -p %{buildroot}/usr/share/license
cp LICENSE %{buildroot}/usr/share/license/%{name}

%post
/sbin/ldconfig

%postun
/sbin/ldconfig

%files
%manifest capi-maps-service.manifest
%defattr(-,root,root,-)
/usr/share/license/capi-maps-service
/usr/lib/libcapi-maps-service.so.*


%package devel
Summary:    Tizen Maps Library (Development)
Group:      Framework/maps
Requires:	%{name} = %{version}-%{release}

%description devel
Tizen Maps Library (Development)

%post devel
/sbin/ldconfig

%postun devel
/sbin/ldconfig

%files devel
%defattr(-,root,root,-)
%{_includedir}/maps/maps_*.h
%{_libdir}/pkgconfig/capi-maps-service.pc
/usr/lib/libcapi-maps-service.so
%exclude %{_includedir}/maps/maps_plugin*.h
%exclude %{_includedir}/maps/maps_*_plugin.h


%package plugin-devel
Summary:    Tizen Maps Library for Maps Service Plugin(Development)
Group:      Framework/maps
Requires:	%{name} = %{version}-%{release}

%description plugin-devel
Tizen Maps Library for Maps Service Plugin(Development)

%post plugin-devel
/sbin/ldconfig

%postun plugin-devel
/sbin/ldconfig

%files plugin-devel
%defattr(-,root,root,-)
%{_includedir}/maps/maps_plugin*.h
%{_includedir}/maps/maps_*plugin.h


### Some API signatures are changed
#%if 0
%package test
Summary:    Tizen Maps Library (Internal Dev)
Group:      Framework/maps
Requires:	capi-maps-service = %{version}-%{release}

%description test
Tizen Maps Library (Internal Dev)

%files test
%manifest test/maps-service-test.manifest
/etc/smack/accesses2.d/maps-service-test.rule
%{_bindir}/capi-maps-service-test

#################################################################################
# Test plugin library

%package -n maps-plugin-test
Summary:    Tizen Maps Test Plugin (Internal Dev)
Group:      Framework/maps
Requires:   %{name} = %{version}-%{release}

%description -n maps-plugin-test
Tizen Maps Test Plugin (Internal Dev)

%files -n maps-plugin-test
%manifest test/dummy_plugin/maps-plugin-test.manifest
%defattr(-,root,root,-)
%{_libdir}/maps/plugins/libmaps-plugin-test.so*

#%endif
