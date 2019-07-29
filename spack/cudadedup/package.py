from spack import *
import os.path
import sys
class Cudadedup(Package):
    version('master', git='https://github.com/bwelton/cuda_deduplicator.git', branch='master', preferred=True)
    version('develop', git='https://github.com/bwelton/cuda_deduplicator.git', branch='develop')
    version('v0.1', git='https://github.com/bwelton/cuda_deduplicator.git', branch='master', tag="v0.1")

    # depends_on('libelf')
    depends_on("elf@1", type="link")
    depends_on('libdwarf')
    depends_on('dyninst')
    depends_on('python@2.7.13+shared')
    depends_on('cuda')
    depends_on('cmake', type="build")
    depends_on('boost')

    ## Variants 
    variant('stacktrace', default=False, description="Enable Stacktracing for found duplicates.")
    variant('timeline', default=False, description="Enable Transfer Timeline.")
    variant('desthash', default=False, description="Enable Destination Hash Checking.")
    def install(self, spec, prefix):
        ## Find the location of libcuda.so.... 
        libcuda_path = ""
        spack_root = ""
        if os.path.exists("/opt/cray/nvidia/default/lib64/libcuda.so.1"):
            ## We are on a cray machine which hosts libcuda in a special location....
            libcuda_path = "/opt/cray/nvidia/default/lib64/libcuda.so.1"
            ## Very hacky, tests will not work for installs not on lustre
            spack_root = prefix.split("opt/spack/cray-CNL-interlagos/gcc-4.9.3")[0]
        elif os.path.exists("/usr/lib64/libcuda.so.1"):
            ## Standard location for libcuda.so
            libcuda_path = "/usr/lib64/libcuda.so.1"
        elif os.path.exists("/usr/lib64/libcuda.so"):
            ## Standard location for libcuda.so
            libcuda_path = "/usr/lib64/libcuda.so"            
        elif os.path.exists("/lib64/libcuda.so.1"):
            ## Less common but somtimes appears here...
            libcuda_path = "/lib64/libcuda.so.1"
        elif os.path.exists("/lib64/libcuda.so"):
            ## Less common but somtimes appears here...
            libcuda_path = "/lib64/libcuda.so"
        else:
            raise InstallError("Could not find libcuda.so in any of the standard places...")
        libcuda_art = join_path(spec["cuda"].prefix, "lib64/libcudart.so")
        timeline = "no"
        strace = "no"
        desthash = "no"
        if "+timeline" in spec:
            timeline = "yes"

        if "+stacktrace" in spec:
            strace = "yes"

        if "+desthash" in spec:
            desthash = "yes"

        with working_dir("spack-build", create=True):
            cmake("..", 
                "-DPYTHON_EXECUTABLE=%s" % join_path(spec["python"].prefix.bin,"python"),
                "-DPYTHON_INCLUDE_DIR=%s" % spec["python"].prefix.include,
                "-DPYTHON_LIBRARY=%s" % join_path(spec["python"].prefix.lib,"libpython2.7.so"),
                "-DLIBELF_LIBFILE=%s" % join_path(spec["elf"].prefix.lib,"libelf.so"),
                "-DLIBELF_INCLUDE=%s" % spec["elf"].prefix.include,
                "-DLIBDWARF_LIB=%s" % join_path(spec["libdwarf"].prefix.lib,"libdwarf.so"),
                "-DLIBDWARF_INCLUDE=%s" % spec["libdwarf"].prefix.include,
                "-DDYNINST_ROOT=%s" % spec["dyninst"].prefix,
                "-DCUDA_TOOLKIT_ROOT_DIR=%s" % spec["cuda"].prefix,
                "-DLIBCUDA_SO=%s" % libcuda_path,
                "-DLIBCUDART_SO=%s" % libcuda_art,
                "-DENABLE_STRACE=%s" % strace,
                "-DENABLE_TIMELINE=%s" % timeline,
                "-DENABLE_DESTHASH=%s" % desthash,
                "-DSPACK_INSTALL=yes", 
                "-DSPACK_ROOT=%s" % spack_root,
                "-DBoost_NO_SYSTEM_PATHS=TRUE",
                "-DBOOST_INCLUDEDIR=%s" % spec["boost"].prefix.include,
                "-DBOOST_LIBRARYDIR=%s" % spec["boost"].prefix.lib,
                "-DBOOSTROOT=%s" % spec["boost"].prefix,
                "-DBOOST_ROOT=%s" % spec["boost"].prefix,  *std_cmake_args)
            make()
            make('install')


