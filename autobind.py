import os.path

import logging
import traceback
import couchbase_v2.tests.base
CPPYY=os.getenv("PYCBC_CPPYY")
loglevel=os.environ.get("PYCBC_DEBUG_LOG") or logging.INFO
if loglevel:
    ch = logging.StreamHandler()
    ch.setLevel(logging.getLevelName(loglevel))
    logging.getLogger().addHandler(ch)
class AutoBind(object):
    def dump_declarations(decls,dump, depth=0):
        indent=' '*(depth*4)
        dump.write("{}declarations:[".format(indent))
        depth+=1
        indent=' '*(depth*4)
        for declar in decls:
            if not hasattr(declar,"declarations"):
                continue
            for subdecl in declar.declarations:
                # builder.decl()
                items = []
                items.append("{}{}".format(indent,str(subdecl)) + '\n')
                dump.writelines(items)
                if hasattr(subdecl,"declarations"):
                    AutoBind.dump_declarations(subdecl.declarations,dump,depth)
        depth-=1
        indent=' '*(depth*4)
        dump.write("{}]".format(indent))

    def __call__(self, *args, **kwargs):
        try:
            logging.error("commencing autobind")
            from distutils.sysconfig import get_python_inc
            std_inc = get_python_inc()

            elements = os.path.split(std_inc)
            python_install_base = list(elements[:-1]) + ["site", elements[-1], "couchbase", "cmake_build_release"]
            base_inc = os.path.join(*python_install_base)


            def add_cxx_includes(inc):
                cxx_incs = get_repo(inc)
                add_include(cxx_incs)
                add_include(cxx_incs, "libcouchbase")
                add_include(cxx_incs, "libcouchbase", "couchbase++")


            def get_repo(inc):
                return os.path.join(inc, "libcouchbase-cxx-prefix", "src", "libcouchbase-cxx", "include")


            def add_include(cxx_incs, *path_components):
                final_path=os.path.join(cxx_incs,*path_components)
                import logging
                logging.error("include path adding: {}".format(final_path))
                if os.path.exists(final_path):
                    cppyy.add_include_path(final_path)
                else:
                    logging.error("Path does not exist: {}".format(final_path))


            try:
                add_cxx_includes(os.path.join(os.path.abspath(os.path.curdir),"cmake-build-release"))
                add_cxx_includes(base_inc)
            except:
                pass

            if CPPYY:
                cppyy.include("libcouchbase/couchbase++.h")
                from cppyy.gbl import Couchbase
                try:
                    os.rmdir("couchbase_genned")
                except:
                    pass

                #os.makedirs("couchbase_genned/cppyy",exist_ok=True)
                print(dir(Couchbase.Client))
                print(dir(Couchbase.GetResponse))
                print(dir(Couchbase.Buffer))
                print(dir(Couchbase.Response))
                print(dir(Couchbase))

            import sys

            from pybindgen import FileCodeSink
            from pybindgen.gccxmlparser import ModuleParser

            inc_paths = ['/usr/local/opt/llvm/include/c++/v1',
                         '/Users/ellis_breen/root/workspaces/couchbase/couchbase-python-client-mainline/cmake-build-release/install/include',
                         os.path.abspath(os.path.join(get_repo('cmake-build-release')))]
            gccxml_options = {'xml_generator': "castxml", 'xml_generator_path': "/usr/local/bin/castxml",
                              'cflags': '-std=c++1y ' + ' '.join(map(lambda x: "-I " + x, inc_paths))}

            source_files=[os.path.join(get_repo("cmake-build-release"), "libcouchbase", "couchbase++.h")]

            def my_module_gen():
                module_parser = ModuleParser('a2')
                repo_dir = get_repo("cmake-build-release")
                base_dir = os.path.join(repo_dir, "libcouchbase")
                include_files = list(
                    map(lambda x: os.path.abspath(os.path.join(base_dir, x)), filter(lambda y: y.endswith('.h'), os.listdir(base_dir))))
                print(include_files)
                paths=['/usr/local/opt/llvm/include/c++/v1',repo_dir,'/Users/ellis_breen/root/workspaces/couchbase/couchbase-python-client-mainline/cmake-build-release/install/include']
                print(paths)
                with open("build/genned.py","w+") as output:
                    module=module_parser.parse([os.path.join(get_repo("cmake-build-release"), "libcouchbase", "couchbase++.h")],include_paths=paths,
                                        includes=['"couchbase++.h"'], pygen_sink=FileCodeSink(output),
                                        gccxml_options=gccxml_options)

                print(module)

            #my_module_gen()
            import pyplusplus
            import pyplusplus.module_builder

            import pygccxml
            gccxml_config=pygccxml.parser.config.xml_generator_configuration_t(**gccxml_options)
            if False:
                declarations=pyplusplus.module_builder.boost_python_builder.parser.parse(source_files,config=gccxml_config)
                logging.error("declarations {}".format(declarations))
                #module_builder.module_builder_t()
                with open('dump.txt','w+') as dump:
                    import pprint
                    #pprint.pprint(declarations,dump)
                    dump_declarations(declarations,dump)
                try:
                    import mypy.stubgen

                    #mypy.stubgen.generate_stub_for_module("cppyy.gbl.Couchbase", "couchbase_genned")
                except:
                    pass
            import pyplusplus.code_creators.algorithm
            builder=pyplusplus.module_builder.module_builder_t(source_files,include_paths=inc_paths,cflags=gccxml_options['cflags'],gccxml_config=gccxml_config)
            creator=builder.build_code_creator("couchbase")
            with open('src/bindings_vanilla.cpp','w+') as bindings:
                output=creator.create()
                bindings.write(output)
        except BaseException as e:
            logging.error(repr(e))
            logging.error("exception:{}".format(traceback.format_exc()))

        try:
            pass
        except Exception as e:
            import traceback
            print(traceback.format_exc())
            pass