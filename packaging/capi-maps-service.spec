Name:       capi-maps-service
Summary:    Tizen Maps Service API
Version:    0.5.1
Release:    1
Group:      Location/API
License:    Apache-2.0
Source0:    %{name}-%{version}.tar.gz

BuildRequires:  cmake
BuildRequires:  pkgconfig(glib-2.0)
BuildRequires:  pkgconfig(gmodule-2.0)
BuildRequires:  pkgconfig(capi-base-common)
BuildRequires:  pkgconfig(dlog)

#BuildRequires:  pkgconfig(security-privilege-checker)

Requires(post):  /sbin/ldconfig
Requires(postun):  /sbin/ldconfig
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
export CXXFLAGS="$CXXFLAGS -DTIZEN_ENGINEER_MODE"
export FFLAGS="$FFLAGS -DTIZEN_ENGINEER_MODE"

MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix} -DMAJORVER=${MAJORVER} -DFULLVER=%{version} -DLIBDIR=%{_libdir}
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
%{_includedir}/maps/maps_*plugin.h
%{_includedir}/maps/maps_extra_types.h


# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
# WARNING! FOR TESTING PURPOSES ONLY
# !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#if you want to tesst code, please change %if 0 to %if 1
%if 0
%package test
Summary:    Tizen Maps Service Library Test (Internal Dev)
Group:      Location/Testing
Requires:   capi-maps-service = %{version}-%{release}

%description test
This is a program to test the Tizen Maps Service Library internally.

==================================================================================
READ ME
-------
1. How to build
   1) Change %if 0 to %if 1
   2) Release SET(CMAKE_CXX_FLAGS, ~) and ADD_SUBDIRECTORY(test) in CMakeLists.txt

2. How to install
   # pkgcmd -i -t rpm -p capi-maps-service-test-%{version}-%{release}.rpm
==================================================================================

%files test
%defattr(-,root,root,-)
%manifest test/capi-maps-service-test.manifest
/opt/usr/devel/capi-maps-service-test
/usr/share/packages/capi-maps-service-test.xml

#################################################################################
# Test plugin library

%package -n maps-plugin-test
Summary:    Tizen Maps Service Plug-in Test (Internal Dev)
Group:      Location/Testing
Requires:   %{name} = %{version}-%{release}

%description -n maps-plugin-test
This is a program to test the Tizen Maps Service for Plug-in internally.

%files -n maps-plugin-test
%manifest test/dummy_plugin/maps-plugin-test.manifest
%defattr(-,root,root,-)
%{_libdir}/maps/plugins/libmaps-plugin-test.so*
%endif
