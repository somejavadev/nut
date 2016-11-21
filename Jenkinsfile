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
                                        sh 'if [ -s Makefile ] ; then make -k clean || true; make -k distclean || true; fi; true'
/*
                                        script {
                                                fileExists ([file: "Makefile"]) {
                                                    sh 'make -k clean || true'
                                                    sh 'make -k distclean || true'
                                                }
                                        }
*/
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
                                    stash("${env.BUILD_TAG}-${env.GIT_COMMIT}-${env.NODE_LABELS}".replace(' ','_').replace('%','_'))
                                    script {
                                        def subtests = [:]
                                        for (makerecipe in ["distcheck-light","distcheck-light-man","distcheck-dmf-all-yes","distcheck-dmf-no","distcheck-dmf-warnings"
,"distcheck-dmf-features-REGEN_NO"
,"distcheck-dmf-features-LTDL_YES"
,"distcheck-dmf-features-LTDL_NO"
,"distcheck-dmf-features-LUA_YES"
,"distcheck-dmf-features-LUA_YESNO"
,"distcheck-dmf-features-LUA_NO"
,"distcheck-dmf-features-REGEN_YES"
                                        ] ) {
                                            subtests["TEST-${makerecipe}@${env.NODE_LABELS}".replace(' ',' && ')] = {
                                                    node("${env.NODE_LABELS}".replace(' ',' && ')) {
                                                        stage ("TEST: ${makerecipe}") {
                                                            echo "UNSTASH for DISTCHECK"
                                                            unstash("${env.BUILD_TAG}-${env.GIT_COMMIT}-${env.NODE_LABELS}".replace(' ','_').replace('%','_'))
                                                            echo "MAKE DISTCHECK : ${makerecipe}"
                                                            sh "CCACHE_BASEDIR=\"`pwd`\" gmake ${makerecipe}"
                                                        }
                                                    }
                                                }
                                            }
                                        parallel subtests
                                        }
                                    }
                                }
                        }
                }
//            }
//        }

