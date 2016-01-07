Name: fbzx
Version: 3.1.0
Release: 1
License: GPL3
Summary: A Sinclair ZX Spectrum emulator

BuildRequires: gcc
BuildRequires: gcc-c++
BuildRequires: glibc-headers
BuildRequires: cmake
BuildRequires: pkgconfig
BuildRequires: make
BuildRequires: SDL-devel
BuildRequires: pulseaudio-libs-devel

Requires: SDL
Requires: pulseaudio-libs
Requires: spectrum-roms

%description
A Sinclair ZX Spectrum emulator
.

%post

%clean
rm -rf %{buildroot}

%build
mkdir -p ${RPM_BUILD_DIR}
cd ${RPM_BUILD_DIR}; cmake -DCMAKE_INSTALL_PREFIX=/usr
make -C ${RPM_BUILD_DIR}

%postun

%files
/*

%install
make install -C ${RPM_BUILD_DIR} DESTDIR=%{buildroot}

