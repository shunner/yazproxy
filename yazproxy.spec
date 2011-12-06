Summary: yazproxy
Name: yazproxy
Version: 1.3.6
Release: 1indexdata
License: GPL
Group: Applications/Internet
Vendor: Index Data ApS <info@indexdata.dk>
Source: yazproxy-%{version}.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-root
BuildRequires: libyazpp4-devel
Packager: Adam Dickmeiss <adam@indexdata.dk>
URL: http://www.indexdata.com/yazproxy

%description
YAZ Proxy application

%package -n libyazproxy2
Summary: YAZ Proxy libraries
Group: Libraries
Requires: libyazpp4

%description -n libyazproxy2
YAZ Proxy libraries

%package -n libyazproxy2-devel
Summary: YAZ Proxy development libraries
Group: Development/Libraries
Requires: libyazproxy2 = %{version} libyazpp4-devel

%description -n libyazproxy2-devel
Development libraries and include files for the YAZ proxy.

%prep
%setup

%build

CFLAGS="$RPM_OPT_FLAGS" \
 ./configure --prefix=%{_prefix} --libdir=%{_libdir} --mandir=%{_mandir} \
	--enable-shared --with-yazpp=/usr/bin
make CFLAGS="$RPM_OPT_FLAGS"

%install
rm -fr ${RPM_BUILD_ROOT}
make prefix=${RPM_BUILD_ROOT}/%{_prefix} mandir=${RPM_BUILD_ROOT}/%{_mandir} \
	libdir=${RPM_BUILD_ROOT}/%{_libdir} install
rm ${RPM_BUILD_ROOT}/%{_libdir}/*.la

%clean
rm -fr ${RPM_BUILD_ROOT}

%files
%defattr(-,root,root)
%doc README LICENSE
%{_bindir}/yazproxy
%{_mandir}/man8/yazproxy.*
%{_datadir}/yazproxy
%{_datadir}/doc/yazproxy

%files -n libyazproxy2
%defattr(-,root,root)
%{_libdir}/*.so.*
%{_libdir}/yazproxy

%files -n libyazproxy2-devel
%defattr(-,root,root)
%{_includedir}/yazproxy
%{_libdir}/*.so
%{_libdir}/*.a
