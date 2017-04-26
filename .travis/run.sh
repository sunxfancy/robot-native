#！/bin/bash - 
# @CreateTime: Apr 26, 2017 11:17 AM 
# @Author: sxf 
# @Contact: sunxfancy@gmail.com 
# @Last Modified By: sxf 
# @Last Modified Time: Apr 26, 2017 11:17 AM 
# @Description: Modify Here, Please 

set -e
set -x

if [[ "$(uname -s)" == 'Darwin' ]]; then
    if which pyenv > /dev/null; then
        eval "$(pyenv init -)"
    fi
    pyenv activate conan
fi

npm install