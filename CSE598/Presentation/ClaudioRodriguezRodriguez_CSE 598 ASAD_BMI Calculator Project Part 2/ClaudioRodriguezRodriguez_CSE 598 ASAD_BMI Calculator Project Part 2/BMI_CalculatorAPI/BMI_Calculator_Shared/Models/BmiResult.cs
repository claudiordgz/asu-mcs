namespace BMI_Calculator.Models
{
    public interface IBmiResult
    {
        public double Bmi { get; set; }
        public string Risk { get; set; }
        public string RiskColor { get; set; }
        public string[] More { get; set; }
    }

    public class BmiResult : IBmiResult
    {
        public double Bmi { get; set;}
        public string Risk { get; set; }
        public string RiskColor { get; set; }
        public string[] More { get; set; }
    }
}
