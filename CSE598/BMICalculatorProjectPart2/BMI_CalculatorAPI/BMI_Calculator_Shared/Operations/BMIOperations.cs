using BMI_Calculator.Models;


namespace BMI_Calculator_Shared.Operations
{
    public class BMIOperations
    {
        public static double CalculateBMI(int heightInInches, int weightInPounds)
        {
            double bmi = (weightInPounds / (double)(heightInInches * heightInInches)) * 703;
            return bmi;
        }

        public static IBmiResult GetHealthStatus(int heightInInches, int weightInPounds)
        {
            double bmi = CalculateBMI(heightInInches, weightInPounds);
            var risk = GetBmiRisk(bmi);

            string[] moreLinks = new string[]
            {
            "https://www.cdc.gov/healthyweight/assessing/bmi/index.html",
            "https://www.nhlbi.nih.gov/health/educational/lose_wt/index.htm",
            "https://www.ucsfhealth.org/education/body_mass_index_tool/"
            };

            BmiResult result = new BmiResult
            {
                Bmi = bmi,
                Risk = risk.RiskMessage,
                RiskColor = risk.Color,
                More = moreLinks
            };

            return result;
        }

        private static (string RiskMessage, string Color) GetBmiRisk(double bmi)
        {
            if (bmi < 18)
            {
                return ("underweight", "Blue");
            }
            else if (bmi >= 18 && bmi < 25)
            {
                return ("normal", "Green");
            }
            else if (bmi >= 25 && bmi < 30)
            {
                return ("pre-obese", "Purple");
            }
            else
            {
                return ("obese", "Red");
            }
        }
    }
}
