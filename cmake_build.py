import os.path
import platform
import re
import subprocess
import sys

from distutils.version import LooseVersion
from shutil import copyfile, copymode

from setuptools import Extension
from setuptools.command.build_ext import build_ext
import itertools

class CMakeBuildInfo:
    def __init__(self, cmake_base=None):
        self.setbase(cmake_base)
    @property
    def base(self):
        print("self.base is {}".format(self._cmake_base))

        return self._cmake_base

    def setbase(self, path):
        self._cmake_base=(path if isinstance(path,list) else list(os.path.split(path))) if path else None
        print("set base as {}".format(self._cmake_base))

    @base.setter
    def base(self, path):
        self.setbase(path)

    def entries(self):
        default = ['libcouchbase.so', 'libcouchbase.so.3', 'libcouchbase.so.3.0.1']
        return {'darwin': ['libcouchbase.2.dylib', 'libcouchbase.dylib'], 'linux': default,
                'windows': 'libcouchbase.dll'}.get(sys.platform.lower(), default)

    def lcb_build_base(self):
        print("self.base is {}".format(self.base))
        return self._cmake_base + ['install', 'lib']

    def lcb_dbg_pkg(self):
        return self.lcb_build_base() + ['Debug']

    def lcb_pkgs(self):
        return map(lambda x: self.lcb_dbg_pkg() + [x], self.entries())

    @property
    def lcb_pkgs_strlist(self):
        for x in self.lcb_pkgs():
            yield os.path.join(*x)



class CMakeExtension(Extension):
    def __init__(self, name, sourcedir='', **kw):
        Extension.__init__(self, name, **kw)
        self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
    hasrun = False
    hasbuilt = False
    #info = None
    hybrid = False

    #def __init__(self, *args, **kwargs):
    #    build_ext.__init__(self, *args, **kwargs)
    #    self.info=CMakeBuildInfo(self.build_temp)
    def run(self):
        if not CMakeBuild.hasrun:
            try:
                out = subprocess.check_output(['cmake', '--version'])
            except OSError:
                raise RuntimeError(
                    "CMake must be installed to build the following extensions: " +
                    ", ".join(e.name for e in self.extensions))

            if platform.system() == "Windows":
                cmake_version = LooseVersion(re.search(r'version\s*([\d.]+)',
                                                       out.decode()).group(1))
                if cmake_version < '3.1.0':
                    raise RuntimeError("CMake >= 3.1.0 is required on Windows")
            CMakeBuild.hasrun = True
            if CMakeBuild.hybrid:
                build_ext.run(self)
                #super(CMakeBuild, self).run()

            for ext in self.extensions:
                self.build_extension(ext)

    def build_extension(self, ext):
        if not CMakeBuild.hasbuilt:
            from distutils.sysconfig import get_python_inc
            import distutils.sysconfig as sysconfig
            AUTOBIND = os.environ.get('PYCBC_AUTOBIND', False)
            pycbc_lcb_api = os.environ.get('PYCBC_LCB_API', '0x030000')
            extdir = os.path.abspath(
                os.path.dirname(self.get_ext_fullpath(ext.name)))
            lcb_api_flags = ['-DPYCBC_LCB_API={}'.format(pycbc_lcb_api)]
            cmake_args = lcb_api_flags + ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY=' + extdir,
                                          '-DPYTHON_EXECUTABLE=' + sys.executable]
            cmake_args += ['-DPYTHON_INCLUDE_DIR={}'.format(get_python_inc())]
            self.info.setbase(self.build_temp)
            lib_path = os.path.join(*self.info.lcb_dbg_pkg())
            from distutils import sysconfig
            import os.path as op
            v = sysconfig.get_config_vars()
            print("LIBDIR {}, LIBPL {}".format(v.get("LIBDIR"), v.get("LIBPL")))
            fpaths = [op.join(v.get(pv, ''), v.get('LDLIBRARY', '')) for pv in ('LIBDIR', 'LIBPL')] + [os.path.normpath(
                os.path.join(get_python_inc(), "..", "..", "lib",
                             "libpython{}.dylib".format('.'.join(map(str, sys.version_info[0:2]))))),
                os.path.join(get_python_inc(), "..", "..", "lib")]
            python_lib = None
            python_libdir = None
            for entry in fpaths:
                if not op.exists(entry):
                    print("fpath {} does not exist".format(entry))
                    continue
                try:
                    print("got fpath {}:".format(entry))
                    if op.isfile(entry):
                        print("fpath {} is file, selecting".format(entry))
                        python_lib = python_lib or entry
                        continue
                    else:
                        entries = os.listdir(entry)
                        print("fpath {} is directory, contents {}".format(entry, entries))
                        for subentry in entries:
                            fullname = op.normpath(op.join(entry, subentry))
                            try:
                                fullname = op.readlink(fullname)
                            except:
                                pass
                            print("trying subentry:{}".format(fullname))

                            if op.exists(fullname):
                                # re.match(r".*python.*\.(so|dylib|a|dll|lib)",subentry):
                                python_lib = python_lib or fullname
                                python_libdir = op.normpath(entry)
                                print("got match {}, breaking out".format(fullname))
                                continue
                except:
                    pass
            cmake_args += ['-DHYBRID_BUILD=TRUE'] if CMakeBuild.hybrid else []
            cmake_args += ['-DPYTHON_LIBFILE={}'.format(python_lib)] if python_lib else []
            cmake_args += ['-DPYTHON_LIBDIR={}'.format(python_libdir)] if python_libdir else []
            cmake_args += [
                '-DPYTHON_VERSION_EXACT={}'.format('.'.join(map(str, sys.version_info[0:2])))] if python_libdir else []
            cfg = 'Debug' if self.debug else 'Release'
            build_args = ['--config', cfg]
            if platform.system() == "Windows":
                cmake_args += ['-DCMAKE_LIBRARY_OUTPUT_DIRECTORY_{}={}'.format(
                    cfg.upper(),
                    extdir), '-DLCB_NO_MOCK=1', '-DLCB_NO_SSL=1']
                if sys.maxsize > 2 ** 32:
                    cmake_args += ['-A', 'x64']
                build_args += ['--', '/m']
            else:
                cmake_args += ['-DCMAKE_BUILD_TYPE=' + cfg]
                build_args += ['--', '-j2']

            env = os.environ.copy()
            cmake_args += ['-DCMAKE_VERBOSE_MAKEFILE:BOOL=ON']
            env['CXXFLAGS'] = '{} {} -DVERSION_INFO=\\"{}\\"'.format(
                env.get('CXXFLAGS', ''), ' '.join(ext.extra_compile_args),
                self.distribution.get_version())

            env['CFLAGS'] = '{} {}'.format(
                env.get('CFLAGS', ''), ' '.join(ext.extra_compile_args),
                self.distribution.get_version())
            print("Launching build with env: {}, build_args: {}, cmake_args: {}".format(env, build_args, cmake_args))
            if not os.path.exists(self.build_temp):
                os.makedirs(self.build_temp)
            subprocess.check_call(['cmake', ext.sourcedir] + cmake_args, stdout=sys.stdout, stderr=sys.stdout,
                                  cwd=self.build_temp, env=env)
            subprocess.check_call(['cmake', '--build', '.'] + build_args,
                                  cwd=self.build_temp)
            if AUTOBIND:
                import autobind
                x = autobind.AutoBind()
                x()
            CMakeBuild.hasbuilt = True
            rpaths = [{'Darwin': '@loader_path', 'Linux': '$ORIGIN'}.get(platform.system(), "$ORIGIN"),
                      lib_path]
            print("adding rpaths {}".format(rpaths))
            if CMakeBuild.hybrid:
                if not self.compiler:
                    self.run()

                from distutils.ccompiler import CCompiler
                ext.extra_compile_args += lcb_api_flags
                compiler = self.compiler  # type: CCompiler
                compiler.add_include_dir(os.path.join(self.build_temp, "install", "include"))
                compiler.add_library_dir(os.path.join(self.build_temp, "install", "lib", "Debug"))
                for rpath in rpaths:
                    compiler.add_runtime_library_dir(rpath)
                    if platform.system() != 'Windows':
                        # ext.extra_link_args.append('-Wl,-rpath,.')
                        ext.extra_link_args.append('-Wl,-rpath,' + rpath)
                build_ext.build_extension(self, ext)
            build_dir = os.path.realpath(self.build_lib)

            for name in self.info.entries():
                try:
                    src = os.path.join(lib_path, name)
                    dest = os.path.join(build_dir, 'couchbase', name)
                    print("copying {} to {}".format(src, dest))
                    copyfile(src, dest)
                    print("success")
                except:
                    print("failure")
                    raise
                    pass

            self.info.pkgdata['couchbase'] = self.info.lcb_pkgs_strlist


    def copy_test_file(self, src_file):
        '''
        Copy ``src_file`` to ``dest_file`` ensuring parent directory exists.
        By default, message like `creating directory /path/to/package` and
        `copying directory /src/path/to/package -> path/to/package` are displayed on standard output. Adapted from scikit-build.
        '''
        # Create directory if needed
        dest_dir = os.path.join(os.path.dirname(
            os.path.abspath(__file__)), 'tests', 'bin')
        if dest_dir != "" and not os.path.exists(dest_dir):
            print("creating directory {}".format(dest_dir))
            os.makedirs(dest_dir)

        # Copy file
        dest_file = os.path.join(dest_dir, os.path.basename(src_file))
        print("copying {} -> {}".format(src_file, dest_file))
        copyfile(src_file, dest_file)
        copymode(src_file, dest_file)
