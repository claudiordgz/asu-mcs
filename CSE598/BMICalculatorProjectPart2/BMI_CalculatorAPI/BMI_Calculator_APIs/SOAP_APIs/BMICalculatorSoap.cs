using BMI_Calculator_Shared.Operations;
using System.Runtime.Serialization;
using System.ServiceModel;

namespace BMI_Calculator_APIS.SOAP_APIs
{
    [ServiceContract]
    public interface IBmiCalculatorService
    {
        [OperationContract]
        double myBMI(int heightInInches, int weightInPounds);

        [OperationContract]
        BmiResult myHealth(int heightInInches, int weightInPounds);
    }


    [DataContract]
    public class BmiResult : BMI_Calculator.Models.IBmiResult
    {
        [DataMember]
        public double Bmi { get; set; }

        [DataMember]
        public string Risk { get; set; }
        [DataMember]
        public string RiskColor { get; set; }

        [DataMember]
        public string[] More { get; set; }
    }

    public class BmiCalculatorService : IBmiCalculatorService
    {
        public double myBMI(int heightInInches, int weightInPounds)
        {
            return BMIOperations.CalculateBMI(heightInInches, weightInPounds);
        }

        public BmiResult myHealth(int heightInInches, int weightInPounds)
        {
            var result = BMIOperations.GetHealthStatus(heightInInches, weightInPounds);
            return new BmiResult
            {
                Bmi = result.Bmi,
                Risk = result.Risk,
                More = result.More,
                RiskColor = result.RiskColor
            };
        }
    }
}