pipeline {
    agent {
        dockerfile {
             filename 'Dockerfile.debian8'
             dir 'DOCKER'
        }
    }
    stages {
        stage('debian') {
            steps {
                  checkout scm
                  sh 'make'
            }
        }
    }
}