#!groovy

pipeline {
    agent label:""
    environment {
        PATH="/usr/lib/ccache:\$PATH"
        CC="/usr/lib/ccache/gcc"
        CXX="/usr/lib/ccache/g++"
        CCACHE_DIR="/home/jim/.ccache"
//        CCACHE_BASEDIR="${WORKSPACE}"
//        CCACHE_BASEDIR="\$(pwd)"
    }
    stages {
        stage ("SELECT-BUILDER") {
            steps {
                script {
                    def subbuilds = [:]
                    for (var_buildnode in ["hipster", "debian8"] ) {
                        def buildnode = "${var_buildnode}"
                        subbuilds["${buildnode}"] = {
                            node("${buildnode}") {
//                                agent label:"${buildnode}"
                                stage ("PREP-SCM@${buildnode}") {
//                                    step {
                                        echo "Checkout to ${env.NODE_NAME} : ${env.WORKSPACE}"
                                        checkout scm
//                                    }
                                }

                                stage ("PREP-CLEAN@${buildnode}") {
//                                    step {
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
//                                    }
                                }

                                stage ("PREP-AUTOGEN@${buildnode}") {
//                                    step {
                                        echo 'Autogen'
                                        sh './autogen.sh'
//                                    }
                                }

                                stage ("Configure-DMF-quick@${buildnode}") {
//                                    step {
//                                        sh 'set ; ls -la'
//                                        sh 'PATH=/usr/lib/ccache:$PATH CC=/usr/lib/ccache/gcc CXX=/usr/lib/ccache/g++ ./configure --with-snmp --with-neon --with-dev --with-doc=man -C --with-snmp_dmf=yes --with-dmfnutscan-regenerate=yes --with-dmfsnmp-regenerate=yes'
                                        sh 'CCACHE_BASEDIR="`pwd`" ./configure --with-snmp --with-neon --with-dev --with-doc=skip --with-snmp_dmf=yes --with-dmfnutscan-regenerate=no --with-dmfsnmp-regenerate=no'
//                                    }
                                }

                                stage ("Build@${buildnode}") {
//                                    step {
                                        sh 'CCACHE_BASEDIR="`pwd`" gmake -j 4 -k all || { echo ""; echo "================"; echo "=== REMAKE"; gmake -j1 all; }'
//                                    }
                                }

                                stage ("FanoutTests@${buildnode}") {
//                                    step {
                                        script {
                                            def tag_stashed = "${env.BUILD_TAG}-${env.GIT_COMMIT}-${env.NODE_LABELS}".replace(' ','_').replace('%','_')
                                            stash("${tag_stashed}")
                                            deleteDir()
                                            def subtests = [:]
                                            for (makerecipe in [
 "distcheck-light"
,"distcheck-light-man"
,"distcheck-dmf-all-yes"
,"distcheck-dmf-no"
,"distcheck-dmf-warnings"
,"distcheck-dmf-features-REGEN_NO"
,"distcheck-dmf-features-LTDL_YES"
,"distcheck-dmf-features-LTDL_NO"
,"distcheck-dmf-features-LUA_YES"
,"distcheck-dmf-features-LUA_YESNO"
,"distcheck-dmf-features-LUA_NO"
,"distcheck-dmf-features-REGEN_YES"
                                            ] ) {
                                                def tag_recipe = "${makerecipe}"
                                                def tag_labels = "${env.NODE_LABELS}".replace(' ','_').replace('%','_')
                                                def node_labels = "${env.NODE_LABELS}".replace(' ',' && ')
                                                subtests["TEST-${tag_recipe}@${tag_labels}"] = {
                                                    node("${node_labels}") {
                                                        stage ("TEST@${buildnode}: ${tag_recipe}") {
                                                            echo "WIPE before UNSTASH for DISTCHECK"
                                                            deleteDir()
                                                            echo "UNSTASH for DISTCHECK"
                                                            unstash("${tag_stashed}")
                                                            echo "Reconfigure (fix up srcdir)"
                                                            sh 'CCACHE_BASEDIR="`pwd`" ./configure -C --with-snmp --with-neon --with-dev --with-doc=skip --with-snmp_dmf=yes --with-dmfnutscan-regenerate=no --with-dmfsnmp-regenerate=no'
                                                            echo "MAKE DISTCHECK : ${tag_recipe}"
                                                            sh "CCACHE_BASEDIR=\"`pwd`\" gmake ${tag_recipe}"
                                                            echo "WIPE after UNSTASH for DISTCHECK"
                                                            deleteDir()
                                                        }
                                                    }
                                                }
                                            }
                                            parallel subtests
                                        }
//                                    }
                                }
                            }
                        }
                    }
                parallel subbuilds
                }
            }
        }
    }
}
