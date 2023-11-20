dotnet publish -c Release -o ./publish ./BMI_Calculator/

zip -r ./BMICalculatorAPI.zip ./publish

aws s3 cp ./BMICalculatorAPI.zip s3://elasticbeanstalk-us-west-1-358485744732/BMICalculatorAPI.zip --profile claudio