#!/bin/bash

if [ ! -f "../bin/rshell" ]; then
	echo "Run \`make\` first!"
	exit 1
fi
 
echo "Execute pwd; cd cd; pwd"
../bin/rshell << EOF
pwd; cd cd; pwd
echo
EOF

echo "Execute pwd; cd ../src; pwd"
../bin/rshell << EOF
pwd; cd ../src; pwd
echo
EOF

echo "Execute pwd; cd /usr/bin; pwd"
../bin/rshell << EOF
pwd; cd /usr/bin; pwd
echo
EOF

echo "Execute pwd; mkdir random; cd random; pwd; cd -; pwd"
../bin/rshell << EOF
pwd; mkdir random; cd random; pwd; cd -; pwd
echo
EOF

#clean up directory 'random'
rm -rf random

echo "Execute pwd; cd; pwd; cd -; pwd"
../bin/rshell << EOF
pwd; cd; pwd; cd -; pwd
echo
EOF

echo "Execute pwd; mkdir hello; cd hello; pwd; cd ..; pwd"
../bin/rshell << EOF
pwd; mkdir hello; cd hello; pwd; cd ..; pwd
echo
EOF

#clean up directory 'hello'
rm -rf hello

echo "Execute pwd; mkdir hello; cd hello; pwd; mkdir hello1; cd hello1; pwd"
../bin/rshell << EOF
pwd; mkdir hello; cd hello; pwd; mkdir hello1; cd hello1; pwd
echo
EOF

#clean up directory 'hello'
rm -rf hello

#clean up directory 'hello1'
rm -rf hello1

echo "Execute pwd; mkdir hello; cd hello; pwd; mkdir hello1; cd hello1; pwd; cd..; pwd; cd ..; pwd"
../bin/rshell << EOF
pwd; mkdir hello; cd hello; pwd; mkdir hello1; cd hello1; pwd; cd ..; pwd; cd ..; pwd
echo
EOF

#clean up directory 'hello'
rm -rf hello

#clean up directory 'hello1'
rm -rf hello1

echo "Execute pwd; mkdir hello; cd hello; pwd; mkdir hello1; cd hello1; pwd; cd; pwd"
../bin/rshell << EOF
pwd; mkdir hello; cd hello; pwd; mkdir hello1; cd hello1; pwd; cd; pwd
echo
EOF

#clean up directory 'hello'
rm -rf hello

#clean up directory 'hello1'
rm -rf hello1

echo "Execute pwd; mkdir hello; cd hello; pwd; mkdir hello1; cd hello1; pwd; cd ..; pwd; cd ..; pwd; cd hello/hello1; pwd"
../bin/rshell << EOF
pwd; mkdir hello; cd hello; pwd; mkdir hello1; cd hello1; pwd; cd ..; pwd; cd ..; pwd; cd hello/hello1; pwd
echo
EOF

#clean up directory 'hello'
rm -rf hello

#clean up directory 'hello1'
rm -rf hello1

echo "Execute pwd; mkdir hello; cd hello; pwd; mkdir hello1; cd hello1; pwd; cd ..; pwd; cd ..; pwd; cd /hello/hello1 pwd"
../bin/rshell << EOF
Execute pwd; mkdir hello; cd hello; pwd; mkdir hello1; cd hello1; pwd; cd ..; pwd; cd ..; pwd; cd /hello/hello1 pwd
echo
EOF

#clean up directory 'hello'
rm -rf hello

#clean up directory 'hello1'
rm -rf hello1

exit 0
