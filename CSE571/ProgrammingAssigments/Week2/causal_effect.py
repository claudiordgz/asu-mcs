from pgmpy.inference import VariableElimination
from pgmpy.models import BayesianModel
from pgmpy.factors.discrete import TabularCPD

import decimal
decimal.getcontext().prec = 20

# Define the model structure (also see the instructions)
model = BayesianModel([('X', 'Y'),
                       ('Y', 'Z')])

# now defining the parameters.
cpd_x = TabularCPD(variable='X',
                   variable_card=2,
                   values=[
                       [0.1],
                       [0.9]
                   ])
cpd_y = TabularCPD(variable='Y',
                   variable_card=2,
                   values=[
                       [0.5, 0.2],
                       [0.5, 0.8]
                   ],
                   evidence=['X'],
                   evidence_card=[2])
cpd_z = TabularCPD(variable='Z',
                   variable_card=2,
                   values=[
                       [0.8, 0.8],
                       [0.2, 0.2]
                   ],
                   evidence=['Y'],
                   evidence_card=[2])

# Associating the parameters with the model structure.
model.add_cpds(cpd_x, cpd_y, cpd_z)

model.check_model()


def print_cpds(m):
    for cpd in m.get_cpds():
        print(f'CPT of {cpd.variable}:')
        print(cpd, '\n')

# Print model information
# print(model.edges())
# print(model.nodes())
print_cpds(model)


inference = VariableElimination(model)

# # Part 1
# print(inference.query(variables=['P','C']))

# # Part 2
print(inference.query(variables=['X'], evidence={'Y': 1, 'Z': 0}))
print(inference.query(variables=['X'], evidence={'Y': 1}))

# # Part 3
# print(inference.query(variables=['M'], evidence={'C': 1}))
# print(inference.query(variables=['T'], evidence={'C': 1}))
# print(inference.query(variables=['P','C','M','T']))

# # Part 4
# print(inference.query(variables=['M'], evidence={'C': 1}))
# print(inference.query(variables=['M'], evidence={'C': 1, 'P':1}))
# print(inference.query(variables=['M']))
# print(inference.query(variables=['T'], evidence={'M': 1}))
