//
//  WebView.swift
//  BMI_Calculator
//
//  Created by Claudio Bernardo Rodriguez Rodriguez on 11/21/23.
//

import UIKit
import WebKit

class WebView: UIViewController, WKUIDelegate {
    var webView: WKWebView!

    override func loadView() {
        let webConfiguration = WKWebViewConfiguration()
        webView = WKWebView(frame: .zero, configuration: webConfiguration)
        webView.uiDelegate = self
        view = webView
    }

    override func viewDidLoad() {
        super.viewDidLoad()
        displayWebPage()
    }

    private func displayWebPage() {
        let url = URL(string: "https://www.google.com.au")!
        let request = URLRequest(url: url)
        webView.load(request)
    }
}
