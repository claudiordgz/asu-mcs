
using BMI_Calculator_SOAP;
using SoapCore;

var builder = WebApplication.CreateBuilder(args);

builder.Services.AddAuthorization();

builder.Services.AddSoapCore();
builder.Services.AddScoped<IBmiCalculatorService, BmiCalculatorService>();

builder.Services.AddEndpointsApiExplorer();
var app = builder.Build();

app.UseHttpsRedirection();
app.UseRouting();
app.UseAuthorization();

app.Map("/BmiCalculatorService.asmx", app =>
{   
    app.UseSoapEndpoint<IBmiCalculatorService>("", new SoapEncoderOptions(), SoapSerializer.DataContractSerializer);
});

app.Run();

