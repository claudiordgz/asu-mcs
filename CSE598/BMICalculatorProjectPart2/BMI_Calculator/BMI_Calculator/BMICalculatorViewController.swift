//
//  File.swift
//  BMI_Calculator
//
//  Created by Claudio Bernardo Rodriguez Rodriguez on 11/22/23.
//

import SwiftUI

class BMICalculatorViewController: UIHostingController<BMICalculatorView> {
    required init?(coder aDecoder: NSCoder) {
        super.init(coder: aDecoder, rootView: BMICalculatorView())
    }

    override func viewDidLoad() {
        super.viewDidLoad()
    }
}
