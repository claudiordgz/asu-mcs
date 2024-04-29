//
//  ContentView.swift
//  BMI_Calculator
//
//  Created by Claudio Bernardo Rodriguez Rodriguez on 11/13/23.
//

import SwiftUI
import Foundation
import WebKit

struct BMICalculatorView: View {
    @StateObject private var userInfo = UserInfo()
    @State private var isToggleOn: Bool = false
    
    @State private var showWebView = false
    @State private var selectedURL: URL?
    
    private var webView = WKWebView()
    
    var riskColor: Color {
        switch userInfo.riskColor {
        case "Red":
            return .red
        case "Purple":
            return .purple
        case "Green":
            return .green
        case "Blue":
            return .blue
        default:
            return .black // Default color if none of the above
        }
    }
    
    var educateButtonDisabled: Bool {
        userInfo.moreLinks.isEmpty
    }
    
    var heightString: Binding<String> {
        Binding<String>(
            get: {
                userInfo.height != 0 ? String(userInfo.height) : ""
            },
            set: {
                userInfo.height = Int($0) ?? 0
            }
        )
    }
    
    var weightString: Binding<String> {
        Binding<String>(
            get: {
                userInfo.weight != 0 ? String(userInfo.weight) : ""
            },
            set: {
                userInfo.weight = Int($0) ?? 0
            }
        )
    }
    
    var body: some View {
        NavigationView {
            VStack {
                Form {
                    Section {
                        HStack {
                            Text("Enter height:")
                                .font(.headline)
                            TextField("inches", text: heightString)
                                .keyboardType(.numberPad)
                                .multilineTextAlignment(.trailing)
                                .font(.headline)
                                .padding()
                                .background(RoundedRectangle(cornerRadius: 10).stroke(Color.gray, lineWidth: 1))
                        }
                        HStack {
                            Text("Enter weight:")
                                .font(.headline)
                            TextField("inches", text: weightString)
                                .keyboardType(.numberPad)
                                .multilineTextAlignment(.trailing)
                                .font(.headline)
                                .padding()
                                .background(RoundedRectangle(cornerRadius: 10).stroke(Color.gray, lineWidth: 1))
                        }
                        Spacer()
                        HStack {
                            Spacer()
                            Text("BMI: ")
                                .font(.headline)
                            Text("\(userInfo.bmi, specifier: "%.2f")")
                                .font(.headline)
                            Spacer()
                        }
                        .listRowSeparator(.hidden)
                    
                        
                        Text(userInfo.risk)
                            .foregroundColor(riskColor)
                            .multilineTextAlignment(.center)
                            .font(.headline)
                            .frame(height: /*@START_MENU_TOKEN@*/100/*@END_MENU_TOKEN@*/)
                            .frame(maxWidth: .infinity, alignment: .center)
                    }
                }
                Spacer()
                Group {
                    HStack{
                        Button("Call BMI API") {
                            callBMIAPI()
                        }
                        .padding(10)
                        .background(.blue)
                        .foregroundColor(.white)
                        .cornerRadius(10)
                        Spacer()
                            .frame(maxWidth: 50)
                        Button("Educate Me") {
                            openRandomEducationalLink()
                        }
                        .disabled(educateButtonDisabled)
                        .padding(10)
                        .background(educateButtonDisabled ? .gray : .blue)
                        .foregroundColor(.white)
                        .cornerRadius(10)
                    }
                }
                .padding()
            }
            .navigationBarItems(trailing: Toggle(isOn: $isToggleOn, label: {
                Text($isToggleOn.wrappedValue ? "REST" : "SOAP")
                    .font(.footnote)
                    
              
            }))
            .navigationBarTitle("BMI API", displayMode: .inline)
        }
    }
    
    func callBMIAPI() {
        if isToggleOn {
            userInfo.fetchHealthData{}
        } else {
            userInfo.fetchHealthDataSoap {}
        }
    }
    
    func showWebViewSheet() {
       DispatchQueue.main.async {
           let webViewController = WebView()
           if let url = self.selectedURL {
               webViewController.loadViewIfNeeded()
               webViewController.webView.load(URLRequest(url: url))
           }

           let scenes = UIApplication.shared.connectedScenes
           let windowScene = scenes.first as? UIWindowScene
           let rootVC = windowScene?.windows.first?.rootViewController
           rootVC?.present(webViewController, animated: true, completion: nil)
       }
   }
    
    func openRandomEducationalLink() {
        if let urlString = self.userInfo.moreLinks.randomElement(), let url = URL(string: urlString) {
            self.selectedURL = url
            showWebViewSheet()
        }
    }
    
}

#Preview {
    BMICalculatorView()
}
