import sys
import os
from typing import List
from urllib.parse import urlparse
import urllib.request
from xml.etree.ElementInclude import include
import zipfile
from shutil import copyfile
from distutils.dir_util import copy_tree

CACHE_DIR = 'D:/whatever/MyCache/'
INSTALL_DIR = 'ThirdParty/'

PROXY = 'http://127.0.0.1:10808'
os.environ['http_proxy'] = PROXY
os.environ['HTTP_PROXY'] = PROXY
os.environ['https_proxy'] = PROXY
os.environ['HTTPS_PROXY'] = PROXY


class Library:
    def __init__(self, name: str, urls: list, include_dir: str, debug_lib_dir: str, release_lib_dir: str):
        self.name = name
        self.urls = urls
        self.include_dir = include_dir
        self.debug_lib_dir = debug_lib_dir
        self.release_lib_dir = release_lib_dir


glslang = Library(
    name='glslang',
    urls=[
        'https://github.com/KhronosGroup/glslang/releases/download/8.13.3559/glslang-master-windows-x64-Debug.zip',
        'https://github.com/KhronosGroup/glslang/releases/download/8.13.3559/glslang-master-windows-x64-Release.zip'
    ],
    include_dir='glslang-master-windows-x64-Debug/include',
    debug_lib_dir='glslang-master-windows-x64-Debug/lib',
    release_lib_dir='glslang-master-windows-x64-Release/lib'
)


def get_file_name(url: str):
    url = urlparse(url)
    file_name = os.path.basename(url.path)
    return file_name


def download(dir: str, url: str):
    file_name = get_file_name(url)
    file_path = os.path.join(dir, file_name)
    if not os.path.exists(file_path):
        urllib.request.urlretrieve(url, file_path)
    return file_path


def unzip(file_path: str):
    file_name = os.path.basename(file_path)
    dest_folder_base = os.path.dirname(file_path)
    dest_folder_name = os.path.splitext(file_name)[0]
    with zipfile.ZipFile(file_path, 'r') as zip_ref:
        zip_ref.extractall(os.path.join(dest_folder_base, dest_folder_name))


if __name__ == "__main__":
    libraries: List[Library] = []
    libraries.append(glslang)

    if not os.path.exists(INSTALL_DIR):
        os.mkdir(INSTALL_DIR)
    if not os.path.exists(CACHE_DIR):
        os.mkdir(CACHE_DIR)

    for library in libraries:
        library_cache_dir = os.path.join(CACHE_DIR + library.name)
        library_install_dir = os.path.join(INSTALL_DIR + library.name)
        library_install_include_dir = os.path.join(
            library_install_dir, 'include')
        library_install_lib_dir = os.path.join(library_install_dir, 'lib')
        library_install_lib_debug_dir = os.path.join(library_install_lib_dir, 'debug')
        library_install_lib_release_dir = os.path.join(library_install_lib_dir, 'release')
        library_install_bin_dir = os.path.join(library_install_dir, 'bin')
        if not os.path.exists(library_cache_dir):
            os.mkdir(library_cache_dir)

        if not os.path.exists(library_install_dir):
            os.mkdir(library_install_dir)
        if not os.path.exists(library_install_include_dir):
            os.mkdir(library_install_include_dir)
        if not os.path.exists(library_install_lib_dir):
            os.mkdir(library_install_lib_dir)
        if not os.path.exists(library_install_bin_dir):
            os.mkdir(library_install_bin_dir)
        if not os.path.exists(library_install_lib_debug_dir):
            os.mkdir(library_install_lib_debug_dir)
        if not os.path.exists(library_install_lib_release_dir):
            os.mkdir(library_install_lib_release_dir)

        for url in library.urls:
            file_path = download(library_cache_dir, url)
            unzip(file_path)

        library_cache_include_dir = os.path.join(
            library_cache_dir, library.include_dir)
        copy_tree(library_cache_include_dir, library_install_include_dir)
        library_cache_lib_debug_dir = os.path.join(library_cache_dir, library.debug_lib_dir)
        library_cache_lib_release_dir = os.path.join(library_cache_dir, library.release_lib_dir)
        copy_tree(library_cache_lib_debug_dir, library_install_lib_debug_dir)
        copy_tree(library_cache_lib_release_dir, library_install_lib_release_dir)

        # (_, _, filenames) = next(os.walk(library_cache_include_dir))
        # for filename in filenames:
        #     if os.path.splitext(filename)[1] == '.lib':
        #         copyfile(os.path.join(library_cache_include_dir, filename), os.path.join(library_install_include_dir, filename))
