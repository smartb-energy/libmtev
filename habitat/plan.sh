pkg_name=libmtev
pkg_origin=smartb
pkg_version="master"
pkg_maintainer="smartB Engineering <dev@smartb.eu>"
pkg_license=('All Rights')
pkg_source="https://github.com/circonus-labs/${pkg_name}"
pkg_lib_dirs=(lib)
pkg_include_dirs=(include)
pkg_bin_dirs=(bin)
pkg_build_deps=(
  bixu/fq
  core/autoconf
  core/gcc
  core/git
  core/glibc
  core/libxml2
  core/libxslt
  core/lz4
  core/make
  core/ncurses
  core/openssl
  core/perl
  core/pkg-config
  core/util-linux
  core/zlib
  paytmlabs/hostname
  smartb/ck
  smartb/hwloc
  smartb/jlog
  smartb/libcircllhist/master
  smartb/libcircmetrics/master/20170303161519
  smartb/LuaJIT
  smartb/udns
)

do_verify() {
  return 0
}

do_download()
{
  return 0
}

do_clean()
{
  return 0
}

do_unpack()
{
  return 0
}

do_build()
{
  export CPPFLAGS="-D_REENTRANT $CPPFLAGS"
  export LDFLAGS="-Wl,-rpath=$(pkg_path_for core/ncurses)/lib -Wl,-rpath=$(pkg_path_for core/glibc)/lib -Wl,-rpath=$(pkg_path_for smartb/fq)/lib -Wl,-rpath=$(pkg_path_for core/util-linux)/lib $LDFLAGS"
  cd /src
  autoreconf -i
  ./configure --without-ssl --disable-amqp
  make
  return $?
}

do_install() {
  cp /src/src/scripts/mtev-config $pkg_prefix/bin/mtev-config
  chmod +x $pkg_prefix/bin/mtev-config
  return $?
}
