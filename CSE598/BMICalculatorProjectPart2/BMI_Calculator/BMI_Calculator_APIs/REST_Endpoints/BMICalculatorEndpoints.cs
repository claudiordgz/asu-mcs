using BMI_Calculator;
using BMI_Calculator_Shared.Operations;
using Microsoft.AspNetCore.Mvc;


namespace BMI_Calculator_SOAP.Endpoints
{
    public static class BMICalculatorEndpoints
    {
        public static void ConfigureBMICalculatorEndpoints(this WebApplication app)
        {
            app.MapGet("/api/myHealth", getMyHealth)
                .Produces<BMI_Calculator.Models.BmiResult>(200);

            app.MapGet("/api/myBMI", getMyBMI)
                .Produces<double>(200);
        }

        private async static Task<IResult> getMyBMI(int weight, int height, ILogger<Program> _logger)
        {
            try
            {
                _logger.LogInformation("Calculating BMI...");

                var bmi = BMIOperations.CalculateBMI(height, weight);

                _logger.LogInformation($"Calculated BMI: {bmi}");

                return Results.Ok(bmi);
            }
            catch (Exception ex)
            {
                _logger.LogError($"An error occurred during BMI calculation: {ex.Message}");
                return Results.BadRequest(ex.Message);
            }
        }

        private async static Task<IResult> getMyHealth(int weight, int height, ILogger<Program> _logger)
        {
            try
            {
                _logger.LogInformation("Checking health status...");

                var bmi = (BMI_Calculator.Models.BmiResult)BMIOperations.GetHealthStatus(height, weight);

                _logger.LogInformation($"Health status: {bmi.Risk}");

                return Results.Ok(bmi);
            }
            catch (Exception ex)
            {
                _logger.LogError($"An error occurred during health status check: {ex.Message}");
                return Results.BadRequest(ex.Message);
            }
        }
    }
}
