#!groovy
pipeline {
        @NonCPS for (buildnode in ["hipster", "debian8"] ) {
            node("${buildnode}") {
                agent label:"${buildnode}"
                stages {
                        stage ("PREP") {
                                steps {
                                        step ("CHECKOUT") {
                                                echo "Checkout to ${env.NODE_NAME} : ${WORKSPACE}"
                                                checkout scm
                                        }

                                        step ("CLEANUP") {
                                                echo 'Clean'
                                                sh 'if [ -s Makefile ] ; then make -k clean >/dev/null 2>&1 ; make -k distclean >/dev/null 2>&1 ; fi'
                                                sh 'rm -f config.cache config.log config.status'
                                                sh 'git checkout -- scripts/DMF/dmfnutscan/*.dmf scripts/DMF/dmfsnmp/*.dmf || true'
                                        }

                                        step ("AUTOGEN") {
                                                echo 'Autogen'
                                                sh './autogen.sh'
                                        }
                                }
                        }
                        stage ('Configure') {
                                steps {
                                        sh 'PATH=/usr/lib/ccache:$PATH CC=/usr/lib/ccache/gcc CXX=/usr/lib/ccache/g++ ./configure --with-snmp --with-neon --with-dev --with-doc=man -C --with-snmp_dmf=yes --with-dmfnutscan-regenerate=yes --with-dmfsnmp-regenerate=yes'
                                }
                        }
                        stage ('Build') {
                                steps {
                                        sh 'gmake -j 4 -k all || { echo ""; echo "================"; echo "=== REMAKE"; gmake -j1 all; }'
                                }
                        }
                        stage ('TestMore') {
                                steps {
                                        sh 'gmake distcheck-dmf'
                                }
                        }
                }
            }
        }
}
