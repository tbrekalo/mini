import os, shutil, subprocess

from pathlib import Path
from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext

from distutils import log

EXTENSION_NAME = "minipy"


class CMakeExtension(Extension):
  def __init__(self, name, sourcedir=""):
    Extension.__init__(self, name, sources=[])
    self.sourcedir = os.path.abspath(sourcedir)


class CMakeBuild(build_ext):
  def build_extension(self, ext):
    install_dir = Path(self.get_ext_fullpath(ext.name)).absolute().parent
    source_dir = Path(__file__).absolute().parent
    build_dir = Path(self.build_temp).absolute()
  
    self.announce(f'[minipy::setup.py] extdir: {install_dir}', level=log.INFO)

    if build_dir.exists():
      shutil.rmtree(build_dir)
      build_dir.mkdir()

    cmake_generator = os.environ.get("CMAKE_GENERATOR", "")
    cmake_args = [
        f'-S {source_dir}',
        f'-B {build_dir}',

        f'-DMINI_BUILD_PYTHON=1',
        f'-DMINIPY_EXT_DIR={install_dir}',

        f'-DCMAKE_BUILD_TYPE=Release'
    ]

    try:
      import ninja
      cmake_args += ["-G Ninja"]
    except ImportError:
      pass

    build_args = ['--target', 'install']
    if "CMAKE_BUILD_PARALLEL_LEVEL" not in os.environ:
      # self.parallel is a Python 3 only way to set parallel jobs by hand
      # using -j in the build_ext call, not supported by pip or PyPA-build.
      if hasattr(self, "parallel") and self.parallel:
          # CMake 3.12+ only.
          build_args += [f"-j{self.parallel}"]

    if not os.path.exists(self.build_temp):
        os.makedirs(self.build_temp)

    subprocess.check_call(
        ["cmake", ext.sourcedir] + cmake_args, cwd=self.build_temp
    )
    subprocess.check_call(
        ["cmake", "--build", ".", '--target', 'install'] + build_args, cwd=self.build_temp
    )


if __name__ == '__main__':
  setup(
      name=EXTENSION_NAME,
      version='1.0.0',
      author='Tvrtko Brekalo',
      author_email="brekalo.tvrtko@gmail.com",
      platforms=['linux'],
      license='mit',

      ext_modules=[CMakeExtension(EXTENSION_NAME)],
      cmdclass={
          'build_ext': CMakeBuild
      }
  )
