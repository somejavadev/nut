#!groovy
//for (buildnode in ["hipster", "debian8"] ) {
//    node("${buildnode}") {
        pipeline {
//                agent label:"${buildnode}"
                agent label:""
                environment {
                        PATH="/usr/lib/ccache:\$PATH"
                        CC="/usr/lib/ccache/gcc"
                        CXX="/usr/lib/ccache/g++"
                        CCACHE_DIR="/home/jim/.ccache"
//                        CCACHE_BASEDIR="${WORKSPACE}"
//                        CCACHE_BASEDIR="\$(pwd)"
                }
                stages {
                        stage ("PREP-CO") {
                                steps {
                                        echo "Checkout to ${env.NODE_NAME} : ${env.WORKSPACE}"
                                        checkout scm
                                }
                        }

                        stage ("PREP-CLEAN") {
                                steps {
                                        sh 'if [ -s Makefile ] ; then make -k clean >/dev/null 2>&1 || true; make -k distclean >/dev/null 2>&1 || true; fi; true'
                                        sh 'rm -f config.cache config.log config.status || true'
                                        sh 'git checkout -- scripts/DMF/dmfnutscan/*.dmf scripts/DMF/dmfsnmp/*.dmf || true'
                                }
                        }

                        stage ("PREP-AUTOGEN") {
                                steps {
                                        echo 'Autogen'
                                        sh './autogen.sh'
                                }
                        }

                        stage ('Configure-DMF-quick') {
                                steps {
//                                        sh 'set ; ls -la'
//                                        sh 'PATH=/usr/lib/ccache:$PATH CC=/usr/lib/ccache/gcc CXX=/usr/lib/ccache/g++ ./configure --with-snmp --with-neon --with-dev --with-doc=man -C --with-snmp_dmf=yes --with-dmfnutscan-regenerate=yes --with-dmfsnmp-regenerate=yes'
                                        sh 'CCACHE_BASEDIR="`pwd`" ./configure --with-snmp --with-neon --with-dev --with-doc=skip --with-snmp_dmf=yes --with-dmfnutscan-regenerate=no --with-dmfsnmp-regenerate=no'
                                }
                        }

                        stage ('Build') {
                                steps {
                                        sh 'CCACHE_BASEDIR="`pwd`" gmake -j 4 -k all || { echo ""; echo "================"; echo "=== REMAKE"; gmake -j1 all; }'
                                }
                        }
                        stage ('TestMore') {
                                steps {
                                    script {
                                        ["distcheck-light","distcheck-light-man","distcheck-dmf-features","distcheck-dmf-all-yes","distcheck-dmf-no","distcheck-dmf-warnings","distcheck-dmf"].each {
                                            sh "CCACHE_BASEDIR=\"`pwd`\" gmake ${it}"
                                        }
                                    }
                                }
                        }
                }
//            }
//        }
}
