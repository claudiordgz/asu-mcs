using System;
using System.Net.Http;
using System.ServiceModel;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;
using ServiceReference;

namespace BMI_Calculator_Console
{
    public class BmiResultRest
    {
        public double Bmi { get; set; }
        public string Risk { get; set; }

        public string RiskColor { get; set; }
        public string[] More { get; set; }
    }

    class Program
    {
        private const string BaseUrl = "https://localhost:7035/";

        static void Main(string[] args)
        {
            RunAsync().Wait();
        }

        static async Task RunAsync()
        {
            while (true)
            {
                Console.WriteLine("Choose an option:");
                Console.WriteLine("1. Use SOAP - Check BMI");
                Console.WriteLine("2. Use SOAP - Check Health");
                Console.WriteLine("3. Use REST - Check BMI");
                Console.WriteLine("4. Use REST - Check Health");
                Console.WriteLine("5. Exit");

                string choice = Console.ReadLine();

                switch (choice)
                {
                    case "1":
                        await CheckSOAPBMI();
                        break;
                    case "2":
                        await CheckSOAPHealth();
                        break;
                    case "3":
                        await CheckRESTBMI();
                        break;
                    case "4":
                        await CheckRESTHealth();
                        break;
                    case "5":
                        return;
                    default:
                        Console.WriteLine("Invalid choice. Please try again.");
                        break;
                }
            }
        }

        static async Task CheckSOAPBMI()
        {
            var client = new BmiCalculatorServiceClient();

            try
            {
                Console.Write("Enter height (in inches): ");
                int height = int.Parse(Console.ReadLine());

                Console.Write("Enter weight (in pounds): ");
                int weight = int.Parse(Console.ReadLine());

                double bmi = await client.myBMIAsync(height, weight);
                Console.WriteLine($"BMI: {bmi}");
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error: {ex.Message}");
            }
            finally
            {
                client.Close();
            }
        }

        static async Task CheckSOAPHealth()
        {
            var client = new BmiCalculatorServiceClient();

            try
            {
                Console.Write("Enter height (in inches): ");
                int height = int.Parse(Console.ReadLine());

                Console.Write("Enter weight (in pounds): ");
                int weight = int.Parse(Console.ReadLine());

                BmiResult result = await client.myHealthAsync(height, weight);
                Console.ForegroundColor = GetConsoleColor(result.RiskColor.ToLower());
                Console.WriteLine(result.Risk);
                Console.ResetColor();


                Console.WriteLine("More Information Links:");
                foreach (string link in result.More)
                {
                    Console.WriteLine(link);
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error: {ex.Message}");
            }
            finally
            {
                client.Close();
            }
        }

        static async Task CheckRESTBMI()
        {
            try
            {
                Console.Write("Enter height (in inches): ");
                int height = int.Parse(Console.ReadLine());

                Console.Write("Enter weight (in pounds): ");
                int weight = int.Parse(Console.ReadLine());

                var requestData = new { height, weight };
                var response = await PostAsync("api/myBMI", requestData);

                if (response.IsSuccessStatusCode)
                {
                    var content = await response.Content.ReadAsStringAsync();
                    double bmi = JsonSerializer.Deserialize<double>(content);
                    Console.WriteLine($"BMI: {bmi}");
                }
                else
                {
                    Console.WriteLine($"HTTP Error: {response.StatusCode}");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error: {ex.Message}");
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

        static async Task CheckRESTHealth()
        {
            try
            {
                Console.Write("Enter height (in inches): ");
                int height = int.Parse(Console.ReadLine());

                Console.Write("Enter weight (in pounds): ");
                int weight = int.Parse(Console.ReadLine());

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

                    Console.ForegroundColor = GetConsoleColor(result.RiskColor.ToLower());
                    Console.WriteLine(result.Risk);
                    Console.ResetColor();

                    Console.WriteLine($"BMI: {result.Bmi}");

                    Console.WriteLine("More Information Links:");
                    foreach (string link in result.More)
                    {
                        Console.WriteLine(link);
                    }
                }
                else
                {
                    Console.WriteLine($"HTTP Error: {response.StatusCode}");
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error: {ex.Message}");
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
