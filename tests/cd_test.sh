#!/bin/bash

if [ ! -f "../bin/rshell" ]; then
	echo "Run \`make\` first!"
	exit 1
fi
 
echo "Execute cd cd"
../bin/rshell << EOF
cd cd 
echo
EOF

echo "Execute pwd; mkdir random; cd random; pwd; cd; pwd"
../bin/rshell << EOF
pwd; mkdir random; cd random; pwd; cd; pwd
echo
EOF

#clean up directory 'random'
rm -rf random

echo "Execute pwd; mkdir hello; cd hello; pwd"
../bin/rshell << EOF
pwd; mkdir hello; cd hello; pwd
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

echo "Execute pwd; mkdir hello; cd hello; pwd; mkdir hello1; cd hello1; pwd; cd..; pwd; cd ..; pwd"
../bin/rshell << EOF
pwd; mkdir hello; cd hello; pwd; mkdir hello1; cd hello1; pwd; cd..; pwd; cd ..; pwd
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

echo "Execute mkdir hello; cd hello; mkdir hello1; cd hello1; cd..; cd..; cd ./hello/hello1"
../bin/rshell << EOF
mkdir hello; cd hello; mkdir hello1; cd hello1; cd..; cd..; cd ./hello/hello1
echo
EOF

#clean up directory 'hello'
rm -rf hello

#clean up directory 'hello1'
rm -rf hello1

echo "Execute mkdir hello; cd hello; mkdir hello1; cd hello1; cd..; cd..; cd /hello/hello1"
../bin/rshell << EOF
mkdir hello; cd hello; mkdir hello1; cd hello1; cd..; cd..; cd /hello/hello1
echo
EOF

#clean up directory 'hello'
rm -rf hello

#clean up directory 'hello1'
rm -rf hello1

exit 0
