provider "aws" {
  region  = "us-west-1"
  profile = "claudio"
}


resource "aws_elastic_beanstalk_application" "cse598api" {
  name        = "BMICalculatorApi"
  description = "CSE 598 API"
}

resource "aws_elastic_beanstalk_application_version" "cse598apiversion" {
  name        = "BMICalculatorApi"
  application = aws_elastic_beanstalk_application.cse598api.name
  description = "CSE 598 API"
  bucket      = "elasticbeanstalk-us-west-1-358485744732" # Specify your S3 bucket name
  key         = "BMICalculatorAPI.zip"                    # Specify the name of your ZIP file

}

resource "aws_elastic_beanstalk_environment" "cse598api" {
  name        = "BMICalculatorApi"
  application = aws_elastic_beanstalk_application.cse598api.name
  # NET APP
  solution_stack_name    = "64bit Windows Server 2019 v2.12.0 running IIS 10.0"
  wait_for_ready_timeout = "10m"

  setting {
    namespace = "aws:autoscaling:launchconfiguration"
    name      = "IamInstanceProfile"
    value     = "CSE598EBSRole"
  }

  setting {
    namespace = "aws:elasticbeanstalk:environment"
    name      = "EnvironmentType"
    value     = "SingleInstance"
  }

}

