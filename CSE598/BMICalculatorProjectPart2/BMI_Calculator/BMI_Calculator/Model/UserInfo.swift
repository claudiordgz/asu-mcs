//
//  UserInfo.swift
//  BMI_Calculator
//
//  Created by Claudio Bernardo Rodriguez Rodriguez on 11/21/23.
//

import Foundation
import SWXMLHash

class UserInfo: ObservableObject {
    // Read-only fields
    @Published public private(set) var bmi: Double = 0.0
    @Published public private(set) var risk: String = ""
    @Published public private(set) var riskColor: String = ""
    @Published public private(set) var moreLinks: [String] = []

    // UI controlled fields
    @Published var height: Int = 0
    @Published var weight: Int = 0
    
    // Function to fetch health data
    func fetchHealthData(completion: @escaping () -> Void) {
        let urlString = "http://localhost:5011/api/myHealth?height=\(height)&weight=\(weight)"
        guard let url = URL(string: urlString) else { return }

        let task = URLSession.shared.dataTask(with: url) { [weak self] data, response, error in
            guard let data = data, error == nil else { return }

            DispatchQueue.main.async {
                if let json = try? JSONSerialization.jsonObject(with: data) as? [String: Any] {
                    self?.bmi = json["bmi"] as? Double ?? 0.0
                    self?.risk = "You are " + (json["risk"] as? String ?? "")
                    self?.riskColor = json["riskColor"] as? String ?? ""
                    self?.moreLinks = json["more"] as? [String] ?? []
                }
                completion()
            }
        }

        task.resume()
    }
    
    func createMyHealthSoapBody(height: Int, weight: Int) -> String {
        let soapBody = """
        <?xml version="1.0" encoding="utf-8"?>
        <soap:Envelope xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd="http://www.w3.org/2001/XMLSchema" xmlns:soap="http://schemas.xmlsoap.org/soap/envelope/">
          <soap:Body>
            <myHealth xmlns="http://tempuri.org/">
              <heightInInches>\(height)</heightInInches>
              <weightInPounds>\(weight)</weightInPounds>
            </myHealth>
          </soap:Body>
        </soap:Envelope>
        """
        return soapBody
    }
    
    func fetchHealthDataSoap(completion: @escaping () -> Void) {
        let urlString = "http://localhost:5011/BmiCalculatorService.asmx"
        guard let url = URL(string: urlString) else { return }

        var request = URLRequest(url: url)
        request.httpMethod = "POST"
        request.addValue("text/xml; charset=utf-8", forHTTPHeaderField: "Content-Type")
        request.addValue("http://tempuri.org/IBmiCalculatorService/myHealth", forHTTPHeaderField: "SOAPAction")
        request.httpBody = createMyHealthSoapBody(height: self.height, weight: self.weight).data(using: .utf8)

        let task = URLSession.shared.dataTask(with: request) { [weak self] data, response, error in
            if let error = error {
                print("Error: \(error)")
                return
            }

            guard let data = data else {
                print("No data received")
                return
            }
            
            let xml = XMLHash.parse(data)
            DispatchQueue.main.async {
                let bmiValue = xml["s:Envelope"]["s:Body"]["myHealthResponse"]["myHealthResult"]["d4p1:Bmi"].element?.text ?? "0.0"
                let risk = xml["s:Envelope"]["s:Body"]["myHealthResponse"]["myHealthResult"]["d4p1:Risk"].element?.text ?? ""
                let riskColor = xml["s:Envelope"]["s:Body"]["myHealthResponse"]["myHealthResult"]["d4p1:RiskColor"].element?.text ?? ""
                let moreLinks = xml["s:Envelope"]["s:Body"]["myHealthResponse"]["myHealthResult"]["d4p1:More"]["d5p1:string"].all.map { $0.element?.text ?? "" }
                
                guard let strongSelf = self else { return }
                strongSelf.bmi = Double(bmiValue) ?? 0.0
                strongSelf.risk = "You are " + risk
                strongSelf.riskColor = riskColor
                strongSelf.moreLinks = moreLinks
                
                completion()
            }
            
        }

        task.resume()
    }
}
