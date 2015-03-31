'''
    This is the biicode hook file. It changes some relative #include's, adds some lines to CMakeLists
    and some configuration files to root project folder to make possible POCO reusing.

    You can revert all the modifications suffered in the original code. By default,
    if you're using biicode, the changes are applied:

        BII_POCO_REVERT_CHANGES = False

    To revert all, run into your current command prompt:

        Windows:    $ set BII_POCO_REVERT_CHANGES=True
        Unix:       $ export BII_POCO_REVERT_CHANGES=True

    and execute "bii work" to back into original form.
'''
import os
import re
import shutil
import platform


def replacement(match_object):
    if not "Poco/Net/" in match_object.group(0):
        return '#include "Poco/Net/%s"' % match_object.group(1)
    return '#include "%s"' % match_object.group(1)


def save(path, binary_content):
    with open(path, 'wb') as handle:
        handle.write(binary_content)


def load(path):
    with open(path, 'rb') as handle:
        return handle.read()


def search_and_replace_pattern(_files, base_path, pattern):
    for _file in _files:
        try:
            _file_path = os.path.join(base_path, _file)
            c = load(_file_path)
            c = re.sub(pattern, replacement, c)
            save(_file_path, c)
        except:
            pass


def search_and_replace(_file, token, _replacement):
    try:
        c = load(_file)
        c = c.replace(token, _replacement)
        save(_file, c)
    except:
        pass


def prepare_all_samples():
    ignored_samples = ['ApacheConnector', 'CppUnit',
                       'PageCompiler']
    poco_modules = os.listdir(root_folder)
    for module in os.listdir(root_folder):
        if (platform.system() != "Windows" and module == "NetSSL_Win") \
           or module in ignored_samples:
            continue
        samples_module_path = os.path.join(root_folder, module, 'samples')
        if os.path.exists(samples_module_path):
            example_dest = os.path.join(root_folder, 'examples', module)
            shutil.copytree(samples_module_path, example_dest)

    if "#[mains] section" in load(biicode_conf_path):
        examples_mains = '''[mains]
    # Crypto
    examples/Crypto/genrsakey/src/genrsakey.cpp
    # SevenZip
    examples/SevenZip/un7zip/src/un7zip.cpp
    # Util
    examples/Util/pkill/src/pkill.cpp
    examples/Util/SampleApp/src/SampleApp.cpp
    examples/Util/SampleServer/src/SampleServer.cpp
    # Zip
    examples/Zip/zip/src/zip.cpp
    examples/Zip/unzip/src/unzip.cpp
'''
        search_and_replace(biicode_conf_path, "#[mains] section", examples_mains)

def delete_all_samples():
    examples_path = os.path.join(root_folder, 'examples')
    if os.path.exists(examples_path):
        shutil.rmtree(examples_path)


def apply_changes():
    ''' Applying necessary chnages to use POCO with biicode '''
    shutil.copy(os.path.join(root_folder, 'biicode', 'conf', 'biicode.conf'), root_folder)
    shutil.copy(os.path.join(root_folder, 'biicode', 'conf', 'ignore.bii'), root_folder)
    shutil.copy(os.path.join(root_folder, 'biicode', 'conf', 'pocomsg.h.bii'),
                os.path.join(root_folder, 'Foundation', 'src', 'pocomsg.h'))

    base_header_name = os.path.join(root_folder, "Foundation", "include", "Poco")
    # Replacements
    if os_platform == "Windows":
        pattern_match = '#include\s+"Poco/Net/(%s)"' % '|'.join(netssl_win_all_headers)
        pattern_match = pattern_match.replace('.', '\\.')
        search_and_replace_pattern(netssl_win_all_headers, netssl_win_headers_path, pattern_match)
        search_and_replace_pattern(netssl_win_all_sources, netssl_win_src_path, pattern_match)
    search_and_replace(os.path.join(base_header_name, "DeflatingStream.h"), "Poco/zlib.h", "zlib/zlib/zlib.h")
    search_and_replace(os.path.join(base_header_name, "InflatingStream.h"), "Poco/zlib.h", "zlib/zlib/zlib.h")
    search_and_replace(os.path.join(root_folder, "Foundation", "src", "Checksum.cpp"), "Poco/zlib.h", "zlib/zlib/zlib.h")
    search_and_replace(os.path.join(root_folder, "Zip", "src", "ZipStream.cpp"), "Poco/zlib.h", "zlib/zlib/zlib.h")
    search_and_replace(cmakelist_path, cmakelist_token, cmakelist_replacement)

    prepare_all_samples()


def revert_changes():
    ''' Revert all the biicode changes code '''
    os.remove(biicode_conf_path)
    os.remove(ignore_bii_path)
    os.remove(pocomsg_h_path)

    base_header_name = os.path.join(root_folder, "Foundation", "include", "Poco")
    # Replacements
    if os_platform == "Windows":
        pattern_match = '#include\s+"(%s)"' % '|'.join(netssl_win_all_headers)
        pattern_match = pattern_match.replace('.', '\\.')
        search_and_replace_pattern(netssl_win_all_headers, netssl_win_headers_path, pattern_match)
        search_and_replace_pattern(netssl_win_all_sources, netssl_win_src_path, pattern_match)
    search_and_replace(os.path.join(base_header_name, "DeflatingStream.h"), "zlib/zlib/zlib.h", "Poco/zlib.h")
    search_and_replace(os.path.join(base_header_name, "InflatingStream.h"), "zlib/zlib/zlib.h", "Poco/zlib.h")
    search_and_replace(os.path.join(root_folder, "Foundation", "src", "Checksum.cpp"), "zlib/zlib/zlib.h", "Poco/zlib.h")
    search_and_replace(os.path.join(root_folder, "Zip", "src", "ZipStream.cpp"), "zlib/zlib/zlib.h", "Poco/zlib.h")
    if os_platform == "Windows":
        search_and_replace(cmakelist_path, cmakelist_replacement_win, cmakelist_token)
    else:
        search_and_replace(cmakelist_path, cmakelist_replacement, cmakelist_token)

    delete_all_samples()


# Main code
os_platform = platform.system()
BII_POCO_REVERT_CHANGES = os.environ.get('BII_POCO_REVERT_CHANGES', 'False')

root_folder = bii.block_folder if os.path.exists(bii.block_folder) else bii.project_folder
netssl_win_headers_path = os.path.join(root_folder, 'NetSSL_Win', 'include', 'Poco', 'Net')
netssl_win_all_headers = os.listdir(netssl_win_headers_path)
netssl_win_src_path = os.path.join(root_folder, 'NetSSL_Win', 'src')
netssl_win_all_sources = os.listdir(netssl_win_src_path)

ignore_bii_path = os.path.join(root_folder, 'ignore.bii')
biicode_conf_path = os.path.join(root_folder, 'biicode.conf')
pocomsg_h_path = os.path.join(root_folder, 'Foundation', 'src', 'pocomsg.h')

cmakelist_path = os.path.join(root_folder, "CMakeLists.txt")
cmakelist_token = "# COMMENT REPLACED BY BIICODE"
cmakelist_replacement = '''if(BIICODE)
include(biicode/cmake/biicode.cmake)
return()
endif()'''
cmakelist_replacement_win = "if(BIICODE)\r\ninclude(biicode/cmake/biicode.cmake)\r\nreturn()\r\nendif()"

try:
    # Apply or revert changes
    if BII_POCO_REVERT_CHANGES == 'False':
        if "if(BIICODE)" in load(cmakelist_path):
            print "Hook: changes just applied"
        else:
            print "Hook: applying changes"
            apply_changes()
    else:
        if "if(BIICODE)" not in load(cmakelist_path):
           print "Hook: changes just reverted"
        else:
            print "Hook: reverting changes"
            revert_changes()
except Exception as e:
    print "Exception: %s" % e