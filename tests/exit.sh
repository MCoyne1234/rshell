#!/bin/bash

if [ ! -f "../bin/rshell" ]; then
        echo "Run \`make\` first!"
        exit 1
fi

echo "Execute exit"
../bin/rshell << EOF
exit
echo
EOF

echo "Execute exit ls; # echo hello"
../bin/rshell << EOF
exit ls; # echo hello
echo
EOF

echo "Execute echo output1; echo output2 || exit"
../bin/rshell << EOF
echo output1; echo output2 || exit
echo
EOF

echo "Execute exit ls -a; # echo output1 || echo output2 && echo output3"
../bin/rshell << EOF
exit ls -a; # echo output1 || echo output2 && echo output3
echo
EOF

echo "Execute echo output1; echo output2; echo output3 && exit"
../bin/rshell << EOF
echo output1; echo output2; echo output3 && exit"
echo
EOF

echo "Execute exit || exit"
../bin/rshell << EOF
exit || exit
echo
EOF

echo "Execute echo ls -a && echo Hi || exit"
../bin/rshell << EOF
echo ls -a && echo Hi || exit
echo
EOF

echo "Execute exit && echo hello"
../bin/rshell << EOF
exit && echo hello
echo
EOF

exit 0