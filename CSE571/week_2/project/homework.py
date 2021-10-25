from pgmpy.inference import VariableElimination
from pgmpy.models import BayesianModel
from pgmpy.factors.discrete import TabularCPD

import decimal
decimal.getcontext().prec = 20

# Define the model structure (also see the instructions)
model = BayesianModel([('P', 'C'),
                        ('C', 'M'),
                        ('C', 'T')])

# now defining the parameters.
cpd_p_pencil_party_elected = TabularCPD(variable='P',
                                        variable_card=2,
                                        values=[
                                            [0.8],
                                            [0.2] 
                                        ])
cpd_c_car_ownership_regulation_passed = TabularCPD(variable='C', 
                                           variable_card=2,
                                           values=[
                                               [0.667, 0.15],
                                               [0.333, 0.85]
                                           ],
                                           evidence=['P'],
                                           evidence_card=[2])
cpd_more_car_industry_revenue = TabularCPD(variable='M', 
                                           variable_card=2,
                                           values=[
                                                [0.6, 0.7],
                                                [0.4, 0.3]
                                            ],
                                           evidence=['C'],
                                           evidence_card=[2])
cpd_more_people_get_to_work_on_time = TabularCPD(variable='T', 
                                                 variable_card=2,
                                                 values=[
                                                     [0.5, 0.25],
                                                     [0.5, 0.75]
                                                 ],
                                                 evidence=['C'], 
                                                 evidence_card=[2])

# Associating the parameters with the model structure.
model.add_cpds(cpd_p_pencil_party_elected, cpd_c_car_ownership_regulation_passed, cpd_more_car_industry_revenue, cpd_more_people_get_to_work_on_time)

model.check_model()

def print_cpds(m):
    for cpd in m.get_cpds():
        print(f'CPT of {cpd.variable}:')
        print(cpd, '\n')

# Print model information
# print(model.edges())
# print(model.nodes())
# print_cpds(model)

inference = VariableElimination(model)

# Part 1
print(inference.query(variables=['P','C']))

# Part 2
print(inference.query(variables=['P'], evidence={'C': 1}))

# Part 3
print(inference.query(variables=['M'], evidence={'C': 1}))
print(inference.query(variables=['T'], evidence={'C': 1}))
print(inference.query(variables=['P','C','M','T']))

# Part 4
print(inference.query(variables=['M'], evidence={'C': 1}))
print(inference.query(variables=['M'], evidence={'C': 1, 'P':1}))
print(inference.query(variables=['M']))
print(inference.query(variables=['T'], evidence={'M': 1}))



