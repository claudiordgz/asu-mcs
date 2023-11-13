using BMI_Calculator;
using BMI_Calculator_REST.Models;
using BMI_Calculator_Shared.Operations;
using Microsoft.AspNetCore.Mvc;


namespace BMI_Calculator_REST.Endpoints
{
    public static class BMICalculatorEndpoints
    {
        public static void ConfigureBMICalculatorEndpoints(this WebApplication app)
        {
            app.MapPost("/api/myHealth", getMyHealth)
                .Accepts<MyHealthDTO>("application/json")
                .Produces<BMI_Calculator.Models.BmiResult>(200);

            app.MapPost("/api/myBMI", getMyBMI)
                .Accepts<MyBMIDTO>("application/json")
                .Produces<double>(200);
        }

        private async static Task<IResult> getMyBMI(ILogger<Program> _logger, [FromBody] MyBMIDTO parameters)
        {
            try
            {
                _logger.LogInformation("Calculating BMI...");

                var bmi = BMIOperations.CalculateBMI(parameters.height, parameters.weight);

                _logger.LogInformation($"Calculated BMI: {bmi}");

                return Results.Ok(bmi);
            }
            catch (Exception ex)
            {
                _logger.LogError($"An error occurred during BMI calculation: {ex.Message}");
                return Results.BadRequest(ex.Message);
            }
        }

        private async static Task<IResult> getMyHealth(ILogger<Program> _logger, [FromBody] MyHealthDTO parameters)
        {
            try
            {
                _logger.LogInformation("Checking health status...");

                var bmi = (BMI_Calculator.Models.BmiResult)BMIOperations.GetHealthStatus(parameters.height, parameters.weight);

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
