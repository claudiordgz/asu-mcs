﻿//------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
//------------------------------------------------------------------------------

namespace ServiceReference
{
    using System.Runtime.Serialization;
    
    
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.CodeDom.Compiler.GeneratedCodeAttribute("Microsoft.Tools.ServiceModel.Svcutil", "2.0.3")]
    [System.Runtime.Serialization.DataContractAttribute(Name="BmiResult", Namespace="http://schemas.datacontract.org/2004/07/BMI_Calculator_SOAP")]
    public partial class BmiResult : object
    {
        
        private double BmiField;
        
        private string[] MoreField;
        
        private string RiskField;
        
        private string RiskColorField;
        
        [System.Runtime.Serialization.DataMemberAttribute()]
        public double Bmi
        {
            get
            {
                return this.BmiField;
            }
            set
            {
                this.BmiField = value;
            }
        }
        
        [System.Runtime.Serialization.DataMemberAttribute()]
        public string[] More
        {
            get
            {
                return this.MoreField;
            }
            set
            {
                this.MoreField = value;
            }
        }
        
        [System.Runtime.Serialization.DataMemberAttribute()]
        public string Risk
        {
            get
            {
                return this.RiskField;
            }
            set
            {
                this.RiskField = value;
            }
        }
        
        [System.Runtime.Serialization.DataMemberAttribute()]
        public string RiskColor
        {
            get
            {
                return this.RiskColorField;
            }
            set
            {
                this.RiskColorField = value;
            }
        }
    }
    
    [System.CodeDom.Compiler.GeneratedCodeAttribute("Microsoft.Tools.ServiceModel.Svcutil", "2.0.3")]
    [System.ServiceModel.ServiceContractAttribute(ConfigurationName="ServiceReference.IBmiCalculatorService")]
    public interface IBmiCalculatorService
    {
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/IBmiCalculatorService/myBMI", ReplyAction="http://tempuri.org/IBmiCalculatorService/myBMIResponse")]
        System.Threading.Tasks.Task<double> myBMIAsync(int heightInInches, int weightInPounds);
        
        [System.ServiceModel.OperationContractAttribute(Action="http://tempuri.org/IBmiCalculatorService/myHealth", ReplyAction="http://tempuri.org/IBmiCalculatorService/myHealthResponse")]
        System.Threading.Tasks.Task<ServiceReference.BmiResult> myHealthAsync(int heightInInches, int weightInPounds);
    }
    
    [System.CodeDom.Compiler.GeneratedCodeAttribute("Microsoft.Tools.ServiceModel.Svcutil", "2.0.3")]
    public interface IBmiCalculatorServiceChannel : ServiceReference.IBmiCalculatorService, System.ServiceModel.IClientChannel
    {
    }
    
    [System.Diagnostics.DebuggerStepThroughAttribute()]
    [System.CodeDom.Compiler.GeneratedCodeAttribute("Microsoft.Tools.ServiceModel.Svcutil", "2.0.3")]
    public partial class BmiCalculatorServiceClient : System.ServiceModel.ClientBase<ServiceReference.IBmiCalculatorService>, ServiceReference.IBmiCalculatorService
    {
        
        /// <summary>
        /// Implement this partial method to configure the service endpoint.
        /// </summary>
        /// <param name="serviceEndpoint">The endpoint to configure</param>
        /// <param name="clientCredentials">The client credentials</param>
        static partial void ConfigureEndpoint(System.ServiceModel.Description.ServiceEndpoint serviceEndpoint, System.ServiceModel.Description.ClientCredentials clientCredentials);
        
        public BmiCalculatorServiceClient() : 
                base(BmiCalculatorServiceClient.GetDefaultBinding(), BmiCalculatorServiceClient.GetDefaultEndpointAddress())
        {
            this.Endpoint.Name = EndpointConfiguration.BasicHttpBinding_IBmiCalculatorService.ToString();
            ConfigureEndpoint(this.Endpoint, this.ClientCredentials);
        }
        
        public BmiCalculatorServiceClient(EndpointConfiguration endpointConfiguration) : 
                base(BmiCalculatorServiceClient.GetBindingForEndpoint(endpointConfiguration), BmiCalculatorServiceClient.GetEndpointAddress(endpointConfiguration))
        {
            this.Endpoint.Name = endpointConfiguration.ToString();
            ConfigureEndpoint(this.Endpoint, this.ClientCredentials);
        }
        
        public BmiCalculatorServiceClient(EndpointConfiguration endpointConfiguration, string remoteAddress) : 
                base(BmiCalculatorServiceClient.GetBindingForEndpoint(endpointConfiguration), new System.ServiceModel.EndpointAddress(remoteAddress))
        {
            this.Endpoint.Name = endpointConfiguration.ToString();
            ConfigureEndpoint(this.Endpoint, this.ClientCredentials);
        }
        
        public BmiCalculatorServiceClient(EndpointConfiguration endpointConfiguration, System.ServiceModel.EndpointAddress remoteAddress) : 
                base(BmiCalculatorServiceClient.GetBindingForEndpoint(endpointConfiguration), remoteAddress)
        {
            this.Endpoint.Name = endpointConfiguration.ToString();
            ConfigureEndpoint(this.Endpoint, this.ClientCredentials);
        }
        
        public BmiCalculatorServiceClient(System.ServiceModel.Channels.Binding binding, System.ServiceModel.EndpointAddress remoteAddress) : 
                base(binding, remoteAddress)
        {
        }
        
        public System.Threading.Tasks.Task<double> myBMIAsync(int heightInInches, int weightInPounds)
        {
            return base.Channel.myBMIAsync(heightInInches, weightInPounds);
        }
        
        public System.Threading.Tasks.Task<ServiceReference.BmiResult> myHealthAsync(int heightInInches, int weightInPounds)
        {
            return base.Channel.myHealthAsync(heightInInches, weightInPounds);
        }
        
        public virtual System.Threading.Tasks.Task OpenAsync()
        {
            return System.Threading.Tasks.Task.Factory.FromAsync(((System.ServiceModel.ICommunicationObject)(this)).BeginOpen(null, null), new System.Action<System.IAsyncResult>(((System.ServiceModel.ICommunicationObject)(this)).EndOpen));
        }
        
        public virtual System.Threading.Tasks.Task CloseAsync()
        {
            return System.Threading.Tasks.Task.Factory.FromAsync(((System.ServiceModel.ICommunicationObject)(this)).BeginClose(null, null), new System.Action<System.IAsyncResult>(((System.ServiceModel.ICommunicationObject)(this)).EndClose));
        }
        
        private static System.ServiceModel.Channels.Binding GetBindingForEndpoint(EndpointConfiguration endpointConfiguration)
        {
            if ((endpointConfiguration == EndpointConfiguration.BasicHttpBinding_IBmiCalculatorService))
            {
                System.ServiceModel.BasicHttpBinding result = new System.ServiceModel.BasicHttpBinding();
                result.MaxBufferSize = int.MaxValue;
                result.ReaderQuotas = System.Xml.XmlDictionaryReaderQuotas.Max;
                result.MaxReceivedMessageSize = int.MaxValue;
                result.AllowCookies = true;
                result.Security.Mode = System.ServiceModel.BasicHttpSecurityMode.Transport;
                return result;
            }
            throw new System.InvalidOperationException(string.Format("Could not find endpoint with name \'{0}\'.", endpointConfiguration));
        }
        
        private static System.ServiceModel.EndpointAddress GetEndpointAddress(EndpointConfiguration endpointConfiguration)
        {
            if ((endpointConfiguration == EndpointConfiguration.BasicHttpBinding_IBmiCalculatorService))
            {
                return new System.ServiceModel.EndpointAddress("https://localhost:7229/BmiCalculatorService.asmx");
            }
            throw new System.InvalidOperationException(string.Format("Could not find endpoint with name \'{0}\'.", endpointConfiguration));
        }
        
        private static System.ServiceModel.Channels.Binding GetDefaultBinding()
        {
            return BmiCalculatorServiceClient.GetBindingForEndpoint(EndpointConfiguration.BasicHttpBinding_IBmiCalculatorService);
        }
        
        private static System.ServiceModel.EndpointAddress GetDefaultEndpointAddress()
        {
            return BmiCalculatorServiceClient.GetEndpointAddress(EndpointConfiguration.BasicHttpBinding_IBmiCalculatorService);
        }
        
        public enum EndpointConfiguration
        {
            
            BasicHttpBinding_IBmiCalculatorService,
        }
    }
}
