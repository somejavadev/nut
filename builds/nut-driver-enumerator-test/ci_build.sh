#!/bin/sh

# This is a CI test for nut-driver-enumerator-test.sh
# using chosen SHELL_PROGS (passed from caller envvars)

cd "${REPO_DIR}/tests" || exit

printf "Will test nut-driver-enumerator interpreted by: "
if [ -n "${SHELL_PROGS}" ] ; then
    echo "$SHELL_PROGS"
else
    echo "default system '/bin/sh'"
fi

ls -la
sh -x ./nut-driver-enumerator-test.sh
