# BMI Calculator Web Service

This project demonstrates the creation of a BMI (Body Mass Index) Calculator web service with SOAP and RESTful endpoints. It includes two solutions, one for Visual Studio 2019 and .NET 5 and another for Visual Studio 2022 and .NET 7.

## Solution 1: Visual Studio 2019 and .NET 5 (BMI_Calculator_Tester)

### Projects

1. **BMI_Calculator_Web**: An ASP.NET Core web application for BMI calculation and health status checking. It includes a Razor Pages front-end for user interaction.
2. **BMI_Calculator_Console**: A console application for testing SOAP and REST endpoints of the BMI calculator service.

### Getting Started

1. Open the `BMI_Calculator_Tester.sln` solution in Visual Studio 2019.
2. Build and run the `BMI_Calculator_Web` project.
3. Access the web application in your browser at `https://localhost:44350/`.

### Usage

- Use the web application to input height and weight and calculate BMI or check your health status using SOAP and REST endpoints.
- Use the console application to test SOAP and REST endpoints programmatically.

## Solution 2: Visual Studio 2022 and .NET 7 (BMI_Calculator)

### Projects

1. **BMI_Calculator_SOAP**: An ASP.NET Core web application for hosting the SOAP BMI calculator services.
2. **BMI_Calculator_REST**: An ASP.NET Core web application for hosting the REST and RESTful BMI calculator services.
3. **BMI_Calculator_Shared**: Business logic for both services.

### Getting Started

1. Open the `BMI_Calculator.sln` solution in Visual Studio 2022.
2. Build and run the `BMI_Calculator_SOAP` and `BMI_Calculator_REST` project.
3. The SOAP and REST endpoints will be available for testing.

### Usage

- Use the `BMI_Calculator_Web` to test the services using a Web Application.
- Use the `BMI_Calculator_Console` to test using a CLI.
