import sys
from skbuild import setup

setup(
    name="minipy",
    version="1.4.0",
    author="Tvrtko Brekalo",
    author_email="brekalo.tvrtko@gmail.com",
    description="Minimizer extraction utility library as described in minimap paper",
    url="https://github.com/tbrekalo/mini",
    license="BSD",
    packages=['minipy'],
    package_dir={'': 'src'},
    cmake_install_dir="src/minipy",
    include_package_data=True,
    python_requires=">=3.8"
)
