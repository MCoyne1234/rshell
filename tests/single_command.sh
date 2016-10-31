#!/bin/bash

if [ ! -f "../bin/rshell" ]; then
	echo "Run \`make\` first!"
	exit 1
fi
 
../bin/rshell << EOF
echo An     echo     with     multiple      sepaces     between     arguments.
echo 

echo Execute a nonexistent command - foo:
foo
echo

echo Execute cat /proc/version:
cat /proc/version
echo

echo Change to a nonexistent directory - /foo:
cd /foo
echo

echo -n Current working dir: 
pwd
echo

echo Change to parent directory...
cd ..
echo Current working directory:
pwd
echo

echo Execute ls -al:
ls -al
EOF

exit 0