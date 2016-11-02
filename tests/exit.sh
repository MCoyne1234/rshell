#!/bin/bash

if [ ! -f "../bin/rshell" ]; then
        echo "Run \`make\` first!"
        exit 1
fi

echo "Execute exit"
../bin/rshell << EOF
exit
EOF
echo "- bash: Exit status:" $? 

echo
echo "Execute exit 2"
../bin/rshell << EOF
exit 2
EOF
echo "- bash: Exit status:" $? 

echo
echo "Execute exit ls"
echo "[When error occured, 1 would be the default exit status]"
../bin/rshell << EOF
exit ls
EOF
echo "- bash: Exit status:" $? 

echo
echo "Execute mkdir exit.sh && exit 2"
../bin/rshell << EOF
mkdir exit.sh && exit 2
EOF
echo "- bash: Exit status:" $? 

echo
echo "Execute exit && echo hello"
../bin/rshell << EOF
exit && echo hello
EOF
echo "- bash: Exit status:" $? 

echo
echo "Execute echo output1; echo output2 && exit; echo output3"
../bin/rshell << EOF
echo output1; echo output2 && exit; echo output3
EOF
echo "- bash: Exit status:" $? 

echo
echo "Execute exit 1 || exit 2"
../bin/rshell << EOF
exit 1 || exit 2
EOF
echo "- bash: Exit status:" $? 

echo
echo "Execute exit 1 && exit 2"
../bin/rshell << EOF
exit 1 && exit 2
EOF
echo "- bash: Exit status:" $? 

exit 0
