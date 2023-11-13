using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.Extensions.Logging;
using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;
using ServiceReference;
using BMI_Calculator_Web.Controller;

namespace BMI_Calculator_Web.Pages
{
    public class BmiInputModel
    {
        [Required(ErrorMessage = "Height is required.")]
        [Range(1, int.MaxValue, ErrorMessage = "Height must be a positive number.")]
        public int Height { get; set; }

        [Required(ErrorMessage = "Weight is required.")]
        [Range(1, int.MaxValue, ErrorMessage = "Weight must be a positive number.")]
        public int Weight { get; set; }
    }

    public class BmiPageModel : PageModel
    {
        [BindProperty]
        public BmiInputModel Input { get; set; }

        public BmiResultRest Result { get; set; }

        public void OnGet()
        {
        }

        public IActionResult OnPost(string submitType)
        {
            if (ModelState.IsValid)
            {
                int height = Input.Height;
                int weight = Input.Weight;

                Task<double> bmiTask = null;
                Task<BmiResult> healthSoapTask = null;
                Task<BmiResultRest> healthRestTask = null;
                double bmiResult = 0;

                BmiResultRest healthRestResult = new BmiResultRest();
                switch (submitType)
                {
                    case "bmiRest":
                        bmiTask = Task.Run(async () => await BMIController.CheckRESTBMI(height, weight));
                        bmiResult = bmiTask.Result;
                        Result = new BmiResultRest
                        {
                            Bmi = bmiResult
                        };
                        break;

                    case "bmiSoap":
                        bmiTask = Task.Run(async () => await BMIController.CheckSOAPBMI(height, weight));
                        bmiResult = bmiTask.Result;
                        Result = new BmiResultRest
                        {
                            Bmi = bmiResult
                        };
                        break;

                    case "healthSoap":
                        healthSoapTask = Task.Run(async () => await BMIController.CheckSOAPHealth(height, weight));
                        healthRestResult.Bmi = healthSoapTask.Result.Bmi;
                        healthRestResult.Risk = healthSoapTask.Result.Risk;
                        healthRestResult.RiskColor = healthSoapTask.Result.RiskColor;
                        healthRestResult.More = healthSoapTask.Result.More;
                        Result = healthRestResult;
                        break;

                    case "healthRest":
                        healthRestTask = Task.Run(async () => await BMIController.CheckRESTHealth(height, weight));
                        healthRestResult.Bmi = healthRestTask.Result.Bmi;
                        healthRestResult.Risk = healthRestTask.Result.Risk;
                        healthRestResult.RiskColor = healthRestTask.Result.RiskColor;
                        healthRestResult.More = healthRestTask.Result.More;
                        Result = healthRestResult;
                        break;

                    default:
                        break;
                }
            }

            return Page();
        }

    }
}
