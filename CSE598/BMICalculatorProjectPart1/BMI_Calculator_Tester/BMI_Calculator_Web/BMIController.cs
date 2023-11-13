using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Http;
using System.Threading.Tasks;
using System.ServiceModel;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using ServiceReference;

namespace BMI_Calculator_Web.Controller
{
    public class BmiResultRest
    {
        public double Bmi { get; set; }
        public string Risk { get; set; }

        public string RiskColor { get; set; }
        public string[] More { get; set; }
    }

    public class BMIController
    {
        private const string BaseUrl = "https://localhost:7035/";
        public static async Task<double> CheckSOAPBMI(int height, int weight)
        {
            var client = new BmiCalculatorServiceClient();

            try
            {
                double bmi = await client.myBMIAsync(height, weight);
                return bmi;
            }
            catch (Exception ex)
            {
                throw new Exception($"Error: {ex.Message}");
            }
            finally
            {
                client.Close();
            }
        }

        public static async Task<BmiResult> CheckSOAPHealth(int height, int weight)
        {
            var client = new BmiCalculatorServiceClient();

            try
            {
                BmiResult result = await client.myHealthAsync(height, weight);
                return result;
            }
            catch (Exception ex)
            {
                throw new Exception($"Error: {ex.Message}");
            }
            finally
            {
                client.Close();
            }
        }

        public static async Task<double> CheckRESTBMI(int height, int weight)
        {
            try
            {
                var requestData = new { height, weight };
                var response = await PostAsync("api/myBMI", requestData);

                if (response.IsSuccessStatusCode)
                {
                    var content = await response.Content.ReadAsStringAsync();
                    double bmi = JsonSerializer.Deserialize<double>(content);
                    return bmi;
                }
                else
                {
                    throw new Exception($"HTTP Error: {response.StatusCode}");
                }
            }
            catch (Exception ex)
            {
                throw new Exception($"Error: {ex.Message}");
            }
        }
        private static ConsoleColor GetConsoleColor(string colorName)
        {
            switch (colorName.ToLower())
            {
                case "blue":
                    return ConsoleColor.Blue;
                case "green":
                    return ConsoleColor.Green;
                case "purple":
                    return ConsoleColor.DarkMagenta;
                case "red":
                    return ConsoleColor.Red;
                default:
                    return ConsoleColor.White;
            }
        }

        public static async Task<BmiResultRest> CheckRESTHealth(int height, int weight)
        {
            try
            {
                var requestData = new { height, weight };
                var response = await PostAsync("api/myHealth", requestData);

                if (response.IsSuccessStatusCode)
                {
                    var content = await response.Content.ReadAsStringAsync();
                    var options = new JsonSerializerOptions
                    {
                        PropertyNameCaseInsensitive = true,
                        PropertyNamingPolicy = JsonNamingPolicy.CamelCase,
                    };
                    BmiResultRest result = JsonSerializer.Deserialize<BmiResultRest>(content, options);
                    return result;
                }
                else
                {
                    throw new Exception($"HTTP Error: {response.StatusCode}");
                }
            }
            catch (Exception ex)
            {
                throw new Exception($"Error: {ex.Message}");
            }
        }
        static async Task<HttpResponseMessage> PostAsync(string endpoint, object requestData)
        {
            using (HttpClient client = new HttpClient())
            {
                client.BaseAddress = new Uri(BaseUrl);
                client.DefaultRequestHeaders.Accept.Clear();
                client.DefaultRequestHeaders.Accept.Add(new System.Net.Http.Headers.MediaTypeWithQualityHeaderValue("application/json"));

                var requestContent = new StringContent(Newtonsoft.Json.JsonConvert.SerializeObject(requestData), Encoding.UTF8, "application/json");

                return await client.PostAsync(endpoint, requestContent);
            }
        }
    }
}
