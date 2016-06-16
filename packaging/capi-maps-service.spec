Name:       capi-maps-service
Summary:    Tizen Maps Service API
Version:    0.6.2
Release:    1
Group:      Location/API
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz

BuildRequires:  cmake

# Maps API dependencies
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(gmodule-2.0)
BuildRequires: pkgconfig(capi-base-common)
BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(capi-system-info)

# Privilege check for Tizen 2.3.2
#BuildRequires: pkgconfig(capi-appfw-app-manager)
#BuildRequires: pkgconfig(pkgmgr-info)
#BuildRequires: pkgconfig(capi-security-privilege-manager)

# Privilege check for Tizen 2.4
#BuildRequires: pkgconfig(security-privilege-checker)

# Mapping API dependencies
BuildRequires: pkgconfig(eina)
BuildRequires: pkgconfig(evas)
BuildRequires: pkgconfig(ecore)
BuildRequires: pkgconfig(cairo)
BuildRequires: pkgconfig(elementary)
BuildRequires: pkgconfig(capi-media-image-util)

Requires(post): /sbin/ldconfig
Requires(postun): /sbin/ldconfig
Provides: capi-maps-service-plugin-devel

%ifarch %{arm}
%define ARCH arm
%else
%define ARCH i586
%endif

%description
This provides the Tizen Map Service API to access and handle the map data.

%prep
%setup -q

%build
export CFLAGS="$CFLAGS -DTIZEN_ENGINEER_MODE"
export CXXFLAGS="$CXXFLAGS -DTIZEN_ENGINEER_MODE -fprofile-arcs -ftest-coverage -lgcov"
export FFLAGS="$FFLAGS -DTIZEN_ENGINEER_MODE"
export LDFLAGS="$FFLAGS -DTIZEN_ENGINEER_MODE -lgcov"

MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix} -DMAJORVER=${MAJORVER} -DFULLVER=%{version} \
	-DLIBDIR=%{_libdir} -DPROFILE=%{?profile} -DTIZEN_VER=300
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

mkdir -p %{buildroot}/usr/share/license
cp LICENSE %{buildroot}/usr/share/license/%{name}

%post
mkdir -p %{_libdir}/maps/plugins
# privilege mapping for Tizen 3.x
chgrp priv_mapservice %{_libdir}/maps/plugins
chmod g+rw %{_libdir}/maps/plugins
chmod o= %{_libdir}/maps/plugins
chsmack -a "*" %{_libdir}/maps/plugins
/sbin/ldconfig

%postun
/sbin/ldconfig

%files
%manifest capi-maps-service.manifest
%defattr(-,root,root,-)
%{_datadir}/license/%{name}
%{_libdir}/libcapi-maps-service.so.*


%package devel
Summary:    Tizen Maps Service Library (Development)
Group:      Location/Libraries
Requires:  %{name} = %{version}-%{release}

%description devel
This provides the Tizen Maps Service Library to access and handle the map data. (Development)

%post devel
/sbin/ldconfig

%postun devel
/sbin/ldconfig

%files devel
%defattr(-,root,root,-)
%{_includedir}/maps/maps_*.h
%{_libdir}/pkgconfig/capi-maps-service.pc
%{_libdir}/libcapi-maps-service.so
%exclude %{_includedir}/maps/maps_plugin*.h
%exclude %{_includedir}/maps/maps_*_plugin.h
%exclude %{_includedir}/maps/maps_extra_types.h


%package plugin-devel
Summary:    Tizen Maps Library for the Maps Service Plug-in (Development)
Group:      Location/Libraries
Requires:  %{name} = %{version}-%{release}

%description plugin-devel
This provides the Tizen Map Service APIs to access and handle map data for the Maps Service Plug-in. (Development)

%post plugin-devel
/sbin/ldconfig

%postun plugin-devel
/sbin/ldconfig

%files plugin-devel
%defattr(-,root,root,-)
%{_includedir}/maps/maps_plugin*.h
%{_includedir}/maps/maps_*_plugin.h
%{_includedir}/maps/maps_extra_types.h
